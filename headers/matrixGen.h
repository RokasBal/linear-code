#ifndef MATRIXGEN_H
#define MATRIXGEN_H

#include "matrix.h"

Matrix generateRandomMatrix(int k, int n);
Matrix generateParityMatrix(const Matrix& matrix);

#endif // MATRIXGEN_H