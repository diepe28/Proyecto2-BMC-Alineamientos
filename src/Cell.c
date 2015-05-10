#include "Cell.h"

Cell* cell_new(int value, unsigned int flags){
	Cell* cell = (Cell*) malloc(sizeof(Cell));
	cell->value = value;
	cell->flags = flags;
	return cell;
}


void cell_setFlag(Cell *this, unsigned int option){
	this->flags |= option;
}

int cell_isFlagSet(Cell *this, unsigned int option){
	return (this->flags & option) != 0;
}