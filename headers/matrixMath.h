#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include "matrix.h"
std::vector<uint8_t> encodeMessage(const Matrix& G, const std::vector<uint8_t>& message, int k);

Matrix transposeVector(const std::vector<uint8_t>& vector);
Matrix transposeMatrix(const Matrix& matrix);

std::vector<uint8_t> multiplyMatrixVector(const Matrix& matrix, const std::vector<uint8_t>& vector);
Matrix multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2);

std::vector<uint8_t> introduceErrors(const std::vector<uint8_t>& vector, double errorRate);

#endif // MATRIXMATH_H