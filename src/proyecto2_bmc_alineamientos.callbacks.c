#include <gtk/gtk.h>
#include "proyecto2-bmc-alineamientos.h"
#include "proyecto2_bmc_alineamientos.handlers.h"
#include "FillingAlgorithms.h"

#ifndef CALLBACKS
#define CALLBACKS
/* ---------------------------------------------------------------- */
void on_window_init(GtkBuilder* sender) {
	app_set_builder(sender);

	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbGrowthInterval()), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbKValue()), FALSE);

	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbSubstitutionMatrix()), FALSE);
}
/* ---------------------------------------------------------------- */
void on_cbKBand_toggled(GtkCheckButton* sender) {
	gboolean checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sender));
	
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbGrowthInterval()), checked);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbKValue()), checked);
}
/* ---------------------------------------------------------------- */
void on_btGlobalAlignNW_clicked(GtkButton* sender) {
	gchar* v = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txV()));
	gchar* w = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txW()));

	gint matchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMatch()));
	gint missmatchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMissmatch()));
	gint gappenalty1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbF()));
	gint gappenalty2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbK()));
	gboolean freeleftgapsv = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbVLeftFG()));
	gboolean freeleftgapsw = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbWLeftFG()));
	gint numberOfThreads = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbNThreads()));
	
	ScoringOptions* options = ScoringOptions_new(
		matchbonus,
		missmatchbonus,
		gappenalty1,
		gappenalty2,
		freeleftgapsv,
		freeleftgapsw
	);
	
	Cell*** matrix = create_similarity_matrix_full(
		v,
		w,
		strlen(v),
		strlen(w),
		options,
		FALSE,
		numberOfThreads
	);

	app_widget_show_popup(matrix);

	g_critical("btGlobalAlignNW clicked");
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
		app_entry_set_source(GTK_ENTRY(app_builder_get_txV()), filename);
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
		app_entry_set_source(GTK_ENTRY(app_builder_get_txW()), filename);
		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}
/* ---------------------------------------------------------------- */
void on_btLocalAlignSW_clicked(GtkButton* sender) {
	gchar* v = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txV()));
	gchar* w = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txW()));
	
	GtkWidget* popup = GTK_WIDGET(app_builder_get_popup());

	gtk_widget_show_all(popup);
}
/* ---------------------------------------------------------------- */
void on_rbCustomValues_toggled(GtkRadioButton* sender) {
	gboolean selected = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_rbCustomValues()));
	
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbMatch()), selected);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbMissmatch()), selected);
	
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbSubstitutionMatrix()), !selected);
}
/* ---------------------------------------------------------------- */
gboolean on_popup_delete_event(GtkWidget* sender, GdkEvent* event) {
	gtk_widget_hide(sender);
	
	return TRUE;
}
/* ---------------------------------------------------------------- */
#endif