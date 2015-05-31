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
	
#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	//testFillingMatrix ();
	testGlobalAlingment();
	//testSemiGlobalAlingment();
	//testLocalAlingment();
	//printf("%d\n", valueOfMatrix (BLOSUM_62, 'D', 'V'));
	//testGNUPLOT();
	long times[] = {
		4800,4900,4400,4300,4250,4200,4000,4010,4020,4050,
		4800,4900,4400,4300,4250,4200,4000,4010,4020,4050,
		4800,4900,4400,4300,4250,4200,4000,4010,4020,4050,
		4800,4900,4400,4300,4250,4200,4000,4010,4020,4050,
		4800,4900,4400,4300,4250,4200,4000,4010,4020,4050
	};
	long kband[] = {
		3800,3900,3400,3300,3250,3200,3000,3050,3080, 3100,
		3800,3900,3400,3300,3250,3200,3000,3050,3080, 3100,
		3800,3900,3400,3300,3250,3200,3000,3050,3080, 3100,
		3800,3900,3400,3300,3250,3200,3000,3050,3080, 3100,
		3800,3900,3400,3300,3250,3200,3000,3050,3080, 3100
	};
	int n = 50;
	
	//createBenchmarkGraphKBand(&times, &kband, n);
	
	app = proyecto2_bmc_alineamientos_new ();
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}

