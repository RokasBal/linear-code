#ifndef CALCULATIONS_H
#define CALCULATIONS_H

// File used for handling mathematical calculations with matrices and vectors

#include "matrix.h"

// Encodes the message or it's part using the generative matrix
Vec encodeMessage(const Matrix& G, const Vec& message, int k, int* addedBits);

// Functions used for transposing matrices and vectors
Matrix transposeVector(const Vec& vector);
Matrix transposeMatrix(const Matrix& matrix);

// Functions used for multiplying matrices and vectors
Vec multiplyMatrixVector(const Matrix& matrix, const Vec& vector);
Matrix multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2);

std::vector<uint8_t> introduceErrors(const Vec& vector, double errorRate);

#endif // CALCULATIONS_H