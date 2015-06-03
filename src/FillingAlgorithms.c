
#include "FillingAlgorithms.h"

// Helper functions

static void print_matrix(Cell*** matrix, gint m, gint n)
{
	gint i, j = 0;
	for (i = 0; i < m; i++) 
	{
		printf("Row %d:\n", i);
		for (j = 0; j < n; j++) {
			if (matrix[i][j] != NULL && matrix[i][j]->value_a > MIN_VALUE + 100) {
				printf("[%d][%d] = %d   ", i, j, matrix[i][j]->value_a);
				if (cell_isFlagASet (matrix[i][j], COMES_FROM_DIAGONAL)) printf("D "); else printf("  ");
				if (cell_isFlagASet (matrix[i][j], COMES_FROM_UP)) printf("U "); else printf("  ");
				if (cell_isFlagASet (matrix[i][j], COMES_FROM_LEFT)) printf("L "); else printf("  ");
			}
			else
				printf("[%d][%d] = -Inf   ", i, j);
			puts("");
		}
	}
}

static Cell*** create_matrix(gint height, gint width) 
{
	gint i, j = 0;

	Cell*** matrix = (Cell***) g_try_malloc0 (sizeof(Cell**) * height);
	for (i = 0; i < height; i++) {
		matrix[i] = (Cell**) g_try_malloc0 (sizeof(Cell*) * width);
		if (matrix[i] == NULL) {
			for (j = 0; j < i; j++)
				g_free (matrix[j]);
			g_free (matrix);
			return NULL;
		}
	}
	return matrix;
}

static void fill_matrix(Cell*** matrix, ScoringOptions* options, gint startX, gint startY, gint height, gint width, gchar* seq1, gchar* seq2, gboolean isLocalAlignment) 
{
	gint i, j = 0;
	gint xLimit = startX + height;
	gint yLimit = startY + width;
	for (i = startX; i < xLimit; i++)
		for (j = startY; j < yLimit; j++) {
			//printf("Computing [%d][%d]\n", i, j);
			fill (matrix, options, seq1, seq2, i, j, isLocalAlignment);
		}
}

static void fill_kband_boundaries(Cell*** matrix, gint seq1Length, gint seq2Length, gint k) 
{
	gint i, j = 0;
	gint diff = ABS(seq1Length - seq2Length);
	if (seq1Length < seq2Length) {
		for (i = 0, j = 0; i < (seq1Length - k); i++, j++) {
			matrix[i + k + 1][j] = cell_new (MIN_VALUE, NONE);
			matrix[i][j + diff + k + 1] = cell_new (MIN_VALUE, NONE);
		}	
	} else {
		for (i = 0, j = 0; i < (seq2Length - k); i++, j++) {
			matrix[i + diff + k + 1][j] = cell_new (MIN_VALUE, NONE);
			matrix[i][j + k + 1] = cell_new (MIN_VALUE, NONE);
		}
	}		
}

static gint number_of_antidiagonals(gint seq1Length, gint seq2Length) 
{
	return seq1Length + seq2Length + 1;
}

static void fill_antidiagonal(Cell*** matrix, gint n, gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, gint k, ScoringOptions* scoringOptions) 
{
	gint lowerBound, upperBound, i_init, j_init, i, j = 0;
	if (seq1Length < seq2Length) {
		lowerBound = seq1Length - seq2Length - k;
		upperBound = k;
	} else {
		lowerBound = -k;
		upperBound = seq1Length - seq2Length + k;
	}
	if (n <= upperBound) {
		i_init = n;
		j_init = 0;
	} else if (n >= seq1Length + MIN(seq1Length, seq2Length) - k) {
		i_init = seq1Length;
		j_init = n - seq1Length;
	} else {
		i_init = floorf((n - upperBound) / 2.0) + upperBound;
		j_init = ceilf((n - upperBound) / 2.0);
	}
	for (i = i_init, j = j_init; i >= 0 && j <= seq2Length && (i - j) >= lowerBound; i--, j++) {
		if (matrix[i][j] != NULL)
			g_free (matrix[i][j]);
		fill (matrix, scoringOptions, seq1, seq2, i, j, FALSE);
	}
}

// Parallel-helper functions

static void fill_strip(FullFillParameters* params, gboolean firstStrip) 
{
	gint startX = firstStrip ? params->strip1Start : params->strip2Start;
	gint startY = 0;
	gint blockHeight = firstStrip ? params->strip1Height : params->strip2Height;
	gint blockWidth = 0;
	gint n = params->widthExcess;
	
	while (startY < params->totalWidth) {
		if (startX != 0)
			if (sem_wait(params->waitingSemaphore))
				puts("ERROR from sem_wait()");
		blockWidth = (n > 0) ? (params->basicBlockWidth + 1) : params->basicBlockWidth;
		fill_matrix (params->matrix, params->options, startX, startY, blockHeight, blockWidth, params->seq1, params->seq2, params->isLocalAlignment);
		printf("Block finished from thread %d \n", params->threadID);
		startY += blockWidth;
		n--;
		if (sem_post(params->signalSemaphore))
			puts("ERROR from sem_post()");
	}
}

static void* fill_strips(void* parameters) 
{
	FullFillParameters* params = (FullFillParameters*) parameters;
	printf("Thread %d started processing... \n", params->threadID);

	if (params->strip1Height != 0)
		fill_strip (params, TRUE);

	if (params->strip2Height != 0)
		fill_strip (params, FALSE);
	
	printf("Thread %d finished processing... \n", params->threadID);
	pthread_exit((void *) 0);
}

static void fill_matrix_parallel(Cell*** matrix, ScoringOptions* options, gint height, gint width, gchar* seq1, gchar* seq2, gboolean isLocalAlignment, gint numberOfThreads)
{
	gint i = 0;
	pthread_t** threads = (pthread_t**) g_malloc(sizeof(pthread_t*) * numberOfThreads);
	FullFillParameters** parameters = (FullFillParameters**) g_malloc(sizeof(FullFillParameters*) * numberOfThreads);
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	gint basicBlockWidth = width / numberOfThreads;
	gint basicBlockHeight = height / (2 * numberOfThreads);
	gint widthExcess = width % numberOfThreads;
	gint heightExcess = height % (2 * numberOfThreads);

	sem_t** semaphores = (sem_t**) g_malloc(sizeof(sem_t*) * numberOfThreads);
	for (i = 0; i < numberOfThreads; i++) {
		semaphores[i] = (sem_t*) g_malloc(sizeof(sem_t));
		if (sem_init(semaphores[i], FALSE, 0))
			puts("ERROR from sem_init()");
	}

	// Debug
	printf("Width=%d \n", width);
	printf("Height=%d \n", height);
	printf("BasicBlockWidth=%d \n", basicBlockWidth);
	printf("BasicBlockHeight=%d \n", basicBlockHeight);
	printf("WidthExcess=%d \n", widthExcess);
	printf("HeightExcess=%d \n", heightExcess);
	// Debug
	
	for (i = 0; i < numberOfThreads; i++) {
		parameters[i] = (FullFillParameters*) g_malloc(sizeof(FullFillParameters));
		parameters[i]->threadID = i;
		parameters[i]->matrix = matrix;
		parameters[i]->options = options;
		parameters[i]->seq1 = seq1;
		parameters[i]->seq2 = seq2;
		parameters[i]->isLocalAlignment = isLocalAlignment;
		parameters[i]->basicBlockWidth = basicBlockWidth;
		parameters[i]->widthExcess = widthExcess;
		parameters[i]->totalWidth = width;
		parameters[i]->strip1Start = i * basicBlockHeight + MIN(i, heightExcess);
		parameters[i]->strip1Height = (i < heightExcess) ? (basicBlockHeight + 1) : (basicBlockHeight);
		parameters[i]->strip2Start = (i + numberOfThreads) * basicBlockHeight + MIN((i + numberOfThreads), heightExcess);
		parameters[i]->strip2Height = ((i + numberOfThreads) < heightExcess) ? (basicBlockHeight + 1) : (basicBlockHeight);
		parameters[i]->signalSemaphore = semaphores[i];
		parameters[i]->waitingSemaphore = (i == 0) ? semaphores[numberOfThreads-1] : semaphores[i-1];

		// Debug
		printf("Thread %d strip1Start=%d \n", i, parameters[i]->strip1Start);
		printf("Thread %d strip1Height=%d \n",  i, parameters[i]->strip1Height);
		printf("Thread %d strip2Start=%d \n", i, parameters[i]->strip2Start);
		printf("Thread %d strip2Height=%d \n",  i, parameters[i]->strip2Height);
		puts("");
		// Debug

		threads[i] = (pthread_t*) g_malloc(sizeof(pthread_t));
		if (pthread_create(threads[i], &attr, fill_strips, (void*)parameters[i]))
			puts("ERROR from pthread_create()");
	}

	pthread_attr_destroy(&attr);
	for (i = 0; i < numberOfThreads; i++) {
		if (pthread_join(*(threads[i]), NULL))
			puts("ERROR from pthread_join()");
		g_free(threads[i]);
		g_free(parameters[i]);
	}
	g_free(threads);
	g_free(parameters);

	for (i = 0; i < numberOfThreads; i++) {
		if (sem_destroy(semaphores[i]))
			puts("ERROR from sem_destroy()");
		g_free(semaphores[i]);
	}
	g_free(semaphores);

	puts("Paralel processing finished...");
}

// Exposed functions

Cell*** create_similarity_matrix_full(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gboolean isLocalAlignment, gint numberOfThreads) 
{
	Cell*** matrix = create_matrix (seq1Length + 1, seq2Length + 1);
	
	if (matrix == NULL) 
		return NULL;
		
	if (numberOfThreads == 1)
		fill_matrix (matrix, scoringOptions, 0, 0, seq1Length + 1, seq2Length + 1, seq1, seq2, isLocalAlignment);
	else
		fill_matrix_parallel (matrix, scoringOptions, seq1Length + 1, seq2Length + 1, seq1, seq2, isLocalAlignment, numberOfThreads);
	
	return matrix;
}

Cell*** create_similarity_matrix_kband(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, KBandOptions* kbandOptions, gint numberOfThreads) 
{
	Cell*** matrix = create_matrix (seq1Length + 1, seq2Length + 1);
	
	if (matrix == NULL) 
		return NULL;

	gint i = 0;
	gint k = kbandOptions->kInitValue;
	gint minLength = MIN(seq1Length, seq2Length);
	gint maxLength = MAX(seq1Length, seq2Length);
	
	while (k < minLength) {
		fill_kband_boundaries (matrix, seq1Length, seq2Length, k);
		if (numberOfThreads == 1) {
			for (i = 0; i < number_of_antidiagonals (seq1Length, seq2Length); i++)
				fill_antidiagonal (matrix, i, seq1, seq2, seq1Length, seq2Length, k, scoringOptions);
		}
		else
			; // TODO Parallel case.
		gint bestScore = matrix[seq1Length][seq2Length]->value_a;
		gint nextKBound = (2*(k + 1) + maxLength - minLength)*scoringOptions->gapExtensionPenalty + (minLength - (k + 1))*scoringOptions->matchBonus;
		if (bestScore >= nextKBound)
			return matrix;
		k += kbandOptions->kExtensionValue;
	}

	if (numberOfThreads == 1)
		fill_matrix (matrix, scoringOptions, 0, 0, seq1Length + 1, seq2Length + 1, seq1, seq2, FALSE);
	else
		fill_matrix_parallel (matrix, scoringOptions, seq1Length + 1, seq2Length + 1, seq1, seq2, FALSE, numberOfThreads);
	
	return matrix;
}