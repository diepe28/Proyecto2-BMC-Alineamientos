#include <gtk/gtk.h>
#include "Globals.h"
#include "proyecto2-bmc-alineamientos.h"
#include "proyecto2_bmc_alineamientos.handlers.h"
#include "BenchmarkSupport.h"

/* Globals -------------------------------------------------------- */

GtkBuilder* builder = NULL;

/* ---------------------------------------------------------------- */
void app_set_builder(GtkBuilder* value) {
	builder = g_object_ref(value);
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_cbKBand() {
	return gtk_builder_get_object(builder, "cbKBand");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_sbGrowthInterval() {
	return gtk_builder_get_object(builder, "sbGrowthInterval");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_sbKValue() {
	return gtk_builder_get_object(builder, "sbKValue");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_txV() {
	return gtk_builder_get_object(builder, "txV");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_txW() {
	return gtk_builder_get_object(builder, "txW");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_rbCustomValues() {
	return gtk_builder_get_object(builder, "rbCustomValues");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_sbMatch() {
	return gtk_builder_get_object(builder, "sbMatch");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_sbMissmatch() {
	return gtk_builder_get_object(builder, "sbMissmatch");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_sbF() {
	return gtk_builder_get_object(builder, "sbF");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_sbK() {
	return gtk_builder_get_object(builder, "sbK");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_cbVLeftFG() {
	return gtk_builder_get_object(builder, "cbVLeftFG");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_cbWLeftFG() {
	return gtk_builder_get_object(builder, "cbWLeftFG");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_cbVRightFG() {
	return gtk_builder_get_object(builder, "cbVRightFG");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_cbWRightFG() {
	return gtk_builder_get_object(builder, "cbWRightFG");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_sbNThreads() {
	return gtk_builder_get_object(builder, "sbNThreads");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_cbSubstitutionMatrix() {
	return gtk_builder_get_object(builder, "cbSubstitutionMatrix");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_cbVInputType() {
	return gtk_builder_get_object(builder, "cbVInputType");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_cbWInputType() {
	return gtk_builder_get_object(builder, "cbWInputType");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_popup() {
	return gtk_builder_get_object(builder, "popup");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_lAlgorithmValue() {
	return gtk_builder_get_object(builder, "lAlgorithmValue");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_lVNew() {
	return gtk_builder_get_object(builder, "lVNew");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_lWNew() {
	return gtk_builder_get_object(builder, "lWNew");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_lVLengthValue() {
	return gtk_builder_get_object(builder, "lVLengthValue");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_lWLengthValue() {
	return gtk_builder_get_object(builder, "lWLengthValue");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_lVTypeValue() {
	return gtk_builder_get_object(builder, "lVTypeValue");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_lWTypeValue() {
	return gtk_builder_get_object(builder, "lWTypeValue");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_spMinIslands() {
	return gtk_builder_get_object(builder, "spMinIslands");
}
/* ---------------------------------------------------------------- */
void app_widget_show_nwpopup(
	gchar* v, // v is up sequence
	gchar* w,
	gint lengthV,
	gint lengthW,
	gint vType,
	gint wType,
	ScoringOptions* scoringOptions,
	KBandOptions* kBandOptions,
	gint numberOfThreads
) {

	NWBenchmarkResult* result =
		execute_nw_benchmark(w, v, lengthW, lengthV, scoringOptions,kBandOptions,numberOfThreads);

	Island* alignment = result->alignment;
	createBirdWatchGraphNW(alignment, lengthW,  lengthV);
	
	
	GtkWidget* popup = GTK_WIDGET(app_builder_get_popup());

	gtk_label_set_text(GTK_LABEL(app_builder_get_lAlgorithmValue()), "Needleman-Wunsch");
	
	gchar* sSeq1Length = (gchar*) g_malloc(sizeof(gchar) * (log10(lengthV) + 1));
	gchar* sSeq2Length = (gchar*) g_malloc(sizeof(gchar) * (log10(lengthW) + 1));

	sprintf(sSeq1Length, "%d", lengthV);
	sprintf(sSeq2Length, "%d", lengthW);

	gtk_label_set_text(GTK_LABEL(app_builder_get_lVLengthValue()), sSeq1Length);
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWLengthValue()), sSeq2Length);
	
	gtk_label_set_text(GTK_LABEL(app_builder_get_lVTypeValue()), APP_SEQUENCE_TYPE(vType));
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWTypeValue()), APP_SEQUENCE_TYPE(wType));
	
	gtk_label_set_text(GTK_LABEL(app_builder_get_lVNew()), alignment->upSequence);
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWNew()), alignment->leftSequence);

	gtk_widget_show_all(popup);
}
/* ---------------------------------------------------------------- */
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
) {
	SWBenchmarkResult* benchmarkResult = execute_sw_benchmark(w, v, lengthW, lengthV, scoringOptions, minValueIslands, numberOfThreads);
	GSList* islands = benchmarkResult->islands;
	createBirdWatchGraphSW (islands, lengthW, lengthV);
	
	GtkWidget* popup = GTK_WIDGET(app_builder_get_popup());

	gtk_label_set_text(GTK_LABEL(app_builder_get_lAlgorithmValue()), "Smith-Waterman");
	
	gchar* sSeq1Length = (gchar*) g_malloc(sizeof(gchar) * (log10(lengthV) + 1));
	gchar* sSeq2Length = (gchar*) g_malloc(sizeof(gchar) * (log10(lengthW) + 1));

	sprintf(sSeq1Length, "%d", lengthV);
	sprintf(sSeq2Length, "%d", lengthW);

	gtk_label_set_text(GTK_LABEL(app_builder_get_lVLengthValue()), sSeq1Length);
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWLengthValue()), sSeq2Length);
	
	gtk_label_set_text(GTK_LABEL(app_builder_get_lVTypeValue()), APP_SEQUENCE_TYPE(vType));
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWTypeValue()), APP_SEQUENCE_TYPE(wType));
	
	//gtk_label_set_text(GTK_LABEL(app_builder_get_lVNew()), benchmarkResult->result->upSequence);
	//gtk_label_set_text(GTK_LABEL(app_builder_get_lWNew()), benchmarkResult->result->leftSequence);

	gtk_widget_show_all(popup);

}

/* ---------------------------------------------------------------- */
gint app_entry_set_source(GtkEntry* entry, gchar* source) {
	FILE* file;
	if (!(file = fopen(source, "r"))) {
		return 0;
	}

	fseek(file, 0L, SEEK_END);
	gint size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	
	gchar* value = g_malloc(sizeof(gchar*) * size);
	gint result = fscanf(file, "%s", value);

	gtk_entry_set_text(entry, value);

	return result;
}
/* ---------------------------------------------------------------- */
