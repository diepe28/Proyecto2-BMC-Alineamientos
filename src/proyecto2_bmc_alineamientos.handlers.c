#include <gtk/gtk.h>
#include "Globals.h"
#include "proyecto2-bmc-alineamientos.h"
#include "proyecto2_bmc_alineamientos.handlers.h"
#include "BenchmarkSupport.h"

/* Globals -------------------------------------------------------- */

GtkBuilder* builder = NULL;
int currentIslandIndex;
int islandCount = -1;
SWBenchmarkResult* swBenchmarkResult = NULL;

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
GObject* app_builder_get_lAlgTypeValue() {
	return gtk_builder_get_object(builder, "lAlgTypeValue");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_lStartValue(){
	return gtk_builder_get_object(builder, "lStartValue");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_lScoreValue(){
	return gtk_builder_get_object(builder, "lScoreValue");
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
GObject* app_builder_get_lPage(){
	return gtk_builder_get_object(builder, "lPage");
}
/* ---------------------------------------------------------------- */
GObject* app_builder_get_imgBirdWatch(){
	return gtk_builder_get_object(builder, "imgBirdWatch");
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

	gchar* sSeq1Length = (gchar*) g_malloc(sizeof(gchar) * (log10(lengthV) + 1));
	gchar* sSeq2Length = (gchar*) g_malloc(sizeof(gchar) * (log10(lengthW) + 1));

	sprintf(sSeq1Length, "%d", lengthV);
	sprintf(sSeq2Length, "%d", lengthW);

	//TODO, check if is semi blobal alignment to set label 
	gtk_label_set_text(GTK_LABEL(app_builder_get_lAlgTypeValue()), "Global");
	
	gtk_label_set_text(GTK_LABEL(app_builder_get_lVLengthValue()), sSeq1Length);
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWLengthValue()), sSeq2Length);
	
	gtk_label_set_text(GTK_LABEL(app_builder_get_lVTypeValue()), APP_SEQUENCE_TYPE(vType));
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWTypeValue()), APP_SEQUENCE_TYPE(wType));
	
	gtk_label_set_text(GTK_LABEL(app_builder_get_lVNew()), alignment->upSequence);
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWNew()), alignment->leftSequence);

	char startPoint[15], scoreValue[15];

	sprintf(startPoint, "(%d,%d)",alignment->startRow, alignment->startCol);
	sprintf(scoreValue, "%d",alignment->maxValue);

	gtk_label_set_text(GTK_LABEL(app_builder_get_lPage()), "1/1");
	gtk_label_set_text(GTK_LABEL(app_builder_get_lStartValue()), startPoint);
	gtk_label_set_text(GTK_LABEL(app_builder_get_lScoreValue ()), scoreValue);

	loadBirdWatchImage();
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
	swBenchmarkResult = execute_sw_benchmark(w, v, lengthW, lengthV, scoringOptions, minValueIslands, numberOfThreads);
	islandCount = g_slist_length (swBenchmarkResult->islands);
	currentIslandIndex = 0;
	
	GSList* islands = swBenchmarkResult->islands;
	createBirdWatchGraphSW (islands, lengthW, lengthV);
	
	GtkWidget* popup = GTK_WIDGET(app_builder_get_popup());

	gtk_label_set_text(GTK_LABEL(app_builder_get_lAlgTypeValue()), "Local");
	
	gchar* sSeq1Length = (gchar*) g_malloc(sizeof(gchar) * (log10(lengthV) + 1));
	gchar* sSeq2Length = (gchar*) g_malloc(sizeof(gchar) * (log10(lengthW) + 1));

	sprintf(sSeq1Length, "%d", lengthV);
	sprintf(sSeq2Length, "%d", lengthW);

	gtk_label_set_text(GTK_LABEL(app_builder_get_lVLengthValue()), sSeq1Length);
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWLengthValue()), sSeq2Length);
	
	gtk_label_set_text(GTK_LABEL(app_builder_get_lVTypeValue()), APP_SEQUENCE_TYPE(vType));
	gtk_label_set_text(GTK_LABEL(app_builder_get_lWTypeValue()), APP_SEQUENCE_TYPE(wType));
	
	showIsland(currentIslandIndex);
	loadBirdWatchImage();
	
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

void showIsland (int index){
	char* tipoAli = (char*) gtk_label_get_text(GTK_LABEL(app_builder_get_lAlgTypeValue()));
	if(index < islandCount && strcmp(tipoAli, "Local") == 0){
		Island* island = (Island*) g_slist_nth_data (swBenchmarkResult->islands,index);
		char currentIsland[10], startPoint[15], scoreValue[15];

		sprintf(currentIsland, "%d/%d",index+1, islandCount);
		sprintf(startPoint, "(%d,%d)",island->startRow, island->startCol);
		sprintf(scoreValue, "%d",island->maxValue);
		
		gtk_label_set_text(GTK_LABEL(app_builder_get_lVNew()), island->upSequence);
		gtk_label_set_text(GTK_LABEL(app_builder_get_lWNew()), island->leftSequence);
		gtk_label_set_text(GTK_LABEL(app_builder_get_lStartValue()), startPoint);
		gtk_label_set_text(GTK_LABEL(app_builder_get_lPage()), currentIsland);
		gtk_label_set_text(GTK_LABEL(app_builder_get_lScoreValue()), scoreValue);
	}
}
/* ---------------------------------------------------------------- */
void showNextIsland(){
	if(currentIslandIndex+1 < islandCount){
		showIsland(++currentIslandIndex);
	}
}
/* ---------------------------------------------------------------- */
void showPrevIsland(){
	if((currentIslandIndex-1) >= 0){
		showIsland(--currentIslandIndex);
	}
}
/* ---------------------------------------------------------------- */
void loadBirdWatchImage(){
	GError *gerror = 0;
	GtkImage* image = app_builder_get_imgBirdWatch();
	gtk_image_clear (image);
	
	int width = gtk_widget_get_allocated_width (image);
	int heigth = gtk_widget_get_allocated_height (image);
	if(width < 100) width = 400;
	if(heigth < 100) heigth = 350;
	
	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file ("BIRD_WATCH.png", &gerror); 
	GdkPixbuf* resized = gdk_pixbuf_scale_simple (pixbuf,width ,heigth, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(image, resized);
}
/* ---------------------------------------------------------------- */
