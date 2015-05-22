
#include "CellFillingSupport.h"

static void fill_corner(Cell*** matrix, ScoringOptions* options, gint x, gint y, gboolean isLocalAlignment) 
{
	if (options->gapOpeningPenalty == 0) 
		if (isLocalAlignment || (x == 0 && y == 0))
			matrix[x][y] = cell_new (0, NONE);
		else if (x == 0)
			matrix[x][y] = cell_new (options->freeLeftGapsForY ? 0 : (y * options->gapExtensionPenalty), COMES_FROM_LEFT);
		else if (y == 0)
			matrix[x][y] = cell_new (options->freeLeftGapsForX ? 0 : (x * options->gapExtensionPenalty), COMES_FROM_UP);
	else
		// Affine function case
		return;
}

static void fill_interior(Cell*** matrix, ScoringOptions* options, gint x, gint y, gboolean isLocalAlignment) 
{
	if (options->gapOpeningPenalty == 0) {
		gint bestScore = MAX(matrix[x-1][y-1]->value, MAX(matrix[x-1][y]->value, matrix[x][y-1]->value));
		
		
	}
}

void fill(Cell*** matrix, ScoringOptions* options, gint x, gint y, gboolean isLocalAlignment) 
{
	if (x == 0 || y == 0)
		fill_corner (matrix, options, x, y, isLocalAlignment);
	else
		fill_interior (matrix, options, x, y, isLocalAlignment)
}