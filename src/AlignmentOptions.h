
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
	GHashTable* substitutionMatrix;
} ScoringOptions;

typedef struct {
	gint kInitValue;
	gint kExtensionValue;
} KBandOptions;

#endif