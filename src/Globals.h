#include "Island.h"
#include "SubstitutionMatrixes.h"

#ifndef GLOBALS_H
#define GLOBALS_H

gchar codonToOneLetter(gchar[3]);
gchar* sequenceFromFile(gchar *, gulong *); 
//Assumming both chars ar capital
gint valueOfMatrix(gint matrix[26][26], gchar first, gchar second);
gchar* APP_SEQUENCE_TYPE(gint stype);
gint createBirdWatchGraphNW(Island* island, gint rows, gint cols, gboolean drawEachPoint);
gint createBirdWatchGraphSW(Island** islands, gint rows, gint cols, gboolean drawEachPoint);

#endif
