
#include <math.h>
#include <glib.h>
#include <pthread.h>
#include <semaphore.h>
#include "CellFillingSupport.h"

#ifndef FILLING_ALGORITHMS_H
#define FILLING_ALGORITHMS_H

typedef struct {
	gint threadID;
	Cell*** matrix;
	ScoringOptions* options;
	gchar* seq1;
	gchar* seq2;
	gboolean isLocalAlignment;
} FullFillParameters;

Cell*** create_similarity_matrix_full(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gboolean isLocalAlignment, gint numberOfThreads);
Cell*** create_similarity_matrix_kband(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, KBandOptions* kbandOptions, gint numberOfThreads);

#endif
