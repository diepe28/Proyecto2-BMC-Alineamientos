#include "AfterMatrixFilling.h"

static gint getMaxValue(Cell*** matrix, gboolean blockOfGaps, gint i, gint j, gchar* currentMatrix){
	gint valueA = matrix[i][j]->value_a;
	if(blockOfGaps){	
		gint valueB = matrix[i][j]->value_b;
		gint valueC = matrix[i][j]->value_c;
		if(valueC >= valueB && valueC >= valueA){
			*currentMatrix = 'C';
			return valueC;
		}
		if(valueB >= valueA && valueB >= valueC){
			*currentMatrix = 'B';
			return valueB;
		}
	}
	*currentMatrix = 'A';
	return valueA;

}

static Island* alignmentFromPoint(Cell*** matrix, gchar* upSequence, gchar* leftSequence,
                                   gint rows, gint cols, gint startRow, gint startCol,
                                   gboolean blockOfGaps, char currentMatrix){

	guint i, j, upIndex = rows + cols-1, leftIndex = rows + cols-1, 
		alignmentLength = 0, startIndex = 0;
	Island * island = (Island*) (g_malloc(sizeof(Island)));
	gchar * newUpSeq = (gchar*) (g_malloc((rows + cols) * sizeof(gchar))); 
	gchar * newLeftSeq = (gchar*) (g_malloc((rows + cols) * sizeof(gchar)));
	island->points = NULL; island->islandPath = NULL;
	island->startCol = startCol;
	island->startRow = startRow;
	island->maxValue = getMaxValue(matrix, blockOfGaps, startRow, startCol, &currentMatrix);
	
	i = rows;
	j = cols;

	//Inserting cols - startCol rigth gaps in upSequence
	while(startCol++ < cols){
		island->points = g_slist_append(island->points, j);
		island->points = g_slist_append(island->points, i);
		alignmentLength++;
		newLeftSeq[leftIndex--] = GAP;
		newUpSeq[upIndex--] = upSequence[--j];
	}

	//Inserting rows - startRow rigth gaps in left 
	while(startRow++ < rows){
		island->points = g_slist_append(island->points, j);
		island->points = g_slist_append(island->points, i);
		alignmentLength++;
		newUpSeq[upIndex--] = GAP;
		newLeftSeq[leftIndex--] = leftSequence[--i];		
	}

	if(!blockOfGaps){
		//While not int matrix[0][0]
		while(i > 0 || j > 0){
			alignmentLength++;
			island->points = g_slist_append(island->points, j);
			island->points = g_slist_append(island->points, i);
			cell_setFlagA (matrix[i][j], IS_PAINTED);
			if(cell_isFlagASet (matrix[i][j], COMES_FROM_DIAGONAL)){
				newUpSeq[upIndex--] = upSequence[--j];
				newLeftSeq[leftIndex--] = leftSequence[--i];
				continue;
			}
			if(cell_isFlagASet (matrix[i][j], COMES_FROM_LEFT)){
				newUpSeq[upIndex--] = upSequence[--j];
				newLeftSeq[leftIndex--] = GAP;
				continue;
			}
			if(cell_isFlagASet (matrix[i][j], COMES_FROM_UP)){
				newUpSeq[upIndex--] = GAP;
				newLeftSeq[leftIndex--] = leftSequence[--i];
			}
		}
	}else{//block of gaps
		island->islandPath = (g_malloc((rows + cols+1) * sizeof(gchar)));
		while(i > 0 || j > 0){
			alignmentLength++;
			island->points = g_slist_append(island->points, j);
			island->points = g_slist_append(island->points, i);
			island->islandPath[startIndex++] = currentMatrix;
			
			cell_setFlag (matrix[i][j], IS_PAINTED, currentMatrix);
			if(currentMatrix == 'A'){
				if(!cell_isFlagASet (matrix[i][j], COMES_FROM_DIAGONAL)){
						currentMatrix = cell_isFlagBSet (matrix[i][j], COMES_FROM_DIAGONAL)?
						'B' : 'C';
				}
				newUpSeq[upIndex--] = upSequence[--j];
				newLeftSeq[leftIndex--] = leftSequence[--i];
				continue;
			}
			if(currentMatrix == 'B'){
				if(!cell_isFlagBSet (matrix[i][j], COMES_FROM_LEFT)){
						currentMatrix = cell_isFlagASet (matrix[i][j], COMES_FROM_LEFT)?
						'A' : 'C';
				}
				newUpSeq[upIndex--] = upSequence[--j];
				newLeftSeq[leftIndex--] = GAP;
				continue;
			}
			if(currentMatrix == 'C'){
				if(!cell_isFlagCSet (matrix[i][j], COMES_FROM_UP)){
						currentMatrix = cell_isFlagASet (matrix[i][j], COMES_FROM_UP)?
						'A' : 'B';
				}
				newUpSeq[upIndex--] = GAP;
				newLeftSeq[leftIndex--] = leftSequence[--i];
			}
		}
	}

	//always add 0,0 at the end
	island->points = g_slist_append(island->points, 0);
	island->points = g_slist_append(island->points, 0);
	
	island->length = alignmentLength+1;
	startIndex = (rows+cols) - alignmentLength;
	for(i = 0; i < alignmentLength; i++){
		newUpSeq[i] = newUpSeq[startIndex + i];
		newLeftSeq[i] = newLeftSeq[startIndex + i];
	}
	newUpSeq[alignmentLength] = '\0';
	newLeftSeq[alignmentLength] = '\0';
	if(island->islandPath != NULL){
		island->islandPath[alignmentLength++] = 'A';
		island->islandPath[alignmentLength] = '\0';
	}

	island->upSequence = newUpSeq;
	island->leftSequence = newLeftSeq;

	return island;
}

Island* afterMatrixFilling_find_NW_Alignment(Cell*** matrix,
                                                  gchar* upSequence,
                                                  gchar* leftSequence,
                                                  gint rows,
                                                  gint cols,
                                                  gboolean freeRightGapsUp,
                                                  gboolean freeRightGapsLeft,
                                                  gboolean blockOfGaps){

	guint i, j, maxInLastRowIndex = rows, maxInLastColIndex = cols;
	gint maxInLastRow, maxInLastCol, currentMax;
	gchar currentMatrix = 'A';

	//yes, row with cols and col with rows
	maxInLastRowIndex = cols;
	maxInLastColIndex = rows;

	currentMax = getMaxValue (matrix, blockOfGaps, rows,cols, &currentMatrix);
	
	if(freeRightGapsLeft){
		//find the max in the last row of all 3 matrixes or just in A		
		maxInLastRow = getMaxValue(matrix, blockOfGaps, rows, 0, &currentMatrix);
		maxInLastRowIndex = 0;
		for(j = 1; j < cols+1; j++){
			currentMax = getMaxValue(matrix, blockOfGaps, rows, j, &currentMatrix);
			if(currentMax > maxInLastRow){
				maxInLastRow = currentMax;
				maxInLastRowIndex = j;
			}
		}
	}

	if(freeRightGapsUp){
		//find the max in the last col
		maxInLastCol = getMaxValue(matrix, blockOfGaps, 0, cols, &currentMatrix);
		maxInLastColIndex = 0;
		for(i = 1; i < rows+1; i++){
			currentMax = getMaxValue(matrix, blockOfGaps, i, cols, &currentMatrix);
			if(currentMax > maxInLastCol){
				maxInLastCol = currentMax;
				maxInLastColIndex = i;
			}
		}
	}

	//yes, i with col and j with row. 
	i = maxInLastColIndex;
	j = maxInLastRowIndex;

	if(freeRightGapsLeft && freeRightGapsUp){
		if(maxInLastCol > maxInLastRow){
			i = maxInLastColIndex;
			j = cols;
		}else{
			j = maxInLastRowIndex;
			i = rows;
		}
	}

	return alignmentFromPoint(matrix, upSequence, leftSequence, rows, cols, i, j, blockOfGaps, currentMatrix);
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////LOCAL ALIGNMENT///////////////////////////////
////////////////////////////////////////////////////////////////////////////////


static Island* findAndMarkFromPoint(gint startRow, gint startCol, gint n,
                                        gint m,Cell*** matrix,
                                 gchar* upSequence, gchar* leftSequence){
	gint i, j, islandIndex = 0, startvalue = matrix[startRow][startCol]->value_a;
	Island* island = NULL;
	gchar* islandPath = (gchar*) (g_malloc((n+m) * sizeof(gchar)));

	//i > 0 n && j > 0 because first row and col are always 0 (zero)
	for(i = startRow, j = startCol; i > 0 && j > 0;){
		cell_setFlagA (matrix[i][j], IS_PROCESSED);

		if(matrix[i][j]->value_a == 0)
			break;
		
		//if finds greater value_a than the one we started with, then the island starts from there
		if(matrix[i][j]->value_a > startvalue){
			islandIndex = 0;
			startRow = i;
			startCol = j;
			startvalue = matrix[i][j]->value_a;
		}
		
		if(cell_isFlagASet(matrix[i][j], COMES_FROM_DIAGONAL) &&
		   !cell_isFlagASet(matrix[i-1][j-1], IS_PROCESSED)){
			islandPath[islandIndex++] = 'D';
			i--; j--;
			continue;
		}
		if(cell_isFlagASet(matrix[i][j], COMES_FROM_LEFT) &&
		   !cell_isFlagASet (matrix[i][j-1], IS_PROCESSED)){
			islandPath[islandIndex++] = 'L';
			j--;
			continue;
		}
		if(cell_isFlagASet(matrix[i][j], COMES_FROM_UP) &&
		   !cell_isFlagASet (matrix[i-1][j], IS_PROCESSED)){
			islandPath[islandIndex++] = 'U';
			i--;
			continue;
		}
		//if got here means, it comes from only already processed cells
		//or it does not comes from anywhere (error), anyway return NULL
		g_free(islandPath);
		return NULL;
	}

	islandPath[islandIndex] = '\0';
	island = island_new(startRow, startCol, startvalue, islandIndex,
	                            islandPath, upSequence, leftSequence);
	return island;
}




GSList* afterMatrixFilling_findLocalAlignments(Cell*** matrix, 
                                              gchar* upSequence,
                                              gchar* leftSequence,
                                              gint rows,
                                              gint cols,
                                              gint minScore){
	gint i, j;
	GSList *list = NULL;
	
	for(i = rows; i > 0; i--){
		for(j = cols; j > 0; j--){
			if(matrix[i][j]->value_a >= minScore && 
			   !cell_isFlagASet (matrix[i][j], IS_PROCESSED)){
				Island* newIsland = findAndMarkFromPoint(i,j,rows,cols,matrix, 
				                                                 upSequence, 
				                                                 leftSequence);
				if(newIsland != NULL)
					list = g_slist_append(list, newIsland);
				
			}
		}
	}

	return list;
}

