#include <gtk/gtk.h>
#include "proyecto2-bmc-alineamientos.h"

#ifndef HANDLERS
#define HANDLERS

/* Globals -------------------------------------------------------- */

GtkBuilder* builder = NULL;

/* ---------------------------------------------------------------- */
void app_set_builder(GtkBuilder* value) {
	builder = g_object_ref(value);
}
/* ---------------------------------------------------------------- */
GtkToggleButton* app_builder_get_cbKBand() {
	return GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "cbKBand"));
}
/* ---------------------------------------------------------------- */
GtkWidget* app_builder_get_sbGrowthInterval() {
	return GTK_WIDGET(gtk_builder_get_object(builder, "sbGrowthInterval"));
}
/* ---------------------------------------------------------------- */
GtkWidget* app_builder_get_sbKValue() {
	return GTK_WIDGET(gtk_builder_get_object(builder, "sbKValue"));
}
/* ---------------------------------------------------------------- */
GtkEntry* app_builder_get_txV() {
	return GTK_ENTRY(gtk_builder_get_object(builder, "txV"));
}
/* ---------------------------------------------------------------- */
GtkEntry* app_builder_get_txW() {
	return GTK_ENTRY(gtk_builder_get_object(builder, "txW"));
}
/* ---------------------------------------------------------------- */
GtkToggleButton* app_builder_get_rbCustomValues() {
	return GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "rbCustomValues"));
}
/* ---------------------------------------------------------------- */
GtkWidget* app_builder_get_sbMatch() {
	return GTK_WIDGET(gtk_builder_get_object(builder, "sbMatch"));
}
/* ---------------------------------------------------------------- */
GtkWidget* app_builder_get_sbMissmatch() {
	return GTK_WIDGET(gtk_builder_get_object(builder, "sbMissmatch"));
}
/* ---------------------------------------------------------------- */
GtkWidget* app_builder_get_cbSubstitutionMatrix() {
	return GTK_WIDGET(gtk_builder_get_object(builder, "cbSubstitutionMatrix"));
}
/* ---------------------------------------------------------------- */
GtkWidget* app_builder_get_popup() {
	return GTK_WIDGET(gtk_builder_get_object(builder, "popup"));
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
#endif
