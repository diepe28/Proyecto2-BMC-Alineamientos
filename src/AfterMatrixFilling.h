#include "Globals.h"

#ifndef AFTER_MATRIX_FILLING_H
#define AFTER_MATRIX_FILLING_H

//given the matrix of global alignment (NW) and the the sequences
//it finds the alignment of the two sequences and returns it in a two
//length vector of char*. Seq1 = up, N-length, seq2 = left, M-length 
char** afterMatrixFilling_findGlobalAlignment(Cell***, char* , char*, int, int);

#endif
