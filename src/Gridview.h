#include <stdlib.h>
#include <gtk/gtk.h>
#include "CellFillingSupport.h"

#ifndef GRIDVIEW
#define GRIDVIEW

#define CELL_MAX_SIZE 16

#define CELL_A_ARROW_NULL ""
#define CELL_A_ARROW_LEFTWARDS "\u2190"
#define CELL_A_ARROW_UPWARDS "\u2191"
#define CELL_A_ARROW_NORTHWEST "\u2196"

#define CELL_B_ARROW_NULL ""
#define CELL_B_ARROW_LEFTWARDS "\u21d0"
#define CELL_B_ARROW_UPWARDS "\u21d1"
#define CELL_B_ARROW_NORTHWEST "\u21d6"

#define CELL_C_ARROW_NULL ""
#define CELL_C_ARROW_LEFTWARDS "\u21e0"
#define CELL_C_ARROW_UPWARDS "\u21e1"
#define CELL_C_ARROW_NORTHWEST "\u22f1"

void gridview_model_set_value(GtkTreeModel* model, gint row, gint column, gchar* text);
void gridview_resize(GtkWidget* gridview, gint rowslen, gint colslen);
void gridview_init(GtkWidget* gridview);
void gridview_databind(GtkWidget* gridview, Cell*** datasource, gchar* col0, gchar* head, gint rowslen, gint colslen);

#endif
