#include <gtk/gtk.h>
#include "proyecto2-bmc-alineamientos.h"
#include "CellFillingSupport.h"

#ifndef HANDLERS_H
#define HANDLERS_H

void app_set_builder(GtkBuilder* value);
GObject* app_builder_get_window();
GObject* app_builder_get_cbKBand();
GObject* app_builder_get_sbGrowthInterval();
GObject* app_builder_get_sbKValue();
GObject* app_builder_get_txV();
GObject* app_builder_get_txW();
GObject* app_builder_get_rbCustomValues();
GObject* app_builder_get_rbSubstitutionMatrix();
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
GObject* app_builder_get_lAlgTypeValue();
GObject* app_builder_get_lVLengthValue();
GObject* app_builder_get_lWLengthValue();
GObject* app_builder_get_lVTypeValue();
GObject* app_builder_get_lWTypeValue();
GObject* app_builder_get_gridview();
GObject* app_builder_get_cbGotoValue();
GObject* app_builder_get_sbPageSize();
GObject* app_builder_get_sbX();
GObject* app_builder_get_sbY();
GObject* app_builder_get_aXPage();
GObject* app_builder_get_aYPage();
GObject* app_builder_get_spMinIslands();
GObject* app_builder_get_lStartValue();
GObject* app_builder_get_lScoreValue();
GObject* app_builder_get_lVNew();
GObject* app_builder_get_lWNew();
GObject* app_builder_get_lPage();
GObject* app_builder_get_imgBirdWatch();
GObject* app_builder_get_BenchmarkImage();
void loadBirdWatchImage();
void app_widget_show_nwpopup(
	gchar* v, // v is up sequence
	gchar* w,
	gint lengthV,
	gint lengthW,
	gint zpage,
	gint xpage,
	gint ypage,
	gint pagesize,
	gint vType,
	gint wType,
	ScoringOptions* scoringOptions,
	KBandOptions* kBandOptions,
	gint numberOfThreads
);
void app_widget_refresh_nwpopup(
	gchar* v, // v is up sequence
	gchar* w,
	gint lengthV,
	gint lengthW,
	gint index,
	gint xpage,
	gint ypage,
	gint pagesize
);
void app_widget_show_swpopup(
	gchar* v, // v is up sequence
	gchar* w,
	gint lengthV,
	gint lengthW,
	gint vType,
	gint wType,
	ScoringOptions* scoringOptions,
	gint minValueIslands,
	gint numberOfThreads
);
gint app_entry_set_source(GtkEntry* entry, gchar* source);
void showIsland(int index);
void showNextIsland();
void showPrevIsland();
void freeResults();

#endif
