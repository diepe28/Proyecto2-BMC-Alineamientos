#include <stdlib.h>
#include <gtk/gtk.h>
#include "CellFillingSupport.h"

#ifndef GRIDVIEW
#define GRIDVIEW

#define CELL_MAX_SIZE 16

void gridview_model_set_value(GtkTreeModel* model, gint row, gint column, gchar* text);
void gridview_resize(GtkWidget* gridview, gint rowslen, gint colslen);
void gridview_init(GtkWidget* gridview);
void gridview_databind(GtkWidget* gridview, Cell*** datasource, gchar* col0, gchar* head, gint rowslen, gint colslen);

#endif
