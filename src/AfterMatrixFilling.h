#include "Globals.h"

#ifndef AFTER_MATRIX_FILLING_H
#define AFTER_MATRIX_FILLING_H

//given the matrix of global alignment (NW) and the the sequences
//it finds the alignment of the two sequences and returns and Island
// Seq1 = up, N-length, seq2 = left, M-length
Island* afterMatrixFilling_find_NW_Alignment(Cell*** matrix, 
                                              gchar* upSequence,
                                              gchar* leftSequence,
                                              gint rows,
                                              gint cols,
                                              gboolean freeRightGapsUp,
                                              gboolean freeRightGapsLeft,
                                              gboolean blockOfGaps);

//given the matrix of local alignment (SW) and the the sequences
//it finds the islands with value >= minScore, within the matrix
//the matrix if modified with the cells that should be painted
//and a list of island is returned
GSList* afterMatrixFilling_findLocalAlignments(Cell*** matrix, 
                                              gchar* upSequence,
                                              gchar* leftSequence,
                                              gint rows,
                                              gint cols,
                                              gint minScore);


#endif
