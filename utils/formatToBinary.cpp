#include "../headers/formatToBinary.h"
#include <vector>

std::vector<uint8_t> stringToBinary(const std::string& message) {
    std::vector<uint8_t> binaryVector;
    
    for (char c : message) {
        for (int i = 7; i >= 0; --i) {
            uint8_t bit = (c >> i) & 1;
            binaryVector.push_back(bit);
        }
    }
    
    return binaryVector;
}

std::string binaryToString(const std::vector<uint8_t>& binaryVector) {
    std::string message;
    size_t size = binaryVector.size();
    size_t partCount = size / 8;
    
    for (size_t i = 0; i < partCount; ++i) {
        char c = 0;
        for (size_t j = 0; j < 8; ++j) {
            c |= binaryVector[i * 8 + j] << (7 - j);
        }
        message.push_back(c);
    }
    
    return message;
}