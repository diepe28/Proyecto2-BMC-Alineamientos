#include "AlignmentOptions.h"

/* ---------------------------------------------------------------- */
ScoringOptions* ScoringOptions_new(
	gint matchBonus,
	gint missmatchPenalty,
	gint gapOpeningPenalty,
	gint gapExtensionPenalty,
	gboolean freeLeftGapsForX,
	gboolean freeLeftGapsForY
) {
	ScoringOptions* options = (ScoringOptions*) g_malloc(sizeof(ScoringOptions));
	options->matchBonus = matchBonus;
	options->missmatchPenalty = missmatchPenalty;
	options->gapOpeningPenalty = gapOpeningPenalty;
	options->gapExtensionPenalty = gapExtensionPenalty;
	options->freeLeftGapsForX = freeLeftGapsForX;
	options->freeLeftGapsForY = freeLeftGapsForY;
	options->substitutionMatrix = NULL;
	return options;
}
/* ---------------------------------------------------------------- */
KBandOptions* KBandOptions_new(
	gint kInitValue,
	gint kExtensionValue
) {
	KBandOptions* options = (KBandOptions*) g_malloc(sizeof(KBandOptions));
	options->kInitValue = kInitValue;
	options->kExtensionValue = kExtensionValue;
	return options;
}
/* ---------------------------------------------------------------- */