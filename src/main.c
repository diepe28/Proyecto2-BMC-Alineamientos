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
#include "TestingFunctions.h"

int
main (int argc, char *argv[])
{
	Proyecto2bmcalineamientos *app;
	int status;
	/*gchar* dna = "ACUGAAAAGGGUUUCGAACUGGUAUGGUAG";
	gchar* protein = dnaToProtein (dna);
	printf("PROTEIN: %s\n", protein);

	printf("value of: %d\n",valueOfMatrix(BLOSUM_45, '*', '*'));*/
	//testBenchmark ();
	
#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif
	app = proyecto2_bmc_alineamientos_new ();
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}

