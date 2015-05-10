/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2015 Ubuntu <diego@ubuntu>
 *
 * Proyecto2-BMC-Alineamientos is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Proyecto2-BMC-Alineamientos is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <gtk/gtk.h>
#include "proyecto2-bmc-alineamientos.h"
#include <glib/gi18n.h>
#include "AfterMatrixFilling.h"

void testNW(){
	char * up = "ATTGT";
	char * left = "TTGCA";
	int i , j, gapPenalty = -2;
	int n = strlen(up), m = strlen(left);

	Cell ***  mat = (Cell***) (malloc((n+1)* sizeof(Cell*)));
	for(i = 0; i < n+1; i++){
		mat[i] = (Cell**) (malloc((m+1)* sizeof(Cell*)));
	}       

	mat[0][0] = cell_new (0,0);
	//first row
	for(i = 1; i < n+1; i++){
		mat[0][i] = cell_new (gapPenalty * i, COMES_FROM_LEFT );
	}

	//first col
	for(i = 1; i < n+1; i++){
		mat[i][0] = cell_new (gapPenalty * i, COMES_FROM_UP);
	}

	mat[1][1] = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][2] = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[1][3] = cell_new (-3, COMES_FROM_DIAGONAL | COMES_FROM_LEFT);
	mat[1][4] = cell_new (-5, COMES_FROM_LEFT);
	mat[1][5] = cell_new (-7, COMES_FROM_LEFT | COMES_FROM_DIAGONAL);

	mat[2][1] = cell_new (-3, COMES_FROM_UP | COMES_FROM_DIAGONAL);
	mat[2][2] = cell_new (0, COMES_FROM_DIAGONAL);
	mat[2][3] = cell_new (0, COMES_FROM_DIAGONAL);
	mat[2][4] = cell_new (-2, COMES_FROM_LEFT);
	mat[2][5] = cell_new (-4, COMES_FROM_LEFT | COMES_FROM_DIAGONAL);
	
	mat[3][1] = cell_new (-5, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[3][2] = cell_new (-2, COMES_FROM_UP);
	mat[3][3] = cell_new (-1, COMES_FROM_DIAGONAL);
	mat[3][4] = cell_new (1, COMES_FROM_DIAGONAL);
	mat[3][5] = cell_new (-1, COMES_FROM_LEFT);
	
	mat[4][1] = cell_new (-7, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[4][2] = cell_new (-4, COMES_FROM_UP);
	mat[4][3] = cell_new (-3, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[4][4] = cell_new (-1, COMES_FROM_UP);
	mat[4][5] = cell_new (-0, COMES_FROM_DIAGONAL);

	mat[5][1] = cell_new (-7, COMES_FROM_DIAGONAL);
	mat[5][2] = cell_new (-6, COMES_FROM_UP);
	mat[5][3] = cell_new (-5, COMES_FROM_DIAGONAL | COMES_FROM_UP);
	mat[5][4] = cell_new (-3, COMES_FROM_UP);
	mat[5][5] = cell_new (-2, COMES_FROM_UP);

	char** sequences = afterMatrixFilling_findGlobalAlignment (mat, up, left, n, m);

	printf("Text1: %s\n", sequences[0]);
	printf("Text2: %s\n", sequences[1]);

	free(sequences[0]);
	free(sequences[1]);
	free(sequences);
	
	for(i = 0; i < n+1; i++){
		for(j = 0; j < m+1; j++){
			free(mat[i][j]);
		}
	}

	for(i = 0; i < n+1; i++){
		free(mat[i]);
	}

	free(mat);
}

int
main (int argc, char *argv[])
{
	Proyecto2bmcalineamientos *app;
	int status;
	
#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	testNW();
	
	app = proyecto2_bmc_alineamientos_new ();
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}

