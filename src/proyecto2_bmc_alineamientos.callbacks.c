#include <gtk/gtk.h>
#include "proyecto2-bmc-alineamientos.h"
#include "proyecto2_bmc_alineamientos.handlers.h"
#include "BenchmarkSupport.h"
#include "Gridview.h"

#ifndef CALLBACKS
#define CALLBACKS

typedef enum {
	ADN = 0,
	Protein = 1,
	Text = 2
} SequenceType;

typedef enum {
	BLOSUM_45_Matrix = 0,
	BLOSUM_62_Matrix = 1,
	BLOSUM_80_Matrix = 2,
	PAM_70_Matrix = 3,
	PAM_250_Matrix = 4
} SubstitutionMatrix;

const gchar* authors[4] = {"Olger Calderón Achío", "Wilberth Castro Fuentes", "Diego Pérez Arroyo", NULL};
gboolean isLocalAlignment = FALSE;
// Helper Functions

static void show_message(gchar* message, GtkMessageType type) 
{
	GtkMessageDialog* dialog = NULL;
	dialog = GTK_MESSAGE_DIALOG (gtk_message_dialog_new (
			                     GTK_WINDOW(app_builder_get_window()),
		                         GTK_DIALOG_MODAL,
		                         type,
		                         GTK_BUTTONS_CLOSE,
	                             "%s",
	                             message));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

static gboolean pre_process_sequence(gchar* inputSequence, gint seqSize, gchar seqID) 
{
	SequenceType seqType = (seqID == 'v') ? 
		(SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType())) :
		(SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
	SequenceType otherSeqType = (seqID == 'w') ?
		(SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType())) :
		(SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
	GtkEntry* sequenceEntry = (seqID == 'v') ? GTK_ENTRY(app_builder_get_txV()) : GTK_ENTRY(app_builder_get_txW());
	GtkComboBox* seqComboBox = (seqID == 'v') ? GTK_COMBO_BOX(app_builder_get_cbVInputType()) : GTK_COMBO_BOX(app_builder_get_cbWInputType());
	gchar* newSequence = NULL;
	gchar message[100];
	switch (seqType) 
	{
		case Text:
			return FALSE;
		case Protein:
			newSequence = threeLetterCodedProteinToOneLetterCodedProtein(inputSequence, seqSize);
			if (newSequence != NULL) {
				g_snprintf(message, sizeof(message), "Se transformó la hilera '%c' a código de proteína de una letra.", seqID);
				show_message (message, GTK_MESSAGE_INFO);
				gtk_entry_set_text (sequenceEntry, newSequence);
				g_free(newSequence);
			}
			return TRUE;
		case ADN:
			if (otherSeqType == Protein) {
				newSequence = dnaToProtein(inputSequence);
				g_snprintf(message, sizeof(message), "Se transformó la hilera '%c' a proteína usando el código genético.", seqID);
				show_message (message, GTK_MESSAGE_INFO);
				gtk_combo_box_set_active(seqComboBox, (gint)Protein);
				gtk_entry_set_text (sequenceEntry, newSequence);
				g_free(newSequence);
				return TRUE;
			}
			break;
	}
	return FALSE;
}

// Helper functions

static void set_substitution_matrix(ScoringOptions* options) 
{
	if (gtk_widget_get_sensitive(GTK_WIDGET(app_builder_get_cbSubstitutionMatrix()))) {
		SubstitutionMatrix selectedMatrix = (SubstitutionMatrix) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbSubstitutionMatrix()));
		switch (selectedMatrix) {
			case BLOSUM_45_Matrix:
				options->substitutionMatrix = BLOSUM_45;
				break;
			case BLOSUM_62_Matrix:
				options->substitutionMatrix = BLOSUM_62;
				break;
			case BLOSUM_80_Matrix:
				options->substitutionMatrix = BLOSUM_80;
				break;
			case PAM_70_Matrix:
				options->substitutionMatrix = PAM_70;
				break;
			case PAM_250_Matrix:
				options->substitutionMatrix = PAM_250;
				break;
		}
	}
}

static void update_substitution_matrix_dropdown()
{
	if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbKBand()))) {
		GtkToggleButton* rbSubstitutionMatrix = GTK_TOGGLE_BUTTON(app_builder_get_rbSubstitutionMatrix());
		SequenceType seqType1 = (SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType()));
		SequenceType seqType2 = (SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
		if (seqType1 != Protein && seqType2 != Protein) {
			if (gtk_toggle_button_get_active(rbSubstitutionMatrix)) {
				show_message("La opción de matrices de sustitución será desactivada dado que ninguna de las secuencias es una proteína.", GTK_MESSAGE_WARNING);
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app_builder_get_rbCustomValues()), TRUE);
			}
			gtk_widget_set_sensitive(GTK_WIDGET(rbSubstitutionMatrix), FALSE);
		} else
			gtk_widget_set_sensitive(GTK_WIDGET(rbSubstitutionMatrix), TRUE);
	}
}

static void deactivate_kband_incompatible_features() 
{
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(app_builder_get_cbVLeftFG()), FALSE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(app_builder_get_cbWLeftFG()), FALSE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(app_builder_get_cbVRightFG()), FALSE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(app_builder_get_cbWRightFG()), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbVLeftFG()), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbWLeftFG()), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbVRightFG()), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbWRightFG()), FALSE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(app_builder_get_rbCustomValues()), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_rbSubstitutionMatrix()), FALSE);
	GtkSpinButton* spin_button = GTK_SPIN_BUTTON(app_builder_get_sbF());
	gtk_spin_button_set_value(spin_button, 0);
	gtk_widget_set_sensitive(GTK_WIDGET(spin_button), FALSE);
}

static void activate_kband_incompatible_features()
{
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbVLeftFG()), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbWLeftFG()), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbVRightFG()), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbWRightFG()), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbF()), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_rbSubstitutionMatrix()), TRUE);
	update_substitution_matrix_dropdown();
}

static gboolean validate_sequences_types() 
{
	SequenceType seqType1 = (SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType()));
	SequenceType seqType2 = (SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
	if (seqType1 != seqType2 && !((seqType1 == ADN && seqType2 == Protein) || (seqType1 == Protein && seqType2 == ADN))) {
		show_message("Los tipos seleccionados para las hileras son incompatibles.", GTK_MESSAGE_ERROR);
		return FALSE;
	}
	return TRUE;
}

static gboolean validate_ADN(const gchar* seq, gint size) 
{
	gint i = 0;
	for (i = 0; i < size; i++)
		if (seq[i] != 'A' && seq[i] != 'G' && seq[i] != 'C' && seq[i] != 'T')
			return FALSE;
	return TRUE;
}

static gboolean validate_Protein(const gchar* seq, gint size) 
{
	gchar* convertedSequence = threeLetterCodedProteinToOneLetterCodedProtein (seq, size);
	if (convertedSequence != NULL) {
		g_free(convertedSequence);
		return TRUE;
	}
	
	gint i = 0;
	for (i = 0; i < size; i++) {
		if (seq[i] == '*')
			continue;
		if (!g_ascii_isupper(seq[i]) || seq[i] == 'B' || seq[i] == 'J' || seq[i] == 'O' || seq[i] == 'U' || seq[i] == 'Z')
			return FALSE;
	}
	return TRUE;
}

static gboolean validate_Text(const gchar* seq, gint size) 
{
	gint i = 0;
	for (i = 0; i < size; i++)
		if (!g_ascii_isprint(seq[i]))
			return FALSE;
	return TRUE;
}

static gboolean validate_sequences_characters(gint vSize, gint wSize) 
{
	const gchar* v = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txV()));
	const gchar* w = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txW()));
	SequenceType seqTypeV = (SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType()));
	SequenceType seqTypeW = (SequenceType) gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));

	gboolean result = FALSE;

	if (vSize == 0) {
		show_message("No se introdujo ninguna hilera para 'v'.", GTK_MESSAGE_ERROR);
		return FALSE;
	}

	if (wSize == 0) {
		show_message("No se introdujo ninguna hilera para 'w'.", GTK_MESSAGE_ERROR);
		return FALSE;
	}
	
	switch (seqTypeV) {
		case ADN:
			result = validate_ADN(v, vSize);
			if (!result) show_message("La secuencia 'v' no es una hilera de ADN válida.", GTK_MESSAGE_ERROR);
			break;
		case Protein:
			result = validate_Protein(v, vSize);
			if (!result) show_message("La secuencia 'v' no es una hilera de Proteína válida.", GTK_MESSAGE_ERROR);
			break;
		case Text:
			result = validate_Text(v, vSize);
			if (!result) show_message("La secuencia 'v' no está en un formato válido (ASCII, caracteres imprimibles).", GTK_MESSAGE_ERROR);
			break;
	}

	if (result) {
		switch (seqTypeW) {
			case ADN:
				result = validate_ADN(w, wSize);
				if (!result) show_message("La secuencia 'w' no es una hilera de ADN válida.", GTK_MESSAGE_ERROR);
				break;
			case Protein:
				result = validate_Protein(w, wSize);
				if (!result) show_message("La secuencia 'w' no es una hilera de Proteína válida.", GTK_MESSAGE_ERROR);
				break;
			case Text:
				result = validate_Text(w, wSize);
				if (!result) show_message("La secuencia 'w' no está en un formato válido (ASCII, caracteres imprimibles).", GTK_MESSAGE_ERROR);
				break;
		}
	}
	
	return result;
}

static gboolean validate_required_memory(gint seq1Size, gint seq2Size)
{
	gint width = seq1Size + 1;
	gint height = seq2Size + 1;
	gulong requiredSize = (((height * width * sizeof(Cell)) + (height * width * sizeof(Cell*)) + (height * sizeof(Cell**))) / 1024) + 1024;
	
	FILE *meminfo = fopen("/proc/meminfo", "r");
    if(meminfo == NULL) {
        puts("Error while opening /proc/meminfo");
		return TRUE;
	}

    gchar line[256];
	gulong free_ram = 0;
	gulong cached_ram = 0;
	gulong buffered_ram = 0;
	
    while(fgets(line, sizeof(line), meminfo))
    {
        if(sscanf(line, "MemFree: %lu kB", &free_ram) == 1) continue;
		if(sscanf(line, "Buffers: %lu kB", &buffered_ram) == 1) continue;
		if(sscanf(line, "Cached: %lu kB", &cached_ram) == 1) continue;
    }

    fclose(meminfo);
    if (free_ram == 0 && cached_ram == 0 && buffered_ram == 0)
		return TRUE;

	gboolean success = (free_ram + cached_ram + buffered_ram) > requiredSize;
	if (!success) {
		gchar text[200];
		g_snprintf(text, sizeof(text), "No hay suficiente memoria RAM para procesar el alineamiento.\nMemoria requerida:  %lu kB\nMemoria disponible: %lu kB", requiredSize, free_ram + cached_ram + buffered_ram);
		show_message(text, GTK_MESSAGE_ERROR);
	}
	
	return success;
}

static gboolean preprocess_validation(gint seq1Size, gint seq2Size)
{
	return validate_sequences_types() && validate_sequences_characters(seq1Size, seq2Size);
}

// End of Helper Functions

/* ---------------------------------------------------------------- */
void on_window_init(GtkBuilder* sender) {
	app_set_builder(sender);

	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbGrowthInterval()), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbKValue()), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_cbSubstitutionMatrix()), FALSE);
	update_substitution_matrix_dropdown();
	
	GtkWindow* mainWindow = GTK_WINDOW(app_builder_get_window());
	gtk_window_set_icon_from_file (mainWindow, "src/icon_window.png", NULL);
	gtk_window_set_default_icon_from_file ("src/icon_window.png", NULL);
	
	GtkWidget* gridview = GTK_WIDGET(app_builder_get_gridview());
	gridview_init(gridview);
}

/* ---------------------------------------------------------------- */

void on_type_changed(GtkComboBox* sender) 
{
	update_substitution_matrix_dropdown ();
}

/* ---------------------------------------------------------------- */

void on_cbKBand_toggled(GtkCheckButton* sender) {
	gboolean checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sender));
	if (checked) {
		GtkMessageDialog* dialog = NULL;
		dialog = GTK_MESSAGE_DIALOG (gtk_message_dialog_new (
			                         GTK_WINDOW(app_builder_get_window()),
		                             GTK_DIALOG_MODAL,
		                             GTK_MESSAGE_QUESTION,
		                             GTK_BUTTONS_YES_NO,
		                             "Si se activa esta opción las siguientes características serán desactivadas: funciones de penalización afines, gaps gratis y matrices de sustitución. ¿Desea continuar?"));
		gint result = gtk_dialog_run (GTK_DIALOG (dialog));
		switch (result)
		{
			case GTK_RESPONSE_YES:
				gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbGrowthInterval()), TRUE);
				gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbKValue()), TRUE);
				deactivate_kband_incompatible_features();
				break;
			default:
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(sender), FALSE);
				break;
		}
		gtk_widget_destroy (GTK_WIDGET(dialog));
	} else {
		gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbGrowthInterval()), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(app_builder_get_sbKValue()), FALSE);
		activate_kband_incompatible_features();
	}
}
/* ---------------------------------------------------------------- */

void on_btGlobalAlignNW_clicked(GtkButton* sender) {
	isLocalAlignment = FALSE;
	const gchar* v = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txV()));
	const gchar* w = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txW()));
	gint vSize = strlen(v);
	gint wSize = strlen(w);
	if (preprocess_validation(vSize, wSize)) {
		gint vtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType()));
		gint wtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
	
		gint matchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMatch()));
		gint missmatchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMissmatch()));
		gint openGapPenalty = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbF()));
		gint gappenalty2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbK()));
		gboolean freeleftgapsv = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbVLeftFG()));
		gboolean freeleftgapsw = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbWLeftFG()));
		gboolean freerightgapsv = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbVRightFG()));
		gboolean freerightgapsw = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbWRightFG()));
		gboolean usingKBand = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app_builder_get_cbKBand()));
		gint kbandValue = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbKValue()));
		gint kbandGrowth = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbGrowthInterval()));
		gint numberOfThreads = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbNThreads()));

		gboolean wasVConverted = pre_process_sequence(v, vSize, 'v');
		gboolean wasWConverted = pre_process_sequence(w, wSize, 'w');
		if (wasVConverted) {
			v = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txV()));
			vSize = strlen(v);
			vtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType()));
		}
		if (wasWConverted) {
			w = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txW()));
			wSize = strlen(w);
			wtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
		}
		
		if (!validate_required_memory(vSize, wSize))
			return;
		
		ScoringOptions* scoringOptions = ScoringOptions_new(
			matchbonus,
			missmatchbonus,
			openGapPenalty,
			gappenalty2,
			freeleftgapsv,
			freeleftgapsw,
			freerightgapsv,
			freerightgapsw,
			NULL
		);
		set_substitution_matrix(scoringOptions);

		KBandOptions* kBandOptions = NULL;
		if(usingKBand){
			kBandOptions = KBandOptions_new(
				kbandValue,
				kbandGrowth
			);
		}

		gint zpage = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbGotoValue()));
	
		gint xpage = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbX()));
		gint ypage = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbY()));
	
		gint pagesize = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbPageSize()));
	
		app_widget_show_nwpopup(
			v,
			w,
			vSize,
			wSize,
			zpage,
			xpage - 1,
			ypage - 1,
			pagesize,
			vtype,
			wtype,
			scoringOptions,
			kBandOptions,
			numberOfThreads
		);

		loadBirdWatchImage();
	}
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
	isLocalAlignment = TRUE;
	const gchar* v = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txV()));
	const gchar* w = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txW()));
	gint vSize = strlen(v);
	gint wSize = strlen(w);
	if (preprocess_validation(vSize, wSize)) {
		
		gint vtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType()));
		gint wtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
	
		gint matchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMatch()));
		gint missmatchbonus = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbMissmatch()));
		gint gappenalty1 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbF()));
		gint gappenalty2 = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbK()));
		gint numberOfThreads = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbNThreads()));
		gint minValueIslands = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_spMinIslands()));

		gboolean wasVConverted = pre_process_sequence(v, vSize, 'v');
		gboolean wasWConverted = pre_process_sequence(w, wSize, 'w');
		if (wasVConverted) {
			v = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txV()));
			vSize = strlen(v);
			vtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbVInputType()));
		}
		if (wasWConverted) {
			w = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txW()));
			wSize = strlen(w);
			wtype = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbWInputType()));
		}
		
		if (!validate_required_memory(vSize, wSize))
			return;
		
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
		set_substitution_matrix(options);

		app_widget_show_swpopup(
			v,
			w,
			vSize,
			wSize,
			vtype,
			wtype,
			options,
			minValueIslands,
			numberOfThreads
		);
	}
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
	freeResults();
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

	gint zpage = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbGotoValue()));

	gint xpage = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbX()));
	gint ypage = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbY()));

	gint pagesize = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbPageSize()));
	
	app_widget_refresh_popup(v, w, strlen(v), strlen(w), zpage, xpage, ypage, pagesize, isLocalAlignment);
	
	g_critical("cbGotoValue changed!");
}
/* ---------------------------------------------------------------- */
void on_helpmenuitem_activate(GtkMenuItem *menuitem, gpointer user_data) {
	GtkAboutDialog *dialog = GTK_ABOUT_DIALOG(gtk_about_dialog_new());
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Alineador de Hileras");
	gtk_about_dialog_set_comments(
		GTK_ABOUT_DIALOG(dialog), 
	  "Proyecto #2 del curso: Introducción a la Biología Molecular Computacional."
	);

	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("src/icon.png", NULL);
	gtk_about_dialog_set_logo(dialog, pixbuf);
	g_object_unref(pixbuf);
	pixbuf = NULL; 
	
	gtk_about_dialog_set_authors(dialog, authors);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));
}
/* ---------------------------------------------------------------- */
void on_menuitem_exit(GtkMenuItem *menuitem, gpointer user_data) {
	GtkWindow* mainWindow = GTK_WINDOW(app_builder_get_window());
	GtkApplication* application = gtk_window_get_application(mainWindow);
	g_application_quit(G_APPLICATION(application));
}
/* ---------------------------------------------------------------- */
void on_sbX_value_changed(GtkSpinButton *sender) {
	g_critical("sbX changed!");
}
/* ---------------------------------------------------------------- */
void on_sbY_value_changed(GtkSpinButton *sender) {
	g_critical("sbY changed!");
}
/* ---------------------------------------------------------------- */
void on_page_changed(GObject* sender) {	
	gint zpage = gtk_combo_box_get_active(GTK_COMBO_BOX(app_builder_get_cbGotoValue()));
	
	gint xpage = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbX()));
	gint ypage = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbY()));
	
	gint pagesize = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app_builder_get_sbPageSize()));
	
	gchar* v = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txV()));
	gchar* w = gtk_entry_get_text(GTK_ENTRY(app_builder_get_txW()));
	
	app_widget_refresh_popup(v, w, strlen(v), strlen(w), zpage, xpage - 1, ypage - 1, pagesize, isLocalAlignment);
}
/* ---------------------------------------------------------------- */
#endif
