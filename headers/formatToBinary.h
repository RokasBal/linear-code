#ifndef FORMATTOBINARY_H
#define FORMATTOBINARY_H

#include "matrix.h"
#include <string>
#include <cstdint>
#include <fstream>

Matrix stringToBinary(const std::string& message, int length);
std::string binaryToString(const Matrix& binaryParts);
Matrix bmpToBinary(const std::string& filePath, int length);
void binaryToBmp(const Matrix& binaryParts, const std::string& filePath, int width, int height, int channels);

#endif // FORMATTOBINARY_H