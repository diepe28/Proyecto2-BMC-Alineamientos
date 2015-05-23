
#include "CellFillingSupport.h"

static gint value_from_matrix(gchar a, gchar b)
{
	// TODO use substitutionMatrix
	return 0;
}

static void fill_corner(Cell*** matrix, ScoringOptions* options, gint x, gint y, gboolean isLocalAlignment) 
{
	if (options->gapOpeningPenalty == 0) {
		if (isLocalAlignment || (x == 0 && y == 0))
			matrix[x][y] = cell_new (0, NONE);
		else if (x == 0)
			matrix[x][y] = cell_new (options->freeLeftGapsForY ? 0 : (y * options->gapExtensionPenalty), COMES_FROM_LEFT);
		else if (y == 0)
			matrix[x][y] = cell_new (options->freeLeftGapsForX ? 0 : (x * options->gapExtensionPenalty), COMES_FROM_UP);
	}
	// TODO Affine function case
}

static void fill_interior(Cell*** matrix, ScoringOptions* options, gint x, gint y, gchar* seq1, gchar* seq2, gboolean isLocalAlignment) 
{
	Cell* newCell = NULL;
	if (options->gapOpeningPenalty == 0) {
		gint diagonalValue = ((options->substitutionMatrix != NULL) ? value_from_matrix(seq1[x-1], seq2[y-1]) :
							 ((seq1[x-1] == seq2[y-1]) ? options->matchBonus : options->missmatchPenalty)) + matrix[x-1][y-1]->value;
		gint leftValue = options->gapExtensionPenalty + matrix[x][y-1]->value;
		gint upValue = options->gapExtensionPenalty + matrix[x-1][y]->value;
		gint bestScore = MAX(diagonalValue, MAX(leftValue, upValue));
		if (isLocalAlignment && bestScore <= 0)
			newCell = cell_new (0, NONE);
		else {
			newCell = cell_new (bestScore, NONE);
			if (bestScore == diagonalValue) cell_setFlag (newCell, COMES_FROM_DIAGONAL);
			if (bestScore == leftValue) cell_setFlag (newCell, COMES_FROM_LEFT);
			if (bestScore == upValue) cell_setFlag (newCell, COMES_FROM_UP);
		}
	}

    // TODO Affine function case
	matrix[x][y] = newCell;
}

void fill(Cell*** matrix, ScoringOptions* options, gchar* seq1, gchar* seq2, gint x, gint y, gboolean isLocalAlignment) 
{
	if (x == 0 || y == 0)
		fill_corner (matrix, options, x, y, isLocalAlignment);
	else
		fill_interior (matrix, options, x, y, seq1, seq2, isLocalAlignment);
}