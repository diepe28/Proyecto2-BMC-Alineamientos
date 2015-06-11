#include <gtk/gtk.h>
#include "proyecto2-bmc-alineamientos.h"
#include "proyecto2_bmc_alineamientos.handlers.h"
#include "BenchmarkSupport.h"
#include "Gridview.h"

#ifndef CALLBACKS
#define CALLBACKS

/* ---------------------------------------------------------------- */
void on_window_init(GtkBuilder* sender) {
	app_set_builder(sender);

	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbGrowthInterval()), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbKValue()), FALSE);

	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbSubstitutionMatrix()), FALSE);

	GtkWidget* gridview = GTK_WIDGET(app_builder_get_gridview());
	gridview_init(gridview);
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
	gint vtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType()));
	gint wtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
	
	gint matchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMatch()));
	gint missmatchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMissmatch()));
	gint gappenalty1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbF()));
	gint gappenalty2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbK()));
	gboolean freeleftgapsv = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbVLeftFG()));
	gboolean freeleftgapsw = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbWLeftFG()));
	gboolean freerightgapsv = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbVRightFG()));
	gboolean freerightgapsw = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbWRightFG()));
	gboolean usingKBand = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbKBand()));
	gint kbandValue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbKValue()));
	gint kbandGrowth = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbGrowthInterval()));
	gint numberOfThreads = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbNThreads()));
	
	gint index = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbGotoValue()));


	//TODO GET SUBSTITUTION_MATRIXES
	ScoringOptions* scoringOptions = ScoringOptions_new(
		matchbonus,
		missmatchbonus,
		gappenalty1,
		gappenalty2,
		freeleftgapsv,
		freeleftgapsw,
		freerightgapsv,
		freerightgapsw,
		NULL
	);

	KBandOptions* kBandOptions = NULL;
	if(usingKBand){
		kBandOptions = KBandOptions_new(
			kbandValue,
			kbandGrowth
		);
	}
	
	app_widget_show_nwpopup(
		v,
		w,
		strlen(v),
		strlen(w),
		index,
		vtype,
		wtype,
		scoringOptions,
		kBandOptions,
		numberOfThreads
	);

	loadBirdWatchImage();
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
	gint vtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType()));
	gint wtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
	
	gint matchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMatch()));
	gint missmatchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMissmatch()));
	gint gappenalty1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbF()));
	gint gappenalty2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbK()));
	gint numberOfThreads = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbNThreads()));
	gint minValueIslands = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_spMinIslands()));
	
	ScoringOptions* options = ScoringOptions_new(
		matchbonus,
		missmatchbonus,
		gappenalty1,
		gappenalty2,
		FALSE,
		FALSE,
		FALSE,
		FALSE,
		NULL
	);

	app_widget_show_swpopup(
		v,
		w,
		strlen(v),
		strlen(w),
		vtype,
		wtype,
		options,
		minValueIslands,
		numberOfThreads
	);
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
void on_btnResize_clicked(GtkButton* sender) {
	loadBirdWatchImage();
}
/* ---------------------------------------------------------------- */

void on_tbNext_clicked(GtkButton* sender) { 
	showNextIsland();
}
/* ---------------------------------------------------------------- */
void on_tbPrev_clicked(GtkButton* sender) { 
	showPrevIsland();
}
/* ---------------------------------------------------------------- */
void on_cbGotoValue_changed(GtkComboBox* sender) {
	gchar* v = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txV()));
	gchar* w = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txW()));
	
	gint index = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbGotoValue()));
	
	app_widget_refresh_nwpopup(v, w, strlen(v), strlen(w), index);
	
	g_critical("cbGotoValue changed!");
}
/* ---------------------------------------------------------------- */
#endif
