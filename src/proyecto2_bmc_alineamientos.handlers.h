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
GObject* app_builder_get_cbVRightFG();
GObject* app_builder_get_cbWRightFG();
GObject* app_builder_get_sbNThreads();
GObject* app_builder_get_cbSubstitutionMatrix();
GObject* app_builder_get_cbVInputType();
GObject* app_builder_get_cbWInputType();
GObject* app_builder_get_lAlgorithmValue();
GObject* app_builder_get_lVLengthValue();
GObject* app_builder_get_lWLengthValue();
GObject* app_builder_get_lVTypeValue();
GObject* app_builder_get_lWTypeValue();
GObject* app_builder_get_lVNew();
GObject* app_builder_get_lWNew();
void app_widget_show_nwpopup(
	gchar* seq1,
	gchar* seq2,
	gint seq1Length,
	gint seq2Length,
	gint seq1Type,
	gint seq2Type,
	ScoringOptions* scoringOptions,
  gboolean freeRightGapsUp,
  gboolean freeRightGapsLeft,
	gboolean isLocalAlignment,
	gint numberOfThreads
);
gint app_entry_set_source(GtkEntry* entry, gchar* source);

#endif
