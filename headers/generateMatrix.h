#ifndef GENERATEMATRIX_H
#define GENERATEMATRIX_H

#include "matrix.h"

Matrix generateRandomMatrix(int k, int n);
Matrix generateUserMatrix(int k, int n);
Matrix generateParityMatrix(const Matrix& matrix);

#endif // GENERATEMATRIX_H