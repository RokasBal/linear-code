#ifndef FORMATTOBINARY_H
#define FORMATTOBINARY_H

#include "matrix.h"
#include <string>

std::vector<uint8_t> stringToBinary(const std::string& message);
std::string binaryToString(const std::vector<uint8_t>& binaryVector);

#endif // FORMATTOBINARY_H