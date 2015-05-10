#include "AfterMatrixFilling.h"

const char GAP = '_';

char** afterMatrixFilling_findGlobalAlignment(Cell*** matrix,
                                              char* upSeq, char* leftSeq,
                                              int n, int m){
	unsigned int i, j, upIndex = n + m-1, leftIndex = n + m-1, alignmentLength;
	char ** sequences = (char**) (malloc( 2 * sizeof(char*)));
	char * newUpSeq = (char*) (malloc((n + m) * sizeof(char))); 
	char * newLeftSeq = (char*) (malloc((n + m) * sizeof(char)));

	i = n;
	j = m;

	//While not int matrix[0][0]
	while(i > 0 || j > 0){
		if(cell_isFlagSet (matrix[i][j], COMES_FROM_DIAGONAL)){
			newUpSeq[upIndex--] = upSeq[--j];
			newLeftSeq[leftIndex--] = leftSeq[--i];
			//printf("UP: %c   left: %c \n\n",newUpSeq[upIndex+1], newLeftSeq[leftIndex+1] );
			continue;
		}
		if(cell_isFlagSet (matrix[i][j], COMES_FROM_LEFT)){
			newUpSeq[upIndex--] = upSeq[--j];
			newLeftSeq[leftIndex--] = GAP;
			//printf("UP: %c   left: %c \n\n",newUpSeq[upIndex+1], newLeftSeq[leftIndex+1] );
			continue;
		}
		if(cell_isFlagSet (matrix[i][j], COMES_FROM_UP)){
			newUpSeq[upIndex--] = GAP;
			newLeftSeq[leftIndex--] = leftSeq[--i];
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






