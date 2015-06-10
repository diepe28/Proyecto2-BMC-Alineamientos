#include <stdlib.h>
#include <gtk/gtk.h>
#include "CellFillingSupport.h"

#ifndef GRIDVIEW
#define GRIDVIEW

#define CELL_MAX_SIZE 16

#define CELL_ARROW_NULL ""
#define CELL_ARROW_LEFTWARDS "\u2190"
#define CELL_ARROW_UPWARDS "\u2191"
#define CELL_ARROW_NORTHWEST "\u2196"

#define CELL_A_NULL ""
#define CELL_A_LEFTWARDS "0A"
#define CELL_A_UPWARDS "1A"
#define CELL_A_NORTHWEST "6A"

#define CELL_B_NULL ""
#define CELL_B_LEFTWARDS "0B"
#define CELL_B_UPWARDS "1B"
#define CELL_B_NORTHWEST "6B"

#define CELL_C_NULL ""
#define CELL_C_LEFTWARDS "0C"
#define CELL_C_UPWARDS "1C"
#define CELL_C_NORTHWEST "6C"

#define BODY_A 0
#define BODY_B 1
#define BODY_C 2

void gridview_model_set_value(GtkTreeModel* model, gint row, gint column, gchar* text);
void gridview_cell_colorize(GtkTreeViewColumn* column, GtkCellRenderer* cellrenderer, GtkTreeModel* model, GtkTreeIter* iter, gpointer colid);
void gridview_resize(GtkWidget* gridview, gint rowslen, gint colslen);
void gridview_init(GtkWidget* gridview);
void gridview_databind(GtkWidget* gridview, Cell*** datasource, gchar* col0, gchar* head, gint rowslen, gint colslen, gint index);

#endif
