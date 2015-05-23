
#include <glib.h>
#include "CellFillingSupport.h"

#ifndef FILLING_ALGORITHMS_H
#define FILLING_ALGORITHMS_H

void fill_matrix(Cell*** matrix, ScoringOptions* options, gint startX, gint startY, gint height, gint width, gchar* seq1, gchar* seq2, gboolean isLocalAlignment);
void test();

#endif
