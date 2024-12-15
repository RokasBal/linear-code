#ifndef FORMATTOBINARY_H
#define FORMATTOBINARY_H

// File used for converting strings and images to binary and back

#include "matrix.h"
#include <string>
#include <cstdint>
#include <fstream>

// Functions for converting strings to binary and back
Matrix stringToBinary(const std::string& message, int length);
std::string binaryToString(const Matrix& binaryParts);

// Functions for converting images to binary and back
Matrix bmpToBinary(const std::string& filePath, int length);
void binaryToBmp(const Matrix& binaryParts, const std::string& filePath, int width, int height, int channels);

#endif // FORMATTOBINARY_H