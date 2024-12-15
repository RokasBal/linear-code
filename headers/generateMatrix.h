#ifndef GENERATEMATRIX_H
#define GENERATEMATRIX_H

// File used for generating matrices needed for encoding / decoding

#include "matrix.h"

Matrix generateRandomMatrix(int k, int n);         // Generates a random matrix that begins with an identity matrix
Matrix generateUserMatrix(int k, int n);           // Generates a matrix based on user input that begins with an identity matrix
Matrix generateParityMatrix(const Matrix& matrix); // Generates a parity matrix based on the generative matrix

#endif // GENERATEMATRIX_H