
#include "FillingAlgorithms.h"

void fill_matrix(Cell*** matrix, ScoringOptions* options, gint startX, gint startY, gint height, gint width, gchar* seq1, gchar* seq2, gboolean isLocalAlignment) 
{
	gint i, j = 0;
	gint xLimit = startX + height;
	gint yLimit = startY + width;
	for (i = startX; i < xLimit; i++)
		for (j = startY; j < yLimit; j++)
			fill (matrix, options, seq1, seq2, i, j, isLocalAlignment);
}

// For testing

static void print(Cell*** matrix, gint m, gint n)
{
	gint i, j = 0;
	for (i = 0; i < m; i++) 
	{
		printf("Row %d:\n", i);
		for (j = 0; j < n; j++) {
			printf("[%d][%d] = %d   ", i, j, matrix[i][j]->value);
			if (cell_isFlagSet (matrix[i][j], COMES_FROM_DIAGONAL)) printf("D "); else printf("  ");
			if (cell_isFlagSet (matrix[i][j], COMES_FROM_UP)) printf("U "); else printf("  ");
			if (cell_isFlagSet (matrix[i][j], COMES_FROM_LEFT)) printf("L "); else printf("  ");
			puts("");
		}
	}
}

void test() 
{
	gchar* seq1 = "GTACATTCTA";
	gchar* seq2 = "ATTGTGATCC";
	gint size1 = 11;
	gint size2 = 11;

	// Initialization
	Cell*** matrix = g_malloc(sizeof(Cell**) * size1);
	gint i, j = 0;
	for (i = 0; i < size1; i++)
		matrix[i] = g_malloc(sizeof(Cell*) * size2);
	ScoringOptions* options = g_malloc(sizeof(ScoringOptions));
	options->matchBonus = 1;
	options->missmatchPenalty = -1;
	options->gapOpeningPenalty = 0;
	options->gapExtensionPenalty = -2;
	options->freeLeftGapsForX = TRUE;
	options->freeLeftGapsForY = TRUE;
	options->substitutionMatrix = NULL;            

	// Call and printing
	fill_matrix (matrix, options, 0, 0, size1, size2, seq1, seq2, FALSE);
	print (matrix, size1, size2);
	
	// Cleaning
	for (i = 0; i < size1; i++) {
		for (j = 0; j < size2; j++)
			g_free(matrix[i][j]);
		g_free(matrix[i]);
	}
	g_free(matrix);
	g_free(options);
}