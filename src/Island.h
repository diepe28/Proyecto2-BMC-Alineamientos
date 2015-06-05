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
	GSList* points; // and int list of x1,y1,x2,y2,...,xn,yn
}Island;

Island* island_new(gint startRow, gint startCol, gint maxValue, gint length, gchar* islandPath,
                   gchar* upSequence, gchar* leftSequence);


void island_print(Island*);

void island_free(Island*);
void island_destroyer(gpointer data);

#endif