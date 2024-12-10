#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "matrix.h"

void displayMatrix(const Matrix& matrix);
void displayVector(const std::vector<uint8_t>& vector);
void displaySyndromes(const std::vector<std::pair<int, std::vector<uint8_t>>>& syndromes);
void displayVectorDifferences(const std::vector<uint8_t>& vec1, const std::vector<uint8_t>& vec2);

#endif // VISUALIZATION_H
