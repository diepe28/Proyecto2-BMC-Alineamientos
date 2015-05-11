#include "Cell.h"

Cell* cell_new(int value, guint flags){
	Cell* cell = (Cell*) g_malloc(sizeof(Cell));
	cell->value = value;
	cell->flags = flags;
	return cell;
}


void cell_setFlag(Cell *this, guint option){
	this->flags |= option;
}

int cell_isFlagSet(Cell *this, guint option){
	return (this->flags & option) != 0;
}