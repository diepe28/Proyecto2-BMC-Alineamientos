
#include "BenchmarkSupport.h"

static Cell*** create_matrix(gint height, gint width) 
{
	gint i, j = 0;
	Cell*** matrix = (Cell***) g_malloc0 (sizeof(Cell**) * height);
	for (i = 0; i < height; i++) {
		matrix[i] = (Cell**) g_malloc0 (sizeof(Cell*) * width);
		for (j = 0; j < width; j++)
			matrix[i][j] = cell_new (0, NONE);
	}
	return matrix;
}

static void free_matrix(Cell*** matrix, gint n, gint m)
{
	gint i, j = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			if (matrix[i][j] != NULL)
				g_free(matrix[i][j]);
		g_free(matrix[i]);
	}
	g_free(matrix);
}

static void warm_up(Cell*** similarityMatrix, gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gboolean local) 
{
	fill_similarity_matrix_full(similarityMatrix, seq1, seq2, seq1Length, seq2Length, scoringOptions, local, 2);
}

NWBenchmarkResult* execute_nw_benchmark(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, KBandOptions* kbandOptions, gint numberOfThreads)
{
	gint i = 0;
	Cell*** similarityMatrix = NULL;
	Island* alignment = NULL;
	gulong* fullExecutionTimes = NULL;
	gulong* kbandExecutionTimes = NULL;

	similarityMatrix = create_matrix (seq1Length + 1, seq2Length + 1);
	fullExecutionTimes = (gulong*) g_malloc(sizeof(gulong) * numberOfThreads);
	
	for (i = 0; i < numberOfThreads; i++) {
		if (alignment != NULL)
			island_free(alignment);

		if (i == 1)
			warm_up (similarityMatrix, seq1, seq2, seq1Length, seq2Length, scoringOptions, FALSE);
		
		GTimer* timer = g_timer_new();
		fill_similarity_matrix_full(similarityMatrix, seq1, seq2, seq1Length, seq2Length, scoringOptions, FALSE, i + 1);
		alignment = afterMatrixFilling_find_NW_Alignment(similarityMatrix, seq2, seq1, seq1Length, seq2Length, scoringOptions->freeRightGapsForX, scoringOptions->freeRightGapsForY, scoringOptions->gapOpeningPenalty != 0);
		g_timer_stop(timer);
		gulong fractional_part = 0;
		gdouble seconds_elapsed = g_timer_elapsed(timer, &fractional_part);
		fullExecutionTimes[i] = ((gulong)(seconds_elapsed * 1000000)) + fractional_part;
		g_timer_destroy(timer);

		printf("Full Run #%d result=%d \n", i, similarityMatrix[seq1Length][seq2Length]->value_a);
	}

	if (kbandOptions != NULL) {
		kbandExecutionTimes = (gulong*) g_malloc(sizeof(gulong) * numberOfThreads);
		for (i = 0; i < numberOfThreads; i++) {
			if (alignment != NULL)
				island_free(alignment);
			
			GTimer* timer = g_timer_new();
			fill_similarity_matrix_kband(similarityMatrix, seq1, seq2, seq1Length, seq2Length, scoringOptions, kbandOptions, i + 1);
			alignment = afterMatrixFilling_find_NW_Alignment(similarityMatrix, seq2, seq1, seq1Length, seq2Length, scoringOptions->freeRightGapsForX, scoringOptions->freeRightGapsForY, scoringOptions->gapOpeningPenalty != 0);
			g_timer_stop(timer);
			gulong fractional_part = 0;
			gdouble seconds_elapsed = g_timer_elapsed(timer, &fractional_part);
			kbandExecutionTimes[i] = ((gulong)(seconds_elapsed * 1000000)) + fractional_part;
			g_timer_destroy(timer);

			printf("KBand Full Run #%d result=%d \n", i, similarityMatrix[seq1Length][seq2Length]->value_a);
		}
	}

	NWBenchmarkResult* benchmarkResult = (NWBenchmarkResult*) g_malloc(sizeof(NWBenchmarkResult));
	benchmarkResult->similarityMatrix = similarityMatrix;
	benchmarkResult->alignment = alignment;
	benchmarkResult->fullExecutionTimes = fullExecutionTimes;
	benchmarkResult->kbandExecutionTimes = kbandExecutionTimes;
	benchmarkResult->numberOfRuns = numberOfThreads;
	benchmarkResult->seq1Length = seq1Length;
	benchmarkResult->seq2Length = seq2Length;
	return benchmarkResult;
}

SWBenchmarkResult* execute_sw_benchmark(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gint minimumScoreForIsland, gint numberOfThreads)
{
	gint i = 0;
	Cell*** similarityMatrix = NULL;
	GSList* islands = NULL;
	gulong* fullExecutionTimes = NULL;

	similarityMatrix = create_matrix (seq1Length + 1, seq2Length + 1);
	fullExecutionTimes = (gulong*) g_malloc(sizeof(gulong) * numberOfThreads);
	
	for (i = 0; i < numberOfThreads; i++) {
		if (islands != NULL)
			g_slist_free_full(islands, island_destroyer);

		if (i == 1)
			warm_up (similarityMatrix, seq1, seq2, seq1Length, seq2Length, scoringOptions, TRUE);
		
		GTimer* timer = g_timer_new();
		fill_similarity_matrix_full(similarityMatrix, seq1, seq2, seq1Length, seq2Length, scoringOptions, TRUE, i + 1);
		islands = afterMatrixFilling_findLocalAlignments(similarityMatrix, seq2, seq1, seq1Length, seq2Length, minimumScoreForIsland);
		g_timer_stop(timer);
		gulong fractional_part = 0;
		gdouble seconds_elapsed = g_timer_elapsed(timer, &fractional_part);
		fullExecutionTimes[i] = ((gulong)(seconds_elapsed * 1000000)) + fractional_part;
		g_timer_destroy(timer);

		printf("Local Run #%d \n", i);
	}

	SWBenchmarkResult* benchmarkResult = (SWBenchmarkResult*) g_malloc(sizeof(SWBenchmarkResult));
	benchmarkResult->similarityMatrix = similarityMatrix;
	benchmarkResult->islands = islands;
	benchmarkResult->fullExecutionTimes = fullExecutionTimes;
	benchmarkResult->numberOfRuns = numberOfThreads;
	benchmarkResult->seq1Length = seq1Length;
	benchmarkResult->seq2Length = seq2Length;
	return benchmarkResult;
}

void nw_benchmark_result_free(NWBenchmarkResult* data) 
{
	island_free(data->alignment);
	free_matrix(data->similarityMatrix, data->seq1Length + 1, data->seq2Length + 1);
	g_free(data->fullExecutionTimes);
	if (data->kbandExecutionTimes != NULL)
		g_free(data->kbandExecutionTimes);
	g_free(data);
}

void sw_benchmark_result_free(SWBenchmarkResult* data)
{
	g_slist_free_full(data->islands, island_destroyer);
	free_matrix(data->similarityMatrix, data->seq1Length + 1, data->seq2Length + 1);
	g_free(data->fullExecutionTimes);
	g_free(data);
}