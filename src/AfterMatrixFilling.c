#include "AfterMatrixFilling.h"

static gchar ** alignmentFromPoint(Cell*** matrix, gchar* upSequence, gchar* leftSequence,
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
	
	return alignmentFromPoint(matrix, upSequence, leftSequence, n, m, n, m);	
}

gchar** afterMatrixFilling_findSemiGlobalAlignment(Cell*** matrix,
                                                  gchar* upSequence,
                                                  gchar* leftSequence,
                                                  gint n,
                                                  gint m,
                                                  gboolean freeRightGapsUp,
                                                  gboolean freeRightGapsLeft){

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

	return alignmentFromPoint(matrix, upSequence, leftSequence, n, m, i, j);
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////LOCAL ALIGNMENT///////////////////////////////
////////////////////////////////////////////////////////////////////////////////


static Island* findAndMarkFromPoint(gint startRow, gint startCol, gint n,
                                        gint m,Cell*** matrix, gchar* islandPath,
                                 gchar* upSequence, gchar* leftSequence){
	gint i, j, islandIndex = 0, startValue = matrix[startRow][startCol]->value;
	Island* island = NULL;

	//i > 0 n && j > 0 because first row and col are always 0 (zero)
	for(i = startRow, j = startCol; i > 0 && j > 0;){
		if(cell_isFlagSet (matrix[i][j], IS_PROCESSED)){
			
		}
		cell_setFlag (matrix[i][j], IS_PROCESSED);

		if(matrix[i][j]->value == 0)
			break;
		
		//if finds greater value than the one we started with, then the island starts from there
		if(matrix[i][j]->value > startValue){
			islandIndex = 0;
			startRow = i;
			startCol = j;
			startValue = matrix[i][j]->value;
		}
		
		if(cell_isFlagSet(matrix[i][j], COMES_FROM_DIAGONAL) &&
		   !cell_isFlagSet(matrix[i-1][j-1], IS_PROCESSED)){
			islandPath[islandIndex++] = 'D';
			i--; j--;
			continue;
		}
		if(cell_isFlagSet(matrix[i][j], COMES_FROM_LEFT) &&
		   !cell_isFlagSet (matrix[i][j-1], IS_PROCESSED)){
			islandPath[islandIndex++] = 'L';
			j--;
			continue;
		}
		if(cell_isFlagSet(matrix[i][j], COMES_FROM_UP) &&
		   !cell_isFlagSet (matrix[i-1][j], IS_PROCESSED)){
			islandPath[islandIndex++] = 'U';
			i--;
			continue;
		}
		//if got here means, it comes from only already processed cells
		//or it does not comes from anywhere (error), anyway return NULL
		return NULL;
	}

	island = island_new(startRow, startCol, startValue, islandIndex,
	                            islandPath, upSequence, leftSequence);
	return island;
}




GSList* afterMatrixFilling_findLocalAlignments(Cell*** matrix, 
                                              gchar* upSequence,
                                              gchar* leftSequence,
                                              gint n,
                                              gint m,
                                              gint minScore){
	gchar* islandPath  = (gchar*)  (g_malloc((n + m) * sizeof(Cell*)));
	gint i, j;
	GSList *list = NULL;
	
	for(i = n; i > 0; i--){
		for(j = m; j > 0; j--){
			if(matrix[i][j]->value >= minScore && 
			   !cell_isFlagSet (matrix[i][j], IS_PROCESSED)){
				Island* newIsland = findAndMarkFromPoint(i,j,n,m,matrix,
				                                                 islandPath, 
				                                                 upSequence, 
				                                                 leftSequence);
				if(newIsland != NULL)
					list = g_slist_append(list, newIsland);
				
			}
		}
	}

	return list;
}

