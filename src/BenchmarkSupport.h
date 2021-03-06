
#include <glib.h>
#include "FillingAlgorithms.h"
#include "AfterMatrixFilling.h"

#ifndef BENCHMARK_SUPPORT_H
#define BENCHMARK_SUPPORT_H

typedef struct {
	Cell*** similarityMatrix;
	Island* alignment;
	gulong* fullExecutionTimes;
	gulong* kbandExecutionTimes;
	gint numberOfRuns;
	gint seq1Length;
	gint seq2Length;
	gint latestk;
} NWBenchmarkResult;

typedef struct {
	Cell*** similarityMatrix;
	GSList* islands;
	gulong* fullExecutionTimes;
	gint numberOfRuns;
	gint seq1Length;
	gint seq2Length;
} SWBenchmarkResult;

NWBenchmarkResult* execute_nw_benchmark(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, KBandOptions* kbandOptions, gint numberOfThreads);
SWBenchmarkResult* execute_sw_benchmark(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gint minimumScoreForIsland, gint numberOfThreads);

void nw_benchmark_result_free(NWBenchmarkResult* data);
void sw_benchmark_result_free(SWBenchmarkResult* data);

#endif