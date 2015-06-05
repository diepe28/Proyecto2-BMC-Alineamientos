
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
	gint i = 0, j = 0;
	Cell*** similarityMatrix = NULL;
	Island* result = NULL;
	glong* fullExecutionTimes = NULL;
	glong* kbandExecutionTimes = NULL;

	fullExecutionTimes = (glong*) g_malloc(sizeof(glong) * numberOfThreads);
	for (i = 0; i < numberOfThreads; i++) {
		if (similarityMatrix != NULL && result != NULL) {
			island_free(result);
			free_matrix(similarityMatrix, seq1Length + 1, seq2Length + 1);
		}
		// START TIMER
		similarityMatrix = create_similarity_matrix_full(seq1, seq2, seq1Length, seq2Length, scoringOptions, FALSE, i + 1);
		if (similarityMatrix == NULL) {
			for (j = 0; j < i; j++) g_free(fullExecutionTimes[j]);
			g_free(fullExecutionTimes);
			return NULL;
		}
		result = afterMatrixFilling_find_NW_Alignment(similarityMatrix, seq2, seq1, seq1Length + 1, seq2Length + 1, scoringOptions->freeRightGapsForX, scoringOptions->freeRightGapsForY, scoringOptions->gapOpeningPenalty != 0);
		// END TIMER
		// COLLECT TIME
	}

	if (kbandOptions != NULL) {
		kbandExecutionTimes = (glong*) g_malloc(sizeof(glong) * numberOfThreads);
		for (i = 0; i < numberOfThreads; i++) {
			if (similarityMatrix != NULL && result != NULL) {
				island_free(result);
				free_matrix(similarityMatrix, seq1Length + 1, seq2Length + 1);
			}
			// START TIMER
			similarityMatrix = create_similarity_matrix_kband(seq1, seq2, seq1Length, seq2Length, scoringOptions, kbandOptions, i + 1);
			if (similarityMatrix == NULL) {
				for (j = 0; j < numberOfThreads; j++) g_free(fullExecutionTimes[j]);
				g_free(fullExecutionTimes);
				for (j = 0; j < i; j++) g_free(kbandExecutionTimes[j]);
				g_free(kbandExecutionTimes);
				return NULL;
			}
			result = afterMatrixFilling_find_NW_Alignment(similarityMatrix, seq2, seq1, seq1Length + 1, seq2Length + 1, scoringOptions->freeRightGapsForX, scoringOptions->freeRightGapsForY, scoringOptions->gapOpeningPenalty != 0);
			// END TIMER
			// COLLECT TIME
		}
	}

	NWBenchmarkResult* benchmarkResult = (NWBenchmarkResult*) g_malloc(sizeof(NWBenchmarkResult));
	benchmarkResult->similarityMatrix = similarityMatrix;
	benchmarkResult->result = result;
	benchmarkResult->fullExecutionTimes = fullExecutionTimes;
	benchmarkResult->kbandExecutionTimes = kbandExecutionTimes;
	benchmarkResult->numberOfResults = numberOfThreads;
	benchmarkResult->seq1Length = seq1Length;
	benchmarkResult->seq2Length = seq1Length;
	return benchmarkResult;
}

SWBenchmarkResult* execute_sw_benchmark(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gint minimumScoreForIsland, gint numberOfThreads)
{
	gint i = 0, j = 0;
	Cell*** similarityMatrix = NULL;
	GSList* result = NULL;
	glong* fullExecutionTimes = NULL;

	fullExecutionTimes = (glong*) g_malloc(sizeof(glong) * numberOfThreads);
	for (i = 0; i < numberOfThreads; i++) {
		if (similarityMatrix != NULL && result != NULL) {
			g_slist_free_full(result, island_destroyer);
			free_matrix(similarityMatrix, seq1Length + 1, seq2Length + 1);
		}
		// START TIMER
		similarityMatrix = create_similarity_matrix_full(seq1, seq2, seq1Length, seq2Length, scoringOptions, TRUE, i + 1);
		if (similarityMatrix == NULL) {
			for (j = 0; j < i; j++) g_free(fullExecutionTimes[j]);
			g_free(fullExecutionTimes);
			return NULL;
		}
		result = afterMatrixFilling_findLocalAlignments(similarityMatrix, seq2, seq1, seq1Length + 1, seq2Length + 1, minimumScoreForIsland);
		// END TIMER
		// COLLECT TIME
	}

	SWBenchmarkResult* benchmarkResult = (SWBenchmarkResult*) g_malloc(sizeof(SWBenchmarkResult));
	benchmarkResult->similarityMatrix = similarityMatrix;
	benchmarkResult->result = result;
	benchmarkResult->fullExecutionTimes = fullExecutionTimes;
	benchmarkResult->numberOfResults = numberOfThreads;
	benchmarkResult->seq1Length = seq1Length;
	benchmarkResult->seq2Length = seq1Length;
	return benchmarkResult;
}

void nw_benchmark_result_free(NWBenchmarkResult* data) 
{
	gint i = 0;
	island_free(data->result);
	free_matrix(data->similarityMatrix, data->seq1Length + 1, data->seq2Length + 1);
	for (i = 0; i < data->numberOfResults; i++)
		g_free(data->fullExecutionTimes[i]);
	g_free(data->fullExecutionTimes);
	if (data->kbandExecutionTimes != NULL) {
		for (i = 0; i < data->numberOfResults; i++)
			g_free(data->kbandExecutionTimes[i]);
		g_free(data->kbandExecutionTimes);
	}
	g_free(data);
}

void sw_benchmark_result_free(SWBenchmarkResult* data)
{
	gint i = 0;
	g_slist_free_full(data->result, island_destroyer);
	free_matrix(data->similarityMatrix, data->seq1Length + 1, data->seq2Length + 1);
	for (i = 0; i < data->numberOfResults; i++)
		g_free(data->fullExecutionTimes[i]);
	g_free(data->fullExecutionTimes);
	g_free(data);
}