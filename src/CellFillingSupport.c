
#include "CellFillingSupport.h"

static gint value_from_matrix(gint (*substitutionMatrix)[26], gchar a, gchar b)
{
	return substitutionMatrix[(gint)a - 65][(gint)b - 65];
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
	} else {
		if (x == 0 && y == 0)
			matrix[x][y] = triple_cell_new (0, MIN_VALUE, MIN_VALUE, NONE, NONE, NONE);
	    else if (x == 0)
			matrix[x][y] = triple_cell_new (isLocalAlignment ? 0 : MIN_VALUE,
			                                (options->freeLeftGapsForY) ? 0 : (options->gapOpeningPenalty + options->gapExtensionPenalty * y),
			                                MIN_VALUE,
			                                (!isLocalAlignment && y == 1) ? COMES_FROM_LEFT : NONE,
			                                (!isLocalAlignment && y != 1) ? COMES_FROM_LEFT : NONE,
			                                NONE
			                                );
		else if (y == 0)
			matrix[x][y] = triple_cell_new (isLocalAlignment ? 0 : MIN_VALUE,
			                                MIN_VALUE,
			                                (options->freeLeftGapsForX) ? 0 : (options->gapOpeningPenalty + options->gapExtensionPenalty * x),
			                                (!isLocalAlignment && x == 1) ? COMES_FROM_UP : NONE,
			                                NONE,
			                                (!isLocalAlignment && x != 1) ? COMES_FROM_UP : NONE
			                                );
	}
}

static void fill_interior(Cell*** matrix, ScoringOptions* options, gint x, gint y, gchar* seq1, gchar* seq2, gboolean isLocalAlignment) 
{
	Cell* newCell = NULL;
	gint fValue = ((options->substitutionMatrix != NULL) ? value_from_matrix(options->substitutionMatrix, seq1[x-1], seq2[y-1]) :
				  ((seq1[x-1] == seq2[y-1]) ? options->matchBonus : options->missmatchPenalty));
	if (options->gapOpeningPenalty == 0) {
		gint diagonalValue = fValue + matrix[x-1][y-1]->value_a;
		gint leftValue = options->gapExtensionPenalty + matrix[x][y-1]->value_a;
		gint upValue = options->gapExtensionPenalty + matrix[x-1][y]->value_a;
		gint bestScore = MAX(diagonalValue, MAX(leftValue, upValue));
		if (isLocalAlignment && bestScore <= 0)
			newCell = cell_new (0, NONE);
		else {
			newCell = cell_new (bestScore, NONE);
			if (bestScore == diagonalValue) cell_setFlagA (newCell, COMES_FROM_DIAGONAL);
			if (bestScore == leftValue) cell_setFlagA (newCell, COMES_FROM_LEFT);
			if (bestScore == upValue) cell_setFlagA (newCell, COMES_FROM_UP);
		}
	} else {
		gint flagsA = NONE;
		gint flagsB = NONE;
		gint flagsC = NONE;
		
		gint diagonalValueA = fValue + matrix[x-1][y-1]->value_a;
		gint diagonalValueB = fValue + matrix[x-1][y-1]->value_b;
		gint diagonalValueC = fValue + matrix[x-1][y-1]->value_c;
		gint bestAValue = MAX(diagonalValueA, MAX(diagonalValueB, diagonalValueC));
		if (isLocalAlignment && bestAValue <= 0)
			bestAValue = 0;
		else {
			if (bestAValue == diagonalValueA) flagsA |= COMES_FROM_DIAGONAL;
			if (bestAValue == diagonalValueB) flagsB |= COMES_FROM_DIAGONAL;
			if (bestAValue == diagonalValueC) flagsC |= COMES_FROM_DIAGONAL;
		}

		gint leftValueA = matrix[x][y-1]->value_a + options->gapOpeningPenalty + options->gapExtensionPenalty;
		gint leftValueB = matrix[x][y-1]->value_b + options->gapExtensionPenalty;
		gint leftValueC = matrix[x][y-1]->value_c + options->gapOpeningPenalty + options->gapExtensionPenalty;
		gint bestBValue = MAX(leftValueA, MAX(leftValueB, leftValueC));
		if (bestBValue == leftValueA) flagsA |= COMES_FROM_LEFT;
		if (bestBValue == leftValueB) flagsB |= COMES_FROM_LEFT;
		if (bestBValue == leftValueC) flagsC |= COMES_FROM_LEFT;

		gint upValueA = matrix[x-1][y]->value_a + options->gapOpeningPenalty + options->gapExtensionPenalty;
		gint upValueB = matrix[x-1][y]->value_b + options->gapOpeningPenalty + options->gapExtensionPenalty;
		gint upValueC = matrix[x-1][y]->value_c + options->gapExtensionPenalty;
		gint bestCValue = MAX(upValueA, MAX(upValueB, upValueC));
		if (bestCValue == upValueA) flagsA |= COMES_FROM_UP;
		if (bestCValue == upValueB) flagsB |= COMES_FROM_UP;
		if (bestCValue == upValueC) flagsC |= COMES_FROM_UP;
		
		newCell = triple_cell_new (bestAValue, bestBValue, bestCValue, flagsA, flagsB, flagsC);
	}
	matrix[x][y] = newCell;
}

void fill(Cell*** matrix, ScoringOptions* options, gchar* seq1, gchar* seq2, gint x, gint y, gboolean isLocalAlignment) 
{
	if (x == 0 || y == 0)
		fill_corner (matrix, options, x, y, isLocalAlignment);
	else
		fill_interior (matrix, options, x, y, seq1, seq2, isLocalAlignment);
}
