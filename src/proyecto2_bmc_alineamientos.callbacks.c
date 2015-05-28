#include <gtk/gtk.h>
#include "proyecto2-bmc-alineamientos.h"

#ifndef CALLBACKS
#define CALLBACKS
/* ---------------------------------------------------------------- */
void on_window_init(GtkBuilder* sender) {
	app_set_builder(sender);

	gtk_widget_set_sensitive(app_builder_get_sbGrowthInterval(), FALSE);
	gtk_widget_set_sensitive(app_builder_get_sbKValue(), FALSE);

	gtk_widget_set_sensitive(app_builder_get_cbSubstitutionMatrix(), FALSE);
}
/* ---------------------------------------------------------------- */
void on_cbKBand_toggled(GtkCheckButton* sender) {
	gboolean checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sender));
	
	gtk_widget_set_sensitive(app_builder_get_sbGrowthInterval(), checked);
	gtk_widget_set_sensitive(app_builder_get_sbKValue(), checked);
}
/* ---------------------------------------------------------------- */
void on_btGlobalAlignNW_clicked(GtkButton* sender) {
	GtkWidget* popup = app_builder_get_popup();

	gtk_widget_show_all(popup);
}
/* ---------------------------------------------------------------- */
void on_btVLoad_clicked(GtkButton* sender) {
	GtkWidget* dialog = gtk_file_chooser_dialog_new(
		"Open",
		NULL,
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"Cancel",
		GTK_RESPONSE_CANCEL,
		"Open",
		GTK_RESPONSE_ACCEPT,
		NULL
	);

	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res == GTK_RESPONSE_ACCEPT) {
		GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
		gchar* filename = gtk_file_chooser_get_filename(chooser);
		app_entry_set_source(app_builder_get_txV(), filename);
		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}
/* ---------------------------------------------------------------- */
void on_btWLoad_clicked(GtkButton* sender) {
	GtkWidget* dialog = gtk_file_chooser_dialog_new(
		"Open",
		NULL,
		GTK_FILE_CHOOSER_ACTION_OPEN,
		"Cancel",
		GTK_RESPONSE_CANCEL,
		"Open",
		GTK_RESPONSE_ACCEPT,
		NULL
	);

	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res == GTK_RESPONSE_ACCEPT) {
		GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
		gchar* filename = gtk_file_chooser_get_filename(chooser);
		app_entry_set_source(app_builder_get_txW(), filename);
		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}
/* ---------------------------------------------------------------- */
void on_btLocalAlignSW_clicked(GtkButton* sender) {
	GtkWidget* popup = app_builder_get_popup();

	gtk_widget_show_all(popup);
}
/* ---------------------------------------------------------------- */
void on_rbCustomValues_toggled(GtkRadioButton* sender) {
	gboolean selected = gtk_toggle_button_get_active(app_builder_get_rbCustomValues());
	
	gtk_widget_set_sensitive(app_builder_get_sbMatch(), selected);
	gtk_widget_set_sensitive(app_builder_get_sbMissmatch(), selected);
	
	gtk_widget_set_sensitive(app_builder_get_cbSubstitutionMatrix(), !selected);
}
/* ---------------------------------------------------------------- */
gboolean on_popup_delete_event(GtkWidget* sender, GdkEvent* event) {
	gtk_widget_hide(sender);
	
	return TRUE;
}
/* ---------------------------------------------------------------- */
#endif
