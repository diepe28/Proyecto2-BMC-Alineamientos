#include "Gridview.h"
#include "TestingFunctions.h"

/* ---------------------------------------------------------------- */
void gridview_model_set_value(GtkTreeModel* model, gint row, gint column, gchar* text) {
	GtkTreeIter iter;
	
	GtkTreePath* path = gtk_tree_path_new_from_indices(row, column, -1);
	
	gtk_tree_model_get_iter(model, &iter, path);

	gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, text, -1);

	gtk_tree_path_free(path);
}
/* ---------------------------------------------------------------- */
void gridview_cell_colorize(
	GtkTreeViewColumn* column,
	GtkCellRenderer* cellrenderer,
	GtkTreeModel* model,
	GtkTreeIter* iter,
	gpointer colid
) {
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

	if (text[strlen(text)-1] == '*') {
		text = g_strjoinv("", g_strsplit(text, "*", CELL_MAX_SIZE));
		
		g_object_set(cellrenderer, "text", text, NULL);
		
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
void gridview_databind(
	GtkWidget* gridview,
	Cell*** datasource,
	gchar* col0,
	gchar* head,
	gint rowslen,
	gint colslen,
	gint zpage,
	gint xpage,
	gint ypage,
	gint pagesize
) {
	gridview_resize(gridview, pagesize - 1, pagesize - 1);
	GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(gridview));
	GtkTreeViewColumn* column;

	gint xfirst = xpage*pagesize;
	gint xlast = xfirst + pagesize - 1;
	gint yfirst = ypage*pagesize;
	gint ylast = yfirst + pagesize - 1;
	
	gint i;
	gint j = 1;
	gint ivalue;
	gchar value[CELL_MAX_SIZE];
	gchar arrow[CELL_MAX_SIZE];

	gchar bodies[3] = {'A', 'B', 'C'};
	guint comesf[3] = {COMES_FROM_DIAGONAL, COMES_FROM_LEFT, COMES_FROM_UP};

	for (i=xfirst-1; i<xlast; i++) {
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(gridview), j);
		sprintf(value, "%c",  0<=i && i<colslen ? head[i] : '\0');
		gtk_tree_view_column_set_title(column, value);

		j += 2;
	}
	
	for (i=yfirst; i<ylast+1; i++) {
		for (j=xfirst; j<xlast+2; j++) {
			sprintf(value, "%s", CELL_A_NULL);
			sprintf(arrow, "%s", CELL_ARROW_NULL);
			
			if (j == xfirst) {
				if (0 < i && i <= rowslen) {
					sprintf(value, "%c", col0[i - 1]);
				}
			} else if (i<rowslen+1 && j<colslen+2) {
				ivalue = cell_getValue(datasource[i][j - 1], bodies[zpage]);
				
				if (ivalue == -1000000) {
					sprintf(value, "%s", "-INF");
				} else {
					sprintf(value, "%d%s", ivalue, cell_isFlagSet(datasource[i][j - 1], IS_PAINTED, bodies[zpage])? "*": "");
				}

				if (cell_isFlagSet(datasource[i][j - 1], comesf[zpage], 'C')) {
					sprintf(arrow, "%dC", comesf[zpage]);
				}
				if (cell_isFlagSet(datasource[i][j - 1], comesf[zpage], 'B')) {
					sprintf(arrow, "%dB", comesf[zpage]);
				}
				if (cell_isFlagSet(datasource[i][j - 1], comesf[zpage], 'A')) {
					sprintf(arrow, "%dA", comesf[zpage]);
				}
			}

			//                                       row, col
			gridview_model_set_value(model, (i-yfirst)*2, (j-xfirst)==0? 0: (j-xfirst)*2-1, value);

			if (i > yfirst && comesf[zpage] == COMES_FROM_UP) {
				gridview_model_set_value(model, (i-yfirst)*2-1, (j-xfirst)*2-1, arrow);
			}
			if (i > yfirst && j > xfirst + 1 && comesf[zpage] == COMES_FROM_DIAGONAL) {
				gridview_model_set_value(model, (i-yfirst)*2-1, (j-xfirst)*2-2, arrow);
			}
			if (j > xfirst + 1 && comesf[zpage] == COMES_FROM_LEFT) {
				gridview_model_set_value(model, (i-yfirst)*2, (j-xfirst)*2-2, arrow);
			}
		}
	}
	
}
/* ---------------------------------------------------------------- */
void gridview_databind_plain(
	GtkWidget* gridview,
	Cell*** datasource,
	gchar* col0,
	gchar* head,
	gint rowslen,
	gint colslen,
	gint zpage,
	gint xpage,
	gint ypage,
	gint pagesize
) {
	g_critical("gridview_databind_plain");
	gridview_resize(gridview, pagesize - 1, pagesize - 1);
	GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(gridview));
	GtkTreeViewColumn* column;

	gint xfirst = xpage*pagesize;
	gint xlast = xfirst + pagesize - 1;
	gint yfirst = ypage*pagesize;
	gint ylast = yfirst + pagesize - 1;

	gint i;
	gint j = 1;
	gint ivalue;
	guint iflags = 0;
	gchar value[CELL_MAX_SIZE];
	gchar arrou[CELL_MAX_SIZE];
	gchar arron[CELL_MAX_SIZE];
	gchar arrol[CELL_MAX_SIZE];

	gchar bodies[3] = {'A', 'B', 'C'};

	for (i=xfirst-1; i<xlast; i++) {
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(gridview), j);
		sprintf(value, "%c",  0<=i && i<colslen ? head[i] : '\0');
		gtk_tree_view_column_set_title(column, value);

		j += 2;
	}

	for (i=yfirst; i<ylast+1; i++) {
		for (j=xfirst; j<xlast+2; j++) {
			sprintf(value, "%s", CELL_A_NULL);
			
			sprintf(arrou, "%s", CELL_ARROW_NULL);
			sprintf(arron, "%s", CELL_ARROW_NULL);
			sprintf(arrol, "%s", CELL_ARROW_NULL);
			
			if (j == xfirst) {
				if (0 < i && i <= rowslen) {
					sprintf(value, "%c", col0[i - 1]);
				}
			} else if (i<rowslen+1 && j<colslen+2) {
				switch (zpage) {
					case 0:
						ivalue = datasource[i][j - 1]->value_a;
						iflags = datasource[i][j - 1]->flags_a;
					break;
					case 1:
						ivalue = datasource[i][j - 1]->value_b;
						iflags = datasource[i][j - 1]->flags_b;
					break;
					case 2:
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
					sprintf(arrou, "4%c", bodies[zpage]);
				}
				if ((iflags & COMES_FROM_DIAGONAL) != 0) {
					sprintf(arron, "1%c", bodies[zpage]);
				}
				if ((iflags & COMES_FROM_LEFT) != 0) {
					sprintf(arrol, "2%c", bodies[zpage]);
				}
			}

			//                                       row, col
			gridview_model_set_value(model, (i-yfirst)*2, (j-xfirst)==0? 0: (j-xfirst)*2-1, value);

			if (i > yfirst) {
				gridview_model_set_value(model, (i-yfirst)*2-1, (j-xfirst)*2-1, arrou);
			}
			if (i > yfirst && j > xfirst + 1) {
				gridview_model_set_value(model, (i-yfirst)*2-1, (j-xfirst)*2-2, arron);
			}
			if (j > xfirst + 1) {
				gridview_model_set_value(model, (i-yfirst)*2, (j-xfirst)*2-2, arrol);
			}
		}
	}
	
}
/* ---------------------------------------------------------------- */
