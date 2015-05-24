#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <gtk/gtk.h>

#define NONE 0
#define COMES_FROM_DIAGONAL 1
#define COMES_FROM_LEFT 2
#define COMES_FROM_UP 4
#define IS_PAINTED 8
#define IS_PROCESSED 16

#ifndef CELL_H
#define CELL_H

extern const gchar GAP;
typedef struct{
	gint value;
	gint value_b;
	gint value_c;
	guint flags;
	guint flags_b;
	guint flags_c;
}Cell;

Cell* cell_new(gint value, guint flags);
Cell* triple_cell_new(gint value_a, gint value_b, gint value_c, guint flags_a, guint flags_b, guint flags_c);

void cell_setFlag(Cell*, guint flag);
void cell_setFlagB(Cell*, guint flag);
void cell_setFlagC(Cell*, guint flag);
int cell_isFlagSet(Cell*, guint flag);
int cell_isFlagBSet(Cell*, guint flag);
int cell_isFlagCSet(Cell*, guint flag);

#endif