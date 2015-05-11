#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <gtk/gtk.h>

#define COMES_FROM_DIAGONAL 1
#define COMES_FROM_LEFT 2
#define COMES_FROM_UP 4
#define IS_PAINTED 8

#ifndef CELL_H
#define CELL_H

typedef struct{
	gint value;
	guint flags;
}Cell;

Cell* cell_new(gint value, guint flags);

void cell_setFlag(Cell*, guint flag);
int cell_isFlagSet(Cell*, guint flag);

#endif
