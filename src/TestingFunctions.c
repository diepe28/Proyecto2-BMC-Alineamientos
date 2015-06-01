#include "TestingFunctions.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////TEST OF FUNCTIONS///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static void printMatrix(int n, int m,Cell*** matrix){
	int i,j;
	printf("\n");
	for(i = 0; i < n+1; i++){
		for(j = 0; j < m+1; j++){
			if(cell_isFlagASet (matrix[i][j], IS_PAINTED))
				printf("%d\t", matrix[i][j]->value_a);
			else
				printf(".\t");
		}
		printf("\n");
	}
	printf("\n");
}


void testGlobalAlingment(){
	gchar* seq2 = "TTGCATCGGCG";
	gchar* seq1 = "ATTGTGATCCGGGGGGGG";
	gint n = 12;
	gint m = 19;
	gint i, j = 0;
	// Initialization

	ScoringOptions* options = g_malloc(sizeof(ScoringOptions));
	options->matchBonus = 1;
	options->missmatchPenalty = -1;
	options->gapOpeningPenalty = 0;
	options->gapExtensionPenalty = -2;
	options->freeLeftGapsForX = FALSE;
	options->freeLeftGapsForY = FALSE;
	options->substitutionMatrix = NULL;

	Cell*** matrix = create_similarity_matrix_full(seq1, seq2, n, m, options, FALSE, 1);
	char** sequences = afterMatrixFilling_find_NW_Alignment (matrix, seq1, seq2, n, m,
	                                                         FALSE, FALSE, FALSE);

	printf("Text1: %s\n", sequences[0]);
	printf("Text2: %s\n", sequences[1]);

	g_free(sequences[0]);
	g_free(sequences[1]);
	g_free(sequences);
	
	for(i = 0; i < n+1; i++){
		for(j = 0; j < m+1; j++){
			g_free(matrix[i][j]);
		}
	}

	for(i = 0; i < n+1; i++){
		g_free(matrix[i]);
	}

	g_free(matrix);
}


void testSemiGlobalAlingment(){
	gchar * up =   "ATTGTGATCC";
	gchar * left = "GTACATTCTA";
	gint i , j /*,gapPenalty = -2*/;
	gint n = strlen(up), m = strlen(left);

	Cell ***  mat = (Cell***) (g_malloc((n+1)* sizeof(Cell*)));
	for(i = 0; i < n+1; i++){
		mat[i] = (Cell**) (g_malloc((m+1)* sizeof(Cell*)));
	}       

	mat[0][0] = cell_new (0,0);
	//first row and col
	for(i = 1; i < n+1; i++){
		mat[0][i] = cell_new (0, COMES_FROM_LEFT );
		mat[i][0] = cell_new (0, COMES_FROM_UP);
	}


	mat[1][1]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][2]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][3]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][4]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][5]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][6]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][7]  = cell_new (-1, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[1][8]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][9]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][10] = cell_new (-1, COMES_FROM_DIAGONAL);

	mat[2][1]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[2][2]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[2][3]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[2][4]  = cell_new (-1, COMES_FROM_UP);
	mat[2][5]  = cell_new (2,  COMES_FROM_DIAGONAL);
	mat[2][6]  = cell_new (0,  COMES_FROM_LEFT);
	mat[2][7]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[2][8]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[2][9]  = cell_new (-2, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[2][10] = cell_new (-2, COMES_FROM_DIAGONAL);
	
	mat[3][1]  = cell_new (1,  COMES_FROM_DIAGONAL);
	mat[3][2]  = cell_new (-1, COMES_FROM_LEFT);
	mat[3][3]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[3][4]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[3][5]  = cell_new (0,  COMES_FROM_UP);
	mat[3][6]  = cell_new (1,  COMES_FROM_DIAGONAL);
	mat[3][7]  = cell_new (1,  COMES_FROM_DIAGONAL);
	mat[3][8]  = cell_new (-1, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[3][9]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[3][10] = cell_new (-3, COMES_FROM_LEFT | COMES_FROM_DIAGONAL);
	
	mat[4][1]  = cell_new (-1, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[4][2]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[4][3]  = cell_new (-2, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[4][4]  = cell_new (-2, COMES_FROM_DIAGONAL);
	mat[4][5]  = cell_new (-2, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[4][6]  = cell_new (-1, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[4][7]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[4][8]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[4][9]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[4][10] = cell_new (0,  COMES_FROM_DIAGONAL);

	mat[5][1]  = cell_new (1,  COMES_FROM_DIAGONAL);
	mat[5][2]  = cell_new (-1, COMES_FROM_UP);
	mat[5][3]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[5][4]  = cell_new (-3, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[5][5]  = cell_new (-3, COMES_FROM_DIAGONAL);
	mat[5][6]  = cell_new (-3, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[5][7]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[5][8]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[5][9]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[5][10] = cell_new (-1, COMES_FROM_DIAGONAL);

	mat[6][1]  = cell_new (-1, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[6][2]  = cell_new (2,  COMES_FROM_DIAGONAL);
	mat[6][3]  = cell_new (0,  COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[6][4]  = cell_new (-2, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[6][5]  = cell_new (-2, COMES_FROM_DIAGONAL);
	mat[6][6]  = cell_new (-4, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[6][7]  = cell_new (-2, COMES_FROM_DIAGONAL);
	mat[6][8]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[6][9]  = cell_new (-1, COMES_FROM_LEFT);
	mat[6][10] = cell_new (-2, COMES_FROM_DIAGONAL);

	mat[7][1]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[7][2]  = cell_new (0,  COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[7][3]  = cell_new (3,  COMES_FROM_DIAGONAL);
	mat[7][4]  = cell_new (1,  COMES_FROM_LEFT);
	mat[7][5]  = cell_new (-1, COMES_FROM_LEFT | COMES_FROM_DIAGONAL);
	mat[7][6]  = cell_new (-3, COMES_FROM_LEFT | COMES_FROM_DIAGONAL);
	mat[7][7]  = cell_new (-4, COMES_FROM_UP);
	mat[7][8]  = cell_new (-1, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[7][9]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[7][10] = cell_new (-2, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	
	mat[8][1]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[8][2]  = cell_new (-2, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[8][3]  = cell_new (1,  COMES_FROM_UP);
	mat[8][4]  = cell_new (2,  COMES_FROM_DIAGONAL);
	mat[8][5]  = cell_new (0,  COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[8][6]  = cell_new (-2, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[8][7]  = cell_new (-4, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[8][8]  = cell_new (-3, COMES_FROM_UP);
	mat[8][9]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[8][10] = cell_new (1,  COMES_FROM_DIAGONAL);
	
	mat[9][1]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[9][2]  = cell_new (0,  COMES_FROM_DIAGONAL);
	mat[9][3]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[9][4]  = cell_new (0,  COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[9][5]  = cell_new (3,  COMES_FROM_DIAGONAL);
	mat[9][6]  = cell_new (1,  COMES_FROM_LEFT);
	mat[9][7]  = cell_new (-1, COMES_FROM_LEFT);
	mat[9][8]  = cell_new (-3, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[9][9]  = cell_new (-2, COMES_FROM_UP);
	mat[9][10] = cell_new (-1, COMES_FROM_UP | COMES_FROM_DIAGONAL);

	mat[10][1]  = cell_new (1,  COMES_FROM_DIAGONAL);
	mat[10][2]  = cell_new (-1, COMES_FROM_LEFT);
	mat[10][3]  = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[10][4]  = cell_new (-2, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[10][5]  = cell_new (1,  COMES_FROM_UP);
	mat[10][6]  = cell_new (2,  COMES_FROM_DIAGONAL);
	mat[10][7]  = cell_new (2,  COMES_FROM_DIAGONAL);
	mat[10][8]  = cell_new (0,  COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[10][9]  = cell_new (-2, COMES_FROM_LEFT);
	mat[10][10] = cell_new (-3, COMES_FROM_UP | COMES_FROM_DIAGONAL);

	printf("\n");
	for(i = 0; i < n+1; i++){
		for(j = 0; j < m+1; j++){
			printf("%d\t", mat[i][j]->value_a);
		}
		printf("\n");
	}
	printf("\n");
	
	
	char** sequences = NULL; //afterMatrixFilling_findSemiGlobalAlignment (mat, up, left, n, m, 0, 1);

	printf("Text1: %s\n", sequences[0]);
	printf("Text2: %s\n", sequences[1]);

	g_free(sequences[0]);
	g_free(sequences[1]);
	g_free(sequences);
	
	for(i = 0; i < n+1; i++){
		for(j = 0; j < m+1; j++){
			g_free(mat[i][j]);
		}
	}

	for(i = 0; i < n+1; i++){
		g_free(mat[i]);
	}

	g_free(mat);
}


void testLocalAlingment(){
	gchar * up =   "ATTGTGATCC";
	gchar * left = "GTACATTCTA";
	gint i , j /*,gapPenalty = -2*/;
	gint n = strlen(up), m = strlen(left);

	Cell ***  mat = (Cell***) (g_malloc((n+1)* sizeof(Cell*)));
	for(i = 0; i < n+1; i++){
		mat[i] = (Cell**) (g_malloc((m+1)* sizeof(Cell*)));
	}       

	mat[0][0] = cell_new (0,0);
	//first row and col
	for(i = 1; i < n+1; i++){
		mat[0][i] = cell_new (0, 0);
		mat[i][0] = cell_new (0, 0);
	}


	mat[1][1]  = cell_new (0, 0);
	mat[1][2]  = cell_new (0, 0);
	mat[1][3]  = cell_new (0, 0);
	mat[1][4]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[1][5]  = cell_new (0, 0);
	mat[1][6]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[1][7]  = cell_new (0, 0);
	mat[1][8]  = cell_new (0, 0);
	mat[1][9]  = cell_new (0, 0);
	mat[1][10] = cell_new (0, 0);

	mat[2][1]  = cell_new (0, 0);
	mat[2][2]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[2][3]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[2][4]  = cell_new (0, 0);
	mat[2][5]  = cell_new (2, COMES_FROM_DIAGONAL);
	mat[2][6]  = cell_new (0, 0);
	mat[2][7]  = cell_new (0, 0);
	mat[2][8]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[2][9]  = cell_new (0, 0);
	mat[2][10] = cell_new (0, 0);
	
	mat[3][1]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[3][2]  = cell_new (0, 0);
	mat[3][3]  = cell_new (0, 0);
	mat[3][4]  = cell_new (0, 0);
	mat[3][5]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[3][6]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[3][7]  = cell_new (0, 0);
	mat[3][8]  = cell_new (0, 0);
	mat[3][9]  = cell_new (0, 0);
	mat[3][10] = cell_new (0, 0);
	
	mat[4][1]  = cell_new (0, 0);
	mat[4][2]  = cell_new (0, 0);
	mat[4][3]  = cell_new (0, 0);
	mat[4][4]  = cell_new (0, 0);
	mat[4][5]  = cell_new (0, 0);
	mat[4][6]  = cell_new (0, 0);
	mat[4][7]  = cell_new (0, 0);
	mat[4][8]  = cell_new (0, 0);
	mat[4][9]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[4][10] = cell_new (1, COMES_FROM_DIAGONAL);

	mat[5][1]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[5][2]  = cell_new (0, 0);
	mat[5][3]  = cell_new (0, 0);
	mat[5][4]  = cell_new (0, 0);
	mat[5][5]  = cell_new (0, 0);
	mat[5][6]  = cell_new (0, 0);
	mat[5][7]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[5][8]  = cell_new (0, 0);
	mat[5][9]  = cell_new (0, 0);
	mat[5][10] = cell_new (0, 0);

	mat[6][1]  = cell_new (0, 0);
	mat[6][2]  = cell_new (2, COMES_FROM_DIAGONAL);
	mat[6][3]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[6][4]  = cell_new (0, 0);
	mat[6][5]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[6][6]  = cell_new (0, 0);
	mat[6][7]  = cell_new (0, 0);
	mat[6][8]  = cell_new (2, COMES_FROM_DIAGONAL);
	mat[6][9]  = cell_new (0, 0);
	mat[6][10] = cell_new (0, 0);

	mat[7][1]  = cell_new (0, 0);
	mat[7][2]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[7][3]  = cell_new (3, COMES_FROM_DIAGONAL);
	mat[7][4]  = cell_new (1, COMES_FROM_LEFT);
	mat[7][5]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[7][6]  = cell_new (0, 0);
	mat[7][7]  = cell_new (0, 0);
	mat[7][8]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[7][9]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[7][10] = cell_new (0, 0);
	
	mat[8][1]  = cell_new (0, 0);
	mat[8][2]  = cell_new (0, 0);
	mat[8][3]  = cell_new (1, COMES_FROM_UP);
	mat[8][4]  = cell_new (2, COMES_FROM_DIAGONAL);
	mat[8][5]  = cell_new (0, 0);
	mat[8][6]  = cell_new (0, 0);
	mat[8][7]  = cell_new (0, 0);
	mat[8][8]  = cell_new (0, 0);
	mat[8][9]  = cell_new (2, COMES_FROM_DIAGONAL);
	mat[8][10] = cell_new (2, COMES_FROM_DIAGONAL);
	
	mat[9][1]  = cell_new (0, 0);
	mat[9][2]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[9][3]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[9][4]  = cell_new (0, 0);
	mat[9][5]  = cell_new (3, COMES_FROM_DIAGONAL);
	mat[9][6]  = cell_new (1, COMES_FROM_LEFT);
	mat[9][7]  = cell_new (0, 0);
	mat[9][8]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[9][9]  = cell_new (0, 0);
	mat[9][10] = cell_new (1, COMES_FROM_DIAGONAL);

	mat[10][1]  = cell_new (1, COMES_FROM_DIAGONAL);
	mat[10][2]  = cell_new (0, 0);
	mat[10][3]  = cell_new (0, 0);
	mat[10][4]  = cell_new (0, 0);
	mat[10][5]  = cell_new (1, COMES_FROM_UP);
	mat[10][6]  = cell_new (2, COMES_FROM_DIAGONAL);
	mat[10][7]  = cell_new (2, COMES_FROM_DIAGONAL);
	mat[10][8]  = cell_new (0, 0);
	mat[10][9]  = cell_new (0, 0);
	mat[10][10] = cell_new (0, 0);

	printf("\n");
	for(i = 0; i < n+1; i++){
		for(j = 0; j < m+1; j++){
			printf("%d\t", mat[i][j]->value_a);
		}
		printf("\n");
	}
	printf("\n");
	
	
	GSList* islandList = afterMatrixFilling_findLocalAlignments (mat, up, left, n, m, 2);
	GSList* iterator;
	
	for(iterator = islandList; iterator; iterator = iterator->next){
		island_print (iterator->data);
	}
	g_slist_free(islandList);
	
	for(i = 0; i < n+1; i++){
		for(j = 0; j < m+1; j++){
			g_free(mat[i][j]);
		}
	}

	for(i = 0; i < n+1; i++){
		g_free(mat[i]);
	}

	g_free(mat);
}

// Test functions for filling matrix

static void print(Cell*** matrix, gint m, gint n, gboolean printOthers)
{
	gint i, j = 0;
	if (!printOthers) {
		for (i = 0; i < m; i++) 
		{
			printf("Row %d:\n", i);
			for (j = 0; j < n; j++) {
				if (matrix[i][j] != NULL && matrix[i][j]->value_a > MIN_VALUE + 100) {
					printf("[%d][%d] = %d   ", i, j, matrix[i][j]->value_a);
					if (cell_isFlagASet (matrix[i][j], COMES_FROM_DIAGONAL)) printf("D "); else printf("  ");
					if (cell_isFlagASet (matrix[i][j], COMES_FROM_UP)) printf("U "); else printf("  ");
					if (cell_isFlagASet (matrix[i][j], COMES_FROM_LEFT)) printf("L "); else printf("  ");
				}
				else
					printf("[%d][%d] = -Inf   ", i, j);
				puts("");
			}
		}
	} else {
		puts("First table");

		for (i = 0; i < m; i++) 
		{
			printf("Row %d:\n", i);
			for (j = 0; j < n; j++) {
				if (matrix[i][j] != NULL && matrix[i][j]->value_a > MIN_VALUE + 100)
					printf("[%d][%d] = %d   ", i, j, matrix[i][j]->value_a);
				else
					printf("[%d][%d] = -Inf   ", i, j);
				if (cell_isFlagASet (matrix[i][j], COMES_FROM_DIAGONAL)) printf("A "); else printf("  ");
				if (cell_isFlagBSet (matrix[i][j], COMES_FROM_DIAGONAL)) printf("V "); else printf("  ");
				if (cell_isFlagCSet (matrix[i][j], COMES_FROM_DIAGONAL)) printf("R "); else printf("  ");
				puts("");
			}
		}

		puts("Second table");

		for (i = 0; i < m; i++) 
		{
			printf("Row %d:\n", i);
			for (j = 0; j < n; j++) {
				if (matrix[i][j]->value_b > MIN_VALUE + 100)
					printf("[%d][%d] = %d   ", i, j, matrix[i][j]->value_b);
				else
					printf("[%d][%d] = -Inf   ", i, j);
				if (cell_isFlagASet (matrix[i][j], COMES_FROM_LEFT)) printf("A "); else printf("  ");
				if (cell_isFlagBSet (matrix[i][j], COMES_FROM_LEFT)) printf("V "); else printf("  ");
				if (cell_isFlagCSet (matrix[i][j], COMES_FROM_LEFT)) printf("R "); else printf("  ");
				puts("");
			}
		}

		puts("Third table");

		for (i = 0; i < m; i++) 
		{
			printf("Row %d:\n", i);
			for (j = 0; j < n; j++) {
				if (matrix[i][j]->value_c > MIN_VALUE + 100)
					printf("[%d][%d] = %d   ", i, j, matrix[i][j]->value_c);
				else
					printf("[%d][%d] = -Inf   ", i, j);
				if (cell_isFlagASet (matrix[i][j], COMES_FROM_UP)) printf("A "); else printf("  ");
				if (cell_isFlagBSet (matrix[i][j], COMES_FROM_UP)) printf("V "); else printf("  ");
				if (cell_isFlagCSet (matrix[i][j], COMES_FROM_UP)) printf("R "); else printf("  ");
				puts("");
			}
		}
	}
}

void testFillingMatrix() 
{
	gchar* seq1 = "ACTGACGTCAG";
	gchar* seq2 = "CAGTTTTCTCA";
	gint size1 = strlen(seq1)+1;
	gint size2 = strlen(seq2)+1;
	gint i, j = 0;
	// Initialization

	ScoringOptions* options = g_malloc(sizeof(ScoringOptions));
	options->matchBonus = 1;
	options->missmatchPenalty = -1;
	options->gapOpeningPenalty = 0;
	options->gapExtensionPenalty = -2;
	options->freeLeftGapsForX = FALSE;
	options->freeLeftGapsForY = FALSE;
	options->substitutionMatrix = NULL;

	KBandOptions* kbandOptions = g_malloc(sizeof(KBandOptions));
	kbandOptions->kInitValue = 3;
	kbandOptions->kExtensionValue = 1;

	// Call and printing
	Cell*** matrix = create_similarity_matrix_full (seq1, seq2, size1-1, size2-1, options, FALSE, 1);
	//Cell*** matrix = create_similarity_matrix_kband (seq1, seq2, 18, 11, options, kbandOptions, 1);
	print (matrix, size1, size2, FALSE);
	
	// Cleaning
	for (i = 0; i < size1; i++) {
		for (j = 0; j < size2; j++) {
			if (matrix[i][j] != NULL)
				g_free(matrix[i][j]);
		}
		g_free(matrix[i]);
	}
	g_free(matrix);
	g_free(options);
	g_free(kbandOptions);
}
