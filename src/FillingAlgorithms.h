
#include <glib.h>
#include "CellFillingSupport.h"
#include "SubstitutionMatrixes.h"

#ifndef FILLING_ALGORITHMS_H
#define FILLING_ALGORITHMS_H

Cell*** create_similarity_matrix_full(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gboolean isLocalAlignment, gint numberOfThreads);
Cell*** create_similarity_matrix_kband(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, KBandOptions* kbandOptions, gint numberOfThreads);

void test();

#endif
