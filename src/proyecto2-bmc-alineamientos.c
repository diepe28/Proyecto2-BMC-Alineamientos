/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * proyecto2-bmc-alineamientos.c
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
#include "proyecto2-bmc-alineamientos.h"

#include <glib/gi18n.h>



/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/proyecto2_bmc_alineamientos.ui" */
#define UI_FILE "src/proyecto2_bmc_alineamientos.ui"
#define TOP_WINDOW "window"


G_DEFINE_TYPE (Proyecto2bmcalineamientos, proyecto2_bmc_alineamientos, GTK_TYPE_APPLICATION);

/* ANJUTA: Macro PROYECTO2_BMC_ALINEAMIENTOS_APPLICATION gets Proyecto2bmcalineamientos - DO NOT REMOVE */
struct _Proyecto2bmcalineamientosPrivate
{
	/* ANJUTA: Widgets declaration for proyecto2_bmc_alineamientos.ui - DO NOT REMOVE */
};

/* Create a new window loading a file */
static void
proyecto2_bmc_alineamientos_new_window (GApplication *app,
                           GFile        *file)
{
	GtkWidget *window;

	GtkBuilder *builder;
	GError* error = NULL;

	Proyecto2bmcalineamientosPrivate *priv = PROYECTO2_BMC_ALINEAMIENTOS_APPLICATION(app)->priv;

	/* Load UI from file */
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (builder, app);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (builder, TOP_WINDOW));
        if (!window)
        {
		g_critical ("Widget \"%s\" is missing in file %s.",
				TOP_WINDOW,
				UI_FILE);
        }

	
	/* ANJUTA: Widgets initialization for proyecto2_bmc_alineamientos.ui - DO NOT REMOVE */
	
	on_window_init (builder);

	g_object_unref (builder);
	
	
	gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));
	if (file != NULL)
	{
		/* TODO: Add code here to open the file in the new window */
	}

	gtk_widget_show_all (GTK_WIDGET (window));
}


/* GApplication implementation */
static void
proyecto2_bmc_alineamientos_activate (GApplication *application)
{
	proyecto2_bmc_alineamientos_new_window (application, NULL);
}

static void
proyecto2_bmc_alineamientos_open (GApplication  *application,
                     GFile        **files,
                     gint           n_files,
                     const gchar   *hint)
{
	gint i;

	for (i = 0; i < n_files; i++)
		proyecto2_bmc_alineamientos_new_window (application, files[i]);
}

static void
proyecto2_bmc_alineamientos_init (Proyecto2bmcalineamientos *object)
{
	object->priv = G_TYPE_INSTANCE_GET_PRIVATE (object, PROYECTO2_BMC_ALINEAMIENTOS_TYPE_APPLICATION, Proyecto2bmcalineamientosPrivate);
}

static void
proyecto2_bmc_alineamientos_finalize (GObject *object)
{
	G_OBJECT_CLASS (proyecto2_bmc_alineamientos_parent_class)->finalize (object);
}

static void
proyecto2_bmc_alineamientos_class_init (Proyecto2bmcalineamientosClass *klass)
{
	G_APPLICATION_CLASS (klass)->activate = proyecto2_bmc_alineamientos_activate;
	G_APPLICATION_CLASS (klass)->open = proyecto2_bmc_alineamientos_open;

	g_type_class_add_private (klass, sizeof (Proyecto2bmcalineamientosPrivate));

	G_OBJECT_CLASS (klass)->finalize = proyecto2_bmc_alineamientos_finalize;
}

Proyecto2bmcalineamientos *
proyecto2_bmc_alineamientos_new (void)
{
	g_type_init ();

	return g_object_new (proyecto2_bmc_alineamientos_get_type (),
	                     "application-id", "org.gnome.proyecto2_bmc_alineamientos",
	                     "flags", G_APPLICATION_HANDLES_OPEN,
	                     NULL);
}

