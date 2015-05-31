#include <gtk/gtk.h>
#include "proyecto2-bmc-alineamientos.h"
#include "CellFillingSupport.h"

#ifndef HANDLERS_H
#define HANDLERS_H

void app_set_builder(GtkBuilder* value);
GObject* app_builder_get_cbKBand();
GObject* app_builder_get_sbGrowthInterval();
GObject* app_builder_get_sbKValue();
GObject* app_builder_get_txV();
GObject* app_builder_get_txW();
GObject* app_builder_get_rbCustomValues();
GObject* app_builder_get_sbMatch();
GObject* app_builder_get_sbMissmatch();
GObject* app_builder_get_sbF();
GObject* app_builder_get_sbK();
GObject* app_builder_get_cbVLeftFG();
GObject* app_builder_get_cbWLeftFG();
GObject* app_builder_get_sbNThreads();
GObject* app_builder_get_cbSubstitutionMatrix();
void app_widget_show_popup(Cell*** datasource);
gint app_entry_set_source(GtkEntry* entry, gchar* source);

#endif
