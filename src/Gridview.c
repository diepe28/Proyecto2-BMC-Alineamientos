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
void gridview_resize(GtkWidget* gridview, gint rowslen, gint colslen) {
	gint oldcolslen = (gtk_tree_view_get_n_columns(GTK_TREE_VIEW(gridview)) - 2)/2;
	gint i;
	gchar data[CELL_MAX_SIZE];
	GType types[2 + colslen*2];
	GtkTreeViewColumn* column;
	GtkCellRenderer* cellrenderer;

	for (i=0; i<2 + colslen*2; i++) {
		types[i] = G_TYPE_STRING;
	}

	if (oldcolslen <= colslen) {
		for (i=oldcolslen*2+2; i<colslen*2+2; i++) {
			sprintf(data, "%s", "");
			/*
			if (i%2 == 0) {
				cellrenderer = gtk_cell_renderer_pixbuf_new();

				column = gtk_tree_view_column_new_with_attributes(data, cellrenderer, NULL);
			} else {
				*/
				cellrenderer = gtk_cell_renderer_text_new();
				
				column = gtk_tree_view_column_new_with_attributes(data, cellrenderer, "text", i, NULL);
			//}
			
			g_object_set_data(G_OBJECT(cellrenderer), "column", GINT_TO_POINTER(i));
			
			gtk_tree_view_append_column(GTK_TREE_VIEW(gridview), column);
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
			sprintf(data, "%s", "");
			
			g_value_init(&values[j], G_TYPE_STRING);
			g_value_set_string(&values[j], data);

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
	gchar data[CELL_MAX_SIZE];
	GtkTreeViewColumn* column;
	GtkCellRenderer* cellrenderer;
	
	for (i=0; i<2; i++) {
		cellrenderer = gtk_cell_renderer_text_new();
		g_object_set_data(G_OBJECT(cellrenderer), "column", GINT_TO_POINTER(i));

		sprintf(data, "%s", "");
		
		column = gtk_tree_view_column_new_with_attributes(data, cellrenderer, "text", i, NULL);
		
		gtk_tree_view_append_column(GTK_TREE_VIEW(gridview), column);
	}
}
/* ---------------------------------------------------------------- */
void gridview_databind(GtkWidget* gridview, Cell*** datasource, gchar* col0, gchar* head, gint rowslen, gint colslen) {
	gridview_resize(gridview, rowslen, colslen);
	GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(gridview));
	GtkTreeViewColumn* column;

	gint i = 0;
	gint j;
	gchar data[CELL_MAX_SIZE];

	for (i=0; i<colslen; i++) {
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(gridview), i*2 + 3);
		sprintf(data, "%c", head[i]);
		gtk_tree_view_column_set_title(column, data);
	}

	for (i=0; i<rowslen + 1; i++) {
		for (j=0; j<colslen + 2; j++) {
			if (j == 0) {
				if (i == 0) {
					sprintf(data, "%s", "");
				} else {
					sprintf(data, "%c", col0[i - 1]);
				}
			} else {
				if (datasource[i][j - 1]->value_a == -1000000) {
					sprintf(data, "%s", "-INF");
				} else {
					sprintf(data, "%d", datasource[i][j - 1]->value_a);
				}
			}

			//                              row, col
			gridview_model_set_value(model, i*2, j == 0? 0: j*2 - 1, data);
		}
	}
	
}
/* ---------------------------------------------------------------- */
