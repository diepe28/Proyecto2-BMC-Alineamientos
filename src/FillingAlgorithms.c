
#include "FillingAlgorithms.h"

// Helper functions

static Cell*** create_matrix(gint height, gint width) 
{
	gint i = 0;

	// TODO Return NULL if the requested memory is too much
	
	Cell*** matrix = (Cell***) g_malloc(sizeof(Cell**) * height);
	for (i = 0; i < height; i++)
		matrix[i] = (Cell**) g_malloc(sizeof(Cell*) * width);
	return matrix;
}

static void fill_matrix(Cell*** matrix, ScoringOptions* options, gint startX, gint startY, gint height, gint width, gchar* seq1, gchar* seq2, gboolean isLocalAlignment) 
{
	gint i, j = 0;
	gint xLimit = startX + height;
	gint yLimit = startY + width;
	for (i = startX; i < xLimit; i++)
		for (j = startY; j < yLimit; j++)
			fill (matrix, options, seq1, seq2, i, j, isLocalAlignment);
}

// Exposed functions

Cell*** create_similarity_matrix_full(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, gboolean isLocalAlignment, gint numberOfThreads) 
{
	Cell*** matrix = create_matrix (seq1Length + 1, seq2Length + 1);
	
	if (matrix == NULL) 
		return NULL;
		
	if (numberOfThreads == 1)
		fill_matrix (matrix, scoringOptions, 0, 0, seq1Length + 1, seq2Length + 1, seq1, seq2, isLocalAlignment);
	
	// TODO parallel case
	
	return matrix;
}

Cell*** create_similarity_matrix_kband(gchar* seq1, gchar* seq2, gint seq1Length, gint seq2Length, ScoringOptions* scoringOptions, KBandOptions* kbandOptions, gint numberOfThreads) 
{
	// TODO
	return NULL;
}

// Testing functions.

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
	gint i, j = 0;
	// Initialization

	ScoringOptions* options = g_malloc(sizeof(ScoringOptions));
	options->matchBonus = 1;
	options->missmatchPenalty = -1;
	options->gapOpeningPenalty = 0;
	options->gapExtensionPenalty = -2;
	options->freeLeftGapsForX = TRUE;
	options->freeLeftGapsForY = TRUE;
	options->substitutionMatrix = NULL;

	// Call and printing
	Cell*** matrix = create_similarity_matrix_full (seq1, seq2, 10, 10, options, FALSE, 1);
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