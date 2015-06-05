
#include "BenchmarkSupport.h"

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

NWBenchmarkResult* execute_nw_benchmark(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, KBandOptions* kbandOptions, gint numberOfThreads)
{
	gint i = 0;
	Cell*** similarityMatrix = NULL;
	Island* result = NULL;
	gulong* fullExecutionTimes = NULL;
	gulong* kbandExecutionTimes = NULL;

	fullExecutionTimes = (gulong*) g_malloc(sizeof(gulong) * numberOfThreads);
	for (i = 0; i < numberOfThreads; i++) {
		if (similarityMatrix != NULL && result != NULL) {
			island_free(result);
			free_matrix(similarityMatrix, seq1Length + 1, seq2Length + 1);
		}
		GTimer* timer = g_timer_new();
		similarityMatrix = create_similarity_matrix_full(seq1, seq2, seq1Length, seq2Length, scoringOptions, FALSE, i + 1);
		if (similarityMatrix == NULL) {
			g_timer_stop(timer);
			g_timer_destroy(timer);
			g_free(fullExecutionTimes);
			return NULL;
		}
		result = afterMatrixFilling_find_NW_Alignment(similarityMatrix, seq2, seq1, seq1Length, seq2Length, scoringOptions->freeRightGapsForX, scoringOptions->freeRightGapsForY, scoringOptions->gapOpeningPenalty != 0);
		g_timer_stop(timer);
		gulong fractional_part = 0;
		gdouble seconds_elapsed = g_timer_elapsed(timer, &fractional_part);
		fullExecutionTimes[i] = ((gulong)(seconds_elapsed * 1000000)) + fractional_part;
		g_timer_destroy(timer);
	}

	if (kbandOptions != NULL) {
		kbandExecutionTimes = (gulong*) g_malloc(sizeof(gulong) * numberOfThreads);
		for (i = 0; i < numberOfThreads; i++) {
			if (similarityMatrix != NULL && result != NULL) {
				island_free(result);
				free_matrix(similarityMatrix, seq1Length + 1, seq2Length + 1);
			}
			GTimer* timer = g_timer_new();
			similarityMatrix = create_similarity_matrix_kband(seq1, seq2, seq1Length, seq2Length, scoringOptions, kbandOptions, i + 1);
			if (similarityMatrix == NULL) {
				g_timer_stop(timer);
				g_timer_destroy(timer);
				g_free(fullExecutionTimes);
				g_free(kbandExecutionTimes);
				return NULL;
			}
			result = afterMatrixFilling_find_NW_Alignment(similarityMatrix, seq2, seq1, seq1Length, seq2Length, scoringOptions->freeRightGapsForX, scoringOptions->freeRightGapsForY, scoringOptions->gapOpeningPenalty != 0);
			g_timer_stop(timer);
			gulong fractional_part = 0;
			gdouble seconds_elapsed = g_timer_elapsed(timer, &fractional_part);
			kbandExecutionTimes[i] = ((gulong)(seconds_elapsed * 1000000)) + fractional_part;
			g_timer_destroy(timer);
		}
	}

	NWBenchmarkResult* benchmarkResult = (NWBenchmarkResult*) g_malloc(sizeof(NWBenchmarkResult));
	benchmarkResult->similarityMatrix = similarityMatrix;
	benchmarkResult->result = result;
	benchmarkResult->fullExecutionTimes = fullExecutionTimes;
	benchmarkResult->kbandExecutionTimes = kbandExecutionTimes;
	benchmarkResult->numberOfRuns = numberOfThreads;
	benchmarkResult->seq1Length = seq1Length;
	benchmarkResult->seq2Length = seq1Length;
	return benchmarkResult;
}

SWBenchmarkResult* execute_sw_benchmark(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gint minimumScoreForIsland, gint numberOfThreads)
{
	gint i = 0;
	Cell*** similarityMatrix = NULL;
	GSList* result = NULL;
	gulong* fullExecutionTimes = NULL;

	fullExecutionTimes = (gulong*) g_malloc(sizeof(gulong) * numberOfThreads);
	for (i = 0; i < numberOfThreads; i++) {
		if (similarityMatrix != NULL && result != NULL) {
			g_slist_free_full(result, island_destroyer);
			free_matrix(similarityMatrix, seq1Length + 1, seq2Length + 1);
		}
		GTimer* timer = g_timer_new();
		similarityMatrix = create_similarity_matrix_full(seq1, seq2, seq1Length, seq2Length, scoringOptions, TRUE, i + 1);
		if (similarityMatrix == NULL) {
			g_timer_stop(timer);
			g_timer_destroy(timer);
			g_free(fullExecutionTimes);
			return NULL;
		}
		result = afterMatrixFilling_findLocalAlignments(similarityMatrix, seq2, seq1, seq1Length, seq2Length, minimumScoreForIsland);
		g_timer_stop(timer);
		gulong fractional_part = 0;
		gdouble seconds_elapsed = g_timer_elapsed(timer, &fractional_part);
		fullExecutionTimes[i] = ((gulong)(seconds_elapsed * 1000000)) + fractional_part;
		g_timer_destroy(timer);
	}

	SWBenchmarkResult* benchmarkResult = (SWBenchmarkResult*) g_malloc(sizeof(SWBenchmarkResult));
	benchmarkResult->similarityMatrix = similarityMatrix;
	benchmarkResult->result = result;
	benchmarkResult->fullExecutionTimes = fullExecutionTimes;
	benchmarkResult->numberOfRuns = numberOfThreads;
	benchmarkResult->seq1Length = seq1Length;
	benchmarkResult->seq2Length = seq1Length;
	return benchmarkResult;
}

void nw_benchmark_result_free(NWBenchmarkResult* data) 
{
	island_free(data->result);
	free_matrix(data->similarityMatrix, data->seq1Length + 1, data->seq2Length + 1);
	g_free(data->fullExecutionTimes);
	if (data->kbandExecutionTimes != NULL)
		g_free(data->kbandExecutionTimes);
	g_free(data);
}

void sw_benchmark_result_free(SWBenchmarkResult* data)
{
	g_slist_free_full(data->result, island_destroyer);
	free_matrix(data->similarityMatrix, data->seq1Length + 1, data->seq2Length + 1);
	g_free(data->fullExecutionTimes);
	g_free(data);
}