#include <gtk/gtk.h>
#include "Globals.h"
#include "proyecto2-bmc-alineamientos.h"
#include "proyecto2_bmc_alineamientos.handlers.h"

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
) {
	Cell*** matrix = create_similarity_matrix_full(
		seq2,
		seq1,
		seq2Length,
		seq1Length,
		scoringOptions,
		isLocalAlignment,
		numberOfThreads
	);

	char** newseqs = afterMatrixFilling_find_NW_Alignment(
		matrix,
		seq1,
		seq2,
		seq2Length,
		seq1Length,
		freeRightGapsUp,
		freeRightGapsLeft,
		TRUE
	);
	
	GtkWidget* popup = GTK_WIDGET(app_builder_get_popup());

	gtk_label_set_text(GTK_LABEL(app_builder_get_lAlgorithmValue()), "Needleman-Wunsch");
	
	gchar* sSeq1Length = (gchar*) g_malloc(sizeof(gchar) * (log10(seq1Length) + 1));
	gchar* sSeq2Length = (gchar*) g_malloc(sizeof(gchar) * (log10(seq2Length) + 1));

	sprintf(sSeq1Length, "%d", seq1Length);
	sprintf(sSeq2Length, "%d", seq2Length);

	gtk_label_set_text(GTK_LABEL(app_builder_get_lVLengthValue()), sSeq1Length);
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWLengthValue()), sSeq2Length);
	
	gtk_label_set_text(GTK_LABEL(app_builder_get_lVTypeValue()), APP_SEQUENCE_TYPE(seq1Type));
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWTypeValue()), APP_SEQUENCE_TYPE(seq1Type));
	
	gtk_label_set_text(GTK_LABEL(app_builder_get_lVNew()), newseqs[0]);
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWNew()), newseqs[1]);

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
