#include "../headers/formatToBinary.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "../headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../headers/stb_image_write.h"

Matrix stringToBinary(const std::string& message, int length) {
    Matrix binaryParts;
    Vec binaryVector;

    for (char c : message) {
        for (int i = 7; i >= 0; --i) {
            uint8_t bit = (c >> i) & 1;
            binaryVector.push_back(bit);
        }
    }

    size_t messageSize = binaryVector.size();
    size_t partCount = (messageSize + length - 1) / length;

    for (size_t i = 0; i < partCount; ++i) {
        Vec part;
        size_t partStart = i * length;
        size_t partEnd = std::min(partStart + length, messageSize);
        
        part.assign(binaryVector.begin() + partStart, binaryVector.begin() + partEnd);
        binaryParts.push_back(part);
    }

    return binaryParts;
}

std::string binaryToString(const Matrix& binaryParts) {
    std::string message;
    Vec binaryVector;

    for (const auto& part : binaryParts) {
        binaryVector.insert(binaryVector.end(), part.begin(), part.end());
    }

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

Matrix bmpToBinary(const std::string& filePath, int length) {
    int width, height, channels;
    uint8_t* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        throw std::runtime_error("Klaida užkraunant nuotrauką.");
    }

    Vec binaryVector;
    for (int i = 0; i < width * height * channels; ++i) {
        for (int bit = 7; bit >= 0; --bit) {
            binaryVector.push_back((data[i] >> bit) & 1);
        }
    }

    stbi_image_free(data);

    Matrix binaryParts;
    size_t messageSize = binaryVector.size();
    size_t partCount = (messageSize + length - 1) / length;

    for (size_t i = 0; i < partCount; ++i) {
        Vec part;
        size_t partStart = i * length;
        size_t partEnd = std::min(partStart + length, messageSize);
        
        part.assign(binaryVector.begin() + partStart, binaryVector.begin() + partEnd);
        binaryParts.push_back(part);
    }

    return binaryParts;
}

void binaryToBmp(const Matrix& binaryParts, const std::string& filePath, int width, int height, int channels) {
    Vec binaryVector;

    for (const auto& part : binaryParts) {
        binaryVector.insert(binaryVector.end(), part.begin(), part.end());
    }

    Vec imageData(width * height * channels);
    for (size_t i = 0; i < imageData.size(); ++i) {
        uint8_t value = 0;
        for (int bit = 0; bit < 8; ++bit) {
            value |= binaryVector[i * 8 + bit] << (7 - bit);
        }
        imageData[i] = value;
    }

    stbi_write_bmp(filePath.c_str(), width, height, channels, imageData.data());
}