#include <Island.h>

Island* island_new(gint startRow, gint startCol, gint maxValue, gint length, gchar* islandPath,
                   gchar* upSequence, gchar* leftSequence){

	Island * island = (Island*) (g_malloc(sizeof(Island)));
	island->length = length;
	island->startCol = startCol;
	island->startRow = startRow;
	island->maxValue = maxValue;
	island->islandPath = islandPath;
	
	gint i = 0, row = startRow, col = startCol, upIndex = length-1, leftIndex = length-1;
	island->upSequence = (gchar*) (g_malloc((length) * sizeof(gchar))); 
	island->leftSequence = (gchar*) (g_malloc((length) * sizeof(gchar)));
	
	for(; i < length; i++){
		switch(islandPath[i]){
			case 'D':
				island->upSequence[upIndex--] = upSequence[--col];
				island->leftSequence[leftIndex--] = leftSequence[--row];
				break;
			case 'L':
				island->upSequence[upIndex--] = upSequence[--col];
				island->leftSequence[leftIndex--] = GAP;
				break;
			case 'U': 
				island->upSequence[upIndex--] = GAP;
				island->leftSequence[leftIndex--] = leftSequence[--row];
				break;
		}
	}
	island->upSequence[length+1] = '\0';
	island->leftSequence[length+1] = '\0';

	return island;	
}

void island_print(Island* this){
	printf("---------------Island-------------\n");
	printf("Starts at: %d, %d, with value: %d\n", this->startRow, this->startCol, this->maxValue);
	printf("Length: %d, path: %s\n", this->length, this->islandPath);
	printf("%s\n", this->upSequence);
	printf("%s\n\n", this->leftSequence);
}


void island_free(Island* this){
	g_free(this->upSequence);
	g_free(this->leftSequence);
	g_free(this->islandPath);
	g_slist_free (this->points);
}

void island_destroyer(gpointer data) 
{
	Island* island_data = (Island*) data;
	island_free (island_data);
}






