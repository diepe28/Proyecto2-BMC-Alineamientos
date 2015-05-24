#include "Globals.h"

#ifndef AFTER_MATRIX_FILLING_H
#define AFTER_MATRIX_FILLING_H

//given the matrix of global alignment (NW) and the the sequences
//it finds the alignment of the two sequences and returns it in a two
//length vector of char*. Seq1 = up, N-length, seq2 = left, M-length
gchar** afterMatrixFilling_findGlobalAlignment(Cell*** matrix, 
                                              gchar* upSequence,
                                              gchar* leftSequence,
                                              gint n,
                                              gint m);

//given the matrix of global alignment (NW) and the the sequences
//it finds the alignment of the two sequences and returns it in a two
//length vector of char*. Seq1 = up, N-length, seq2 = left, M-length
gchar** afterMatrixFilling_findSemiGlobalAlignment(Cell*** matrix,
                                                  gchar* upSequence,
                                                  gchar* leftSequence,
                                                  gint n,
                                                  gint m,
                                                  gboolean freeRightGapsUp,
                                                  gboolean freeRightGapsLeft);

//given the matrix of local alignment (SW) and the the sequences
//it finds the islands with value >= minScore, within the matrix
//the matrix if modified with the cells that should be painted
//and a list of island is returned
GSList* afterMatrixFilling_findLocalAlignments(Cell*** matrix, 
                                              gchar* upSequence,
                                              gchar* leftSequence,
                                              gint n,
                                              gint m,
                                              gint minScore);

#endif
