
#include <glib.h>
#include "Cell.h"
#include "AlignmentOptions.h"

#ifndef CELL_FILLING_SUPPORT_H
#define CELL_FILLING_SUPPORT_H

void fill(Cell*** matrix, ScoringOptions* options, gchar* seq1, gchar* seq2, gint x, gint y, gboolean isLocalAlignment);

#endif