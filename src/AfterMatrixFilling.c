#include "AfterMatrixFilling.h"

const gchar GAP = '_';

gchar ** private_alignmentFromPoint(Cell*** matrix, gchar* upSequence, gchar* leftSequence,
                                   gint n, gint m, gint startRow, gint startCol){

	guint i, j, upIndex = n + m-1, leftIndex = n + m-1, alignmentLength;
	gchar ** sequences = (gchar**) (g_malloc( 2 * sizeof(gchar*)));
	gchar * newUpSeq = (gchar*) (g_malloc((n + m) * sizeof(gchar))); 
	gchar * newLeftSeq = (gchar*) (g_malloc((n + m) * sizeof(gchar)));

	i = n;
	j = m;

	//Inserting m - startCol rigth gaps in left
	while(startCol++ < m){
		newUpSeq[upIndex--] = GAP;
		newLeftSeq[leftIndex--] = leftSequence[--i];
	}

	//Inserting n - startRow rigth gaps in left 
	while(startRow++ < n){
		newLeftSeq[leftIndex--] = GAP;
		newUpSeq[upIndex--] = upSequence[--j];
	}

	//While not int matrix[0][0]
	while(i > 0 || j > 0){
		cell_setFlag(matrix[i][j], IS_PAINTED);
		if(cell_isFlagSet (matrix[i][j], COMES_FROM_DIAGONAL)){
			newUpSeq[upIndex--] = upSequence[--j];
			newLeftSeq[leftIndex--] = leftSequence[--i];
			//printf("UP: %c   left: %c \n\n",newUpSeq[upIndex+1], newLeftSeq[leftIndex+1] );
			continue;
		}
		if(cell_isFlagSet (matrix[i][j], COMES_FROM_LEFT)){
			newUpSeq[upIndex--] = upSequence[--j];
			newLeftSeq[leftIndex--] = GAP;
			//printf("UP: %c   left: %c \n\n",newUpSeq[upIndex+1], newLeftSeq[leftIndex+1] );
			continue;
		}
		if(cell_isFlagSet (matrix[i][j], COMES_FROM_UP)){
			newUpSeq[upIndex--] = GAP;
			newLeftSeq[leftIndex--] = leftSequence[--i];
			//printf("UP: %c   left: %c \n\n",newUpSeq[upIndex+1], newLeftSeq[leftIndex+1] );
		}
	}

	alignmentLength = (n+m) -leftIndex;
		
	memcpy(newUpSeq, newUpSeq+upIndex+1, alignmentLength);
	memcpy(newLeftSeq, newLeftSeq+leftIndex+1, alignmentLength);
	newUpSeq[alignmentLength] = '\0';
	newLeftSeq[alignmentLength] = '\0';

	sequences[0] = newUpSeq;
	sequences[1] = newLeftSeq;

	return sequences;
}

gchar** afterMatrixFilling_findGlobalAlignment(Cell*** matrix,
                                              gchar* upSequence,
                                              gchar* leftSequence,
                                              gint n, gint m){
	
	return private_alignmentFromPoint(matrix, upSequence, leftSequence, n, m, n, m);	
}

gchar** afterMatrixFilling_findSemiGlobalAlignment(Cell*** matrix,
                                                  gchar* upSequence,
                                                  gchar* leftSequence,
                                                  gint n,
                                                  gint m,
                                                  gint freeRightGapsUp,
                                                  gint freeRightGapsLeft){

	guint i, j, maxInLastRowIndex = n, maxInLastColIndex = m;
	gint maxInLastRow, maxInLastCol;
	
	maxInLastRowIndex = n;
	maxInLastColIndex = m;
	
	if(freeRightGapsLeft){
		//find the max in the last row
		maxInLastRow = matrix[n][0]->value;
		maxInLastRowIndex = 0;
		for(i = 1; i < n+1; i++){
			if(matrix[n][i]->value > maxInLastRow){
				maxInLastRow = matrix[n][i]->value;
				maxInLastRowIndex = i;
			}
		}
	}

	if(freeRightGapsUp){
		//find the max in the last col
		maxInLastCol = matrix[0][m]->value;
		maxInLastColIndex = 0;
		for(i = 1; i < m+1; i++){
			if(matrix[i][m]->value > maxInLastCol){
				maxInLastCol = matrix[i][m]->value;
				maxInLastColIndex = i;
			}
		}
	}

	i = maxInLastRowIndex;
	j = maxInLastColIndex;
	
	if(freeRightGapsLeft && freeRightGapsUp){
		if(maxInLastCol > maxInLastRow){
			i = maxInLastColIndex;
			j = m;
		}else{
			j = maxInLastRowIndex;
			i = n;
		}
	}

	return private_alignmentFromPoint(matrix, upSequence, leftSequence, n, m, i, j);
}


void testGlobalAlingment(){
	gchar * up = "ATTGT";
	gchar * left = "TTGCA";
	gint i , j, gapPenalty = -2;
	gint n = strlen(up), m = strlen(left);

	Cell ***  mat = (Cell***) (g_malloc((n+1)* sizeof(Cell*)));
	for(i = 0; i < n+1; i++){
		mat[i] = (Cell**) (g_malloc((m+1)* sizeof(Cell*)));
	}       

	mat[0][0] = cell_new (0,0);
	//first row and first col
	for(i = 1; i < n+1; i++){
		mat[0][i] = cell_new (gapPenalty * i, COMES_FROM_LEFT );
		mat[i][0] = cell_new (gapPenalty * i, COMES_FROM_UP);
	}

	mat[1][1] = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][2] = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][3] = cell_new (-3, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[1][4] = cell_new (-5, COMES_FROM_LEFT);
	mat[1][5] = cell_new (-7, COMES_FROM_LEFT | COMES_FROM_DIAGONAL);

	mat[2][1] = cell_new (-3, COMES_FROM_UP | COMES_FROM_DIAGONAL);
	mat[2][2] = cell_new (0, COMES_FROM_DIAGONAL);
	mat[2][3] = cell_new (0, COMES_FROM_DIAGONAL);
	mat[2][4] = cell_new (-2, COMES_FROM_LEFT);
	mat[2][5] = cell_new (-4, COMES_FROM_LEFT | COMES_FROM_DIAGONAL);
	
	mat[3][1] = cell_new (-5, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[3][2] = cell_new (-2, COMES_FROM_UP);
	mat[3][3] = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[3][4] = cell_new (1, COMES_FROM_DIAGONAL);
	mat[3][5] = cell_new (-1, COMES_FROM_LEFT);
	
	mat[4][1] = cell_new (-7, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[4][2] = cell_new (-4, COMES_FROM_UP);
	mat[4][3] = cell_new (-3, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[4][4] = cell_new (-1, COMES_FROM_UP);
	mat[4][5] = cell_new (-0, COMES_FROM_DIAGONAL);

	mat[5][1] = cell_new (-7, COMES_FROM_DIAGONAL);
	mat[5][2] = cell_new (-6, COMES_FROM_UP);
	mat[5][3] = cell_new (-5, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[5][4] = cell_new (-3, COMES_FROM_UP);
	mat[5][5] = cell_new (-2, COMES_FROM_UP);

	char** sequences = afterMatrixFilling_findGlobalAlignment (mat, up, left, n, m);

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


void testSemiGlobalAlingment(){
	gchar * up =   "ATTGTGATCC";
	gchar * left = "GTACATTCTA";
	gint i , j, gapPenalty = -2;
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
			printf("%d\t", mat[i][j]->value);
		}
		printf("\n");
	}
	printf("\n");
	
	
	char** sequences = afterMatrixFilling_findSemiGlobalAlignment (mat, up, left, n, m, 0, 1);

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
