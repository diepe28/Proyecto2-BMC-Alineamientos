#include "Cell.h"

const gchar GAP = '_';
Cell* cell_new(gint value, guint flags){
	Cell* cell = (Cell*) g_malloc(sizeof(Cell));
	cell->value = value;
	cell->flags = flags;

	cell->value_b = 0;
	cell->value_c = 0;
	cell->flags_b = NONE;
	cell->flags_c = NONE;
	return cell;
}

Cell* triple_cell_new(gint value_a, gint value_b, gint value_c, guint flags_a, guint flags_b, guint flags_c) {
	Cell* cell = (Cell*) g_malloc(sizeof(Cell));
	cell->value = value_a;
	cell->value_b = value_b;
	cell->value_c = value_c;
	cell->flags = flags_a;
	cell->flags_b = flags_b;
	cell->flags_c = flags_c;
	return cell;
}

void cell_setFlag(Cell *this, guint option){
	this->flags |= option;
}

void cell_setFlagB(Cell *this, guint option) {
	this->flags_b |= option;
}

void cell_setFlagC(Cell *this, guint option) {
	this->flags_c |= option;
}

gint cell_isFlagSet(Cell *this, guint option){
	return (this->flags & option) != 0;
}

gint cell_isFlagBSet(Cell *this, guint option) {
	return (this->flags_b & option) != 0;
}

gint cell_isFlagCSet(Cell *this, guint option) {
	return (this->flags_c & option) != 0;
}
