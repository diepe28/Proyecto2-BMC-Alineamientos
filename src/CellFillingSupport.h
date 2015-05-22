
#include <glib.h>
#include "Cell.h"
#include "AlignmentOptions.h"

#ifndef CELL_FILLING_SUPPORT_H
#define CELL_FILLING_SUPPORT_H

void fill(Cell*** matrix, ScoringOptions* options, gint x, gint y, gboolean isLocalAlignment);

#endif