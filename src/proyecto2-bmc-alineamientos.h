/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * proyecto2-bmc-alineamientos.h
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

#ifndef _PROYECTO2_BMC_ALINEAMIENTOS_
#define _PROYECTO2_BMC_ALINEAMIENTOS_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define PROYECTO2_BMC_ALINEAMIENTOS_TYPE_APPLICATION             (proyecto2_bmc_alineamientos_get_type ())
#define PROYECTO2_BMC_ALINEAMIENTOS_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), PROYECTO2_BMC_ALINEAMIENTOS_TYPE_APPLICATION, Proyecto2bmcalineamientos))
#define PROYECTO2_BMC_ALINEAMIENTOS_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), PROYECTO2_BMC_ALINEAMIENTOS_TYPE_APPLICATION, Proyecto2bmcalineamientosClass))
#define PROYECTO2_BMC_ALINEAMIENTOS_IS_APPLICATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PROYECTO2_BMC_ALINEAMIENTOS_TYPE_APPLICATION))
#define PROYECTO2_BMC_ALINEAMIENTOS_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), PROYECTO2_BMC_ALINEAMIENTOS_TYPE_APPLICATION))
#define PROYECTO2_BMC_ALINEAMIENTOS_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), PROYECTO2_BMC_ALINEAMIENTOS_TYPE_APPLICATION, Proyecto2bmcalineamientosClass))

typedef struct _Proyecto2bmcalineamientosClass Proyecto2bmcalineamientosClass;
typedef struct _Proyecto2bmcalineamientos Proyecto2bmcalineamientos;
typedef struct _Proyecto2bmcalineamientosPrivate Proyecto2bmcalineamientosPrivate;

struct _Proyecto2bmcalineamientosClass
{
	GtkApplicationClass parent_class;
};

struct _Proyecto2bmcalineamientos
{
	GtkApplication parent_instance;

	Proyecto2bmcalineamientosPrivate *priv;

};

GType proyecto2_bmc_alineamientos_get_type (void) G_GNUC_CONST;
Proyecto2bmcalineamientos *proyecto2_bmc_alineamientos_new (void);

/* Callbacks */

G_END_DECLS

#endif /* _APPLICATION_H_ */

