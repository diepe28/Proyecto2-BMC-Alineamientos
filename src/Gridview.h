#include <stdlib.h>
#include <gtk/gtk.h>
#include "CellFillingSupport.h"

#ifndef GRIDVIEW
#define GRIDVIEW

#define CELL_MAX_SIZE 16

#define CELL_ARROW_NULL "\0"
#define CELL_ARROW_LEFTWARDS "\u2190"
#define CELL_ARROW_UPWARDS "\u2191"
#define CELL_ARROW_NORTHWEST "\u2196"

#define CELL_A_NULL "\0"
#define CELL_A_LEFTWARDS "2A"
#define CELL_A_UPWARDS "4A"
#define CELL_A_NORTHWEST "1A"

#define CELL_B_NULL "\0"
#define CELL_B_LEFTWARDS "2B"
#define CELL_B_UPWARDS "4B"
#define CELL_B_NORTHWEST "1B"

#define CELL_C_NULL "\0"
#define CELL_C_LEFTWARDS "2C"
#define CELL_C_UPWARDS "4C"
#define CELL_C_NORTHWEST "1C"

void gridview_model_set_value(GtkTreeModel* model, gint row, gint column, gchar* text);
void gridview_cell_colorize(
	GtkTreeViewColumn* column,
	GtkCellRenderer* cellrenderer,
	GtkTreeModel* model,
	GtkTreeIter* iter,
	gpointer colid
);
void gridview_resize(GtkWidget* gridview, gint rowslen, gint colslen);
void gridview_init(GtkWidget* gridview);
void gridview_databind(GtkWidget* gridview,
	Cell*** datasource,
	gchar* col0,
	gchar* head,
	gint rowslen,
	gint colslen,
	gint zpage,
	gint xpage,
	gint ypage,
	gint pagesize
);

#endif
