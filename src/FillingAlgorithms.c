
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
				if (cell_isFlagSet (matrix[i][j], COMES_FROM_DIAGONAL)) printf("D "); else printf("  ");
				if (cell_isFlagSet (matrix[i][j], COMES_FROM_UP)) printf("U "); else printf("  ");
				if (cell_isFlagSet (matrix[i][j], COMES_FROM_LEFT)) printf("L "); else printf("  ");
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
		for (j = startY; j < yLimit; j++)
			fill (matrix, options, seq1, seq2, i, j, isLocalAlignment);
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

// Exposed functions

Cell*** create_similarity_matrix_full(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gboolean isLocalAlignment, gint numberOfThreads) 
{
	Cell*** matrix = create_matrix (seq1Length + 1, seq2Length + 1);
	
	if (matrix == NULL) 
		return NULL;
		
	if (numberOfThreads == 1)
		fill_matrix (matrix, scoringOptions, 0, 0, seq1Length + 1, seq2Length + 1, seq1, seq2, isLocalAlignment);
	
	// TODO parallel case
	
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
		; // TODO Parallel case
	
	return matrix;
}