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
// 
gchar** afterMatrixFilling_findSemiGlobalAlignment(Cell*** matrix,
                                                  gchar* upSequence,
                                                  gchar* leftSequence,
                                                  gint n,
                                                  gint m,
                                                  gint freeRightGapsUp,
                                                  gint freeRightGapsLeft);

#endif
