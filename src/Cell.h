#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#define COMES_FROM_DIAGONAL 1
#define COMES_FROM_LEFT 2
#define COMES_FROM_UP 4
#define IS_PAINTED 8

#ifndef CELL_H
#define CELL_H

typedef struct{
	int value;
	int flags;
}Cell;

Cell* cell_new(int, unsigned int);

void cell_setFlag(Cell*, unsigned int);
int cell_isFlagSet(Cell*, unsigned int);

#endif
