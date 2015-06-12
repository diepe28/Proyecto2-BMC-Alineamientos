#include "Island.h"
#include "SubstitutionMatrixes.h"

#ifndef GLOBALS_H
#define GLOBALS_H

gchar codonToOneLetter(gchar[3]);
gchar* sequenceFromFile(gchar *, gulong *); 
//Assumming both chars ar capital
gint valueOfMatrix(gint matrix[27][27], gchar first, gchar second);
gchar* APP_SEQUENCE_TYPE(gint stype);
gint createBirdWatchGraphNW(Island* island, gint rows, gint cols);
gint createBirdWatchGraphSW(GSList* islands, gint rows, gint cols);
gchar* dnaToProtein(gchar* dna);
gchar* threeLetterCodedProteinToOneLetterCodedProtein(const gchar* seq, gint seqSize);
#endif
