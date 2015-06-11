#include "Gridview.h"

/* ---------------------------------------------------------------- */
void gridview_model_set_value(GtkTreeModel* model, gint row, gint column, gchar* text) {
	GtkTreeIter iter;
	
	GtkTreePath* path = gtk_tree_path_new_from_indices(row, column, -1);
	
	gtk_tree_model_get_iter(model, &iter, path);

	gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, text, -1);

	gtk_tree_path_free(path);
}
/* ---------------------------------------------------------------- */
void gridview_cell_colorize(GtkTreeViewColumn* column, GtkCellRenderer* cellrenderer, GtkTreeModel* model, GtkTreeIter* iter, gpointer colid) {
	GValue* value = g_slice_alloc0(sizeof(GValue));
	gtk_tree_model_get_value(model, iter, GPOINTER_TO_INT(colid), value);
	const gchar* text = g_value_get_string(value);

	g_object_set(cellrenderer, "foreground-set", FALSE, NULL);
	g_object_set(cellrenderer, "background-set", FALSE, NULL);
	
	if (g_strcmp0(text, CELL_A_LEFTWARDS) == 0) {
		g_object_set(cellrenderer, "text", CELL_ARROW_LEFTWARDS, NULL);
		
		g_object_set(cellrenderer, "foreground-set", TRUE, NULL);
		g_object_set(cellrenderer, "foreground", "orange", NULL);
	}

	if (g_strcmp0(text, CELL_A_UPWARDS) == 0) {
		g_object_set(cellrenderer, "text", CELL_ARROW_UPWARDS, NULL);
		
		g_object_set(cellrenderer, "foreground-set", TRUE, NULL);
		g_object_set(cellrenderer, "foreground", "orange", NULL);
	}

	if (g_strcmp0(text, CELL_A_NORTHWEST) == 0) {
		g_object_set(cellrenderer, "text", CELL_ARROW_NORTHWEST, NULL);
		
		g_object_set(cellrenderer, "foreground-set", TRUE, NULL);
		g_object_set(cellrenderer, "foreground", "orange", NULL);
	}

	if (g_strcmp0(text, CELL_B_LEFTWARDS) == 0) {
		g_object_set(cellrenderer, "text", CELL_ARROW_LEFTWARDS, NULL);
		
		g_object_set(cellrenderer, "foreground-set", TRUE, NULL);
		g_object_set(cellrenderer, "foreground", "green", NULL);
	}

	if (g_strcmp0(text, CELL_B_UPWARDS) == 0) {
		g_object_set(cellrenderer, "text", CELL_ARROW_UPWARDS, NULL);
		
		g_object_set(cellrenderer, "foreground-set", TRUE, NULL);
		g_object_set(cellrenderer, "foreground", "green", NULL);
	}

	if (g_strcmp0(text, CELL_B_NORTHWEST) == 0) {
		g_object_set(cellrenderer, "text", CELL_ARROW_NORTHWEST, NULL);
		
		g_object_set(cellrenderer, "foreground-set", TRUE, NULL);
		g_object_set(cellrenderer, "foreground", "green", NULL);
	}

	if (g_strcmp0(text, CELL_C_LEFTWARDS) == 0) {
		g_object_set(cellrenderer, "text", CELL_ARROW_LEFTWARDS, NULL);
		
		g_object_set(cellrenderer, "foreground-set", TRUE, NULL);
		g_object_set(cellrenderer, "foreground", "red", NULL);
	}

	if (g_strcmp0(text, CELL_C_UPWARDS) == 0) {
		g_object_set(cellrenderer, "text", CELL_ARROW_UPWARDS, NULL);
		
		g_object_set(cellrenderer, "foreground-set", TRUE, NULL);
		g_object_set(cellrenderer, "foreground", "red", NULL);
	}

	if (g_strcmp0(text, CELL_C_NORTHWEST) == 0) {
		g_object_set(cellrenderer, "text", CELL_ARROW_NORTHWEST, NULL);
		
		g_object_set(cellrenderer, "foreground-set", TRUE, NULL);
		g_object_set(cellrenderer, "foreground", "red", NULL);
	}
	
	if (text[strlen(text) - 1] == '*') {
		gchar* newtext = g_strjoinv("", g_strsplit(text, "*", CELL_MAX_SIZE));
		
		g_object_set(cellrenderer, "text", newtext, NULL);
		
		g_object_set(cellrenderer, "background-set", TRUE, NULL);
		g_object_set(cellrenderer, "background", "lightgray", NULL);
	}
	
	g_value_unset(value);
}
/* ---------------------------------------------------------------- */
void gridview_resize(GtkWidget* gridview, gint rowslen, gint colslen) {
	gint oldcolslen = (gtk_tree_view_get_n_columns(GTK_TREE_VIEW(gridview)) - 2)/2;
	gint i;
	gchar value[CELL_MAX_SIZE];
	GType types[2 + colslen*2];
	GtkTreeViewColumn* column;
	GtkCellRenderer* cellrenderer;

	for (i=0; i<2 + colslen*2; i++) {
		types[i] = G_TYPE_STRING;
	}

	if (oldcolslen <= colslen) {
		for (i=oldcolslen*2+2; i<colslen*2+2; i++) {
			sprintf(value, "%s", "");
			
			cellrenderer = gtk_cell_renderer_text_new();
				
			column = gtk_tree_view_column_new_with_attributes(value, cellrenderer, "text", i, NULL);
			
			g_object_set_data(G_OBJECT(cellrenderer), "column", GINT_TO_POINTER(i));
			
			gtk_tree_view_append_column(GTK_TREE_VIEW(gridview), column);

			gtk_tree_view_column_set_cell_data_func(column, cellrenderer, gridview_cell_colorize, GINT_TO_POINTER(i), NULL);
		}
	} else {
		for (i=oldcolslen*2+1; i>=colslen*2+2; i--) {
			column = gtk_tree_view_get_column(GTK_TREE_VIEW(gridview), i);
			gtk_tree_view_remove_column(GTK_TREE_VIEW(gridview), column);
		}
	}
	
	gint j;
	GtkTreeIter iter;
	GtkListStore* row = gtk_list_store_newv(2 + colslen*2, types); // *
	GValue* values;
	gint* colids;
	for (i=0; i<=rowslen*2; i++) {
		values = g_slice_alloc0(sizeof(GValue)*(2 + colslen*2));
		colids = g_slice_alloc0(sizeof(gint)*(2 + colslen*2));
		
		gtk_list_store_append(row, &iter);

		for (j=0; j<2+colslen*2; j++) {
			sprintf(value, "%s", "");
			g_value_init(&values[j], G_TYPE_STRING);
			g_value_set_string(&values[j], value);

			colids[j] = j;
		}

		gtk_list_store_set_valuesv(row, &iter, colids, values, 2 + colslen*2);
		
		g_slice_free1(sizeof(GValue)*(2 + colslen*2), values);
		g_slice_free1(sizeof(gint)*(2 + colslen*2), colids);
	}

	gtk_tree_view_set_model(GTK_TREE_VIEW(gridview), GTK_TREE_MODEL(row));
}
/* ---------------------------------------------------------------- */
void gridview_init(GtkWidget* gridview) {
	gint i;
	gchar value[CELL_MAX_SIZE];
	GtkTreeViewColumn* column;
	GtkCellRenderer* cellrenderer;
	
	for (i=0; i<2; i++) {
		cellrenderer = gtk_cell_renderer_text_new();
		g_object_set_data(G_OBJECT(cellrenderer), "column", GINT_TO_POINTER(i));

		sprintf(value, "%s", "");
		
		column = gtk_tree_view_column_new_with_attributes(value, cellrenderer, "text", i, NULL);
		
		gtk_tree_view_append_column(GTK_TREE_VIEW(gridview), column);

		gtk_tree_view_column_set_cell_data_func(column, cellrenderer, gridview_cell_colorize, GINT_TO_POINTER(i), NULL);
	}
}
/* ---------------------------------------------------------------- */
void gridview_databind(GtkWidget* gridview, Cell*** datasource, gchar* col0, gchar* head, gint rowslen, gint colslen, gint index) {
	gridview_resize(gridview, rowslen, colslen);
	GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(gridview));
	GtkTreeViewColumn* column;

	gint i = 0;
	gint j;
	gint ivalue;
	guint iflags;
	gchar value[CELL_MAX_SIZE];
	gchar arrou[CELL_MAX_SIZE];
	gchar arron[CELL_MAX_SIZE];
	gchar arrol[CELL_MAX_SIZE];

	gchar bodies[3] = {'A', 'B', 'C'};

	for (i=0; i<colslen; i++) {
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(gridview), i*2 + 3);
		sprintf(value, "%c", head[i]);
		gtk_tree_view_column_set_title(column, value);
	}

	for (i=0; i<rowslen + 1; i++) {
		for (j=0; j<colslen + 2; j++) {
			sprintf(value, "%s", CELL_A_NULL);
			
			sprintf(arrou, "%s", CELL_A_NULL);
			sprintf(arron, "%s", CELL_A_NULL);
			sprintf(arrol, "%s", CELL_A_NULL);
			
			if (j == 0) {
				if (i > 0) {
					sprintf(value, "%c", col0[i - 1]);
				}
			} else {
				switch (index) {
					case BODY_A:
						ivalue = datasource[i][j - 1]->value_a;
						iflags = datasource[i][j - 1]->flags_a;
					break;
					case BODY_B:
						ivalue = datasource[i][j - 1]->value_b;
						iflags = datasource[i][j - 1]->flags_b;
					break;
					case BODY_C:
						ivalue = datasource[i][j - 1]->value_c;
						iflags = datasource[i][j - 1]->flags_c;
					break;
				}
				
				if (ivalue == -1000000) {
					sprintf(value, "%s", "-INF");
				} else {
					sprintf(value, "%d%s", ivalue, (iflags&IS_PAINTED)!=0? "*": "");
				}

				if ((iflags & COMES_FROM_UP) != 0) {
					sprintf(arrou, "1%c", bodies[index]);
				}
				if ((iflags & COMES_FROM_DIAGONAL) != 0) {
					sprintf(arron, "6%c", bodies[index]);
				}
				if ((iflags & COMES_FROM_LEFT) != 0) {
					sprintf(arrol, "0%c", bodies[index]);
				}
			}

			//                              row, col
			gridview_model_set_value(model, i*2, j==0? 0: j*2-1, value);

			if (i > 0) {
				gridview_model_set_value(model, i*2-1, j*2-1, arrou);
			}
			if (j > 1) {
				gridview_model_set_value(model, i*2-1, j*2-2, arron);
				gridview_model_set_value(model, i*2, j*2-2, arrol);
			}
		}
	}
	
}
/* ---------------------------------------------------------------- */
