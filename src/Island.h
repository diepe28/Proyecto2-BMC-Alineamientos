#include <Cell.h>

#ifndef ISLAND_H
#define ISLAND_H

typedef struct{
	gint length;
	guint maxValue;
	gchar* upSequence;
	gchar* leftSequence;
	gint startCol;
	gint startRow;
	gchar* islandPath; //given in D,U,L chars, Diagonal, Up, Left 
}Island;

Island* island_new(gint startRow, gint startCol, gint maxValue, gint length, gchar* islandPath,
                   gchar* upSequence, gchar* leftSequence);


void island_print(Island*);

void island_free(Island*);
#endif