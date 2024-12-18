#ifndef CALCULATIONS_H
#define CALCULATIONS_H

// File used for handling mathematical calculations with matrices and vectors

#include "matrix.h"

// Encodes the message or it's part using the generative matrix
Vec encodeMessage(const Matrix& G, const Vec& message, int k, int* addedBits);

// Functions used for transposing matrices and vectors
void transposeMatrix(const Matrix& matrix, Matrix& result);
void transposeVector(const Vec& vector, Matrix& result);
void transposeMatrixToVector(const Matrix& matrix, Vec& result);

// Functions used for multiplying matrices and vectors
void multiplyMatrixVector(const Matrix& matrix, const Vec& vector, Vec& result) ;
void multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2, Matrix& result);

std::vector<uint8_t> introduceErrors(const Vec& vector, double errorRate);

#endif // CALCULATIONS_H