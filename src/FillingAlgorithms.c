
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