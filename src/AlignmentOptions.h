#include <glib.h>

#ifndef ALIGNMENT_OPTIONS_H
#define ALIGNMENT_OPTIONS_H

typedef struct {
	gint matchBonus;
	gint missmatchPenalty;
	gint gapOpeningPenalty;
	gint gapExtensionPenalty;
	gboolean freeLeftGapsForX;
	gboolean freeLeftGapsForY;
	gboolean freeRightGapsForX;
	gboolean freeRightGapsForY;
	gint (*substitutionMatrix)[26];
} ScoringOptions;

typedef struct {
	gint kInitValue;
	gint kExtensionValue;
} KBandOptions;

ScoringOptions* ScoringOptions_new(
	gint matchBonus,
	gint missmatchPenalty,
	gint gapOpeningPenalty,
	gint gapExtensionPenalty,
	gboolean freeLeftGapsForX,
	gboolean freeLeftGapsForY,
	gboolean freeRightGapsForX,
	gboolean freeRightGapsForY,
	gint (*substitutionMatrix)[26]
);

KBandOptions* KBandOptions_new(
	gint kInitValue,
	gint kExtensionValue
);

#endif