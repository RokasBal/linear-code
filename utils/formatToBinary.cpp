#include "../headers/formatToBinary.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "../headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../headers/stb_image_write.h"

Matrix stringToBinary(const std::string& message, int length) {
    Matrix binaryParts;
    Vec binaryVector;

    for (char c : message) { // Go over each character in the message
        for (int i = 7; i >= 0; --i) { // Go over each bit in the character
            uint8_t bit = (c >> i) & 1;  // Get the bit
            binaryVector.push_back(bit); // Add the bit to the binary vector
        }
    }

    // Get the amount of parts that the message should be split into
    size_t messageSize = binaryVector.size();
    size_t partCount = (messageSize + length - 1) / length; // Account for if the last part is not full

    // Split the binary vector into parts
    for (size_t i = 0; i < partCount; ++i) {
        Vec part;
        size_t partStart = i * length;
        size_t partEnd = std::min(partStart + length, messageSize);
        
        part.assign(binaryVector.begin() + partStart, binaryVector.begin() + partEnd);
        binaryParts.push_back(part);
    }

    // Return as parts matrix
    return binaryParts;
}

std::string binaryToString(const Matrix& binaryParts) {
    std::string message;
    Vec binaryVector;

    // Reconstruct the parts into a single vector
    for (const auto& part : binaryParts) { // Go over each part in the binary parts
        binaryVector.insert(binaryVector.end(), part.begin(), part.end()); // Add the part to the binary vector
    }

    // Get the amount of bits in the binary vector
    size_t size = binaryVector.size();
    // Get the amount of parts in the binary vector, splitting it into bytes
    size_t partCount = size / 8;

    for (size_t i = 0; i < partCount; ++i) {  // Go over each byte
        char c = 0;
        for (size_t j = 0; j < 8; ++j) { // Go over each bit in the part
            c |= binaryVector[i * 8 + j] << (7 - j); // Add the bit to the character
        }
        message.push_back(c); // Add the character to the message
    }

    return message;
}

Matrix bmpToBinary(const std::string& filePath, int length) {
    int width, height, channels;
    uint8_t* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0); // Load the image using stb_image third party library

    // Check if the image was loaded correctly
    if (data == nullptr) {
        throw std::runtime_error("Klaida užkraunant nuotrauką.");
    }

    // Convert the image data to binary
    Vec binaryVector;
    for (int i = 0; i < width * height * channels; ++i) {
        for (int bit = 7; bit >= 0; --bit) {
            binaryVector.push_back((data[i] >> bit) & 1);
        }
    }

    stbi_image_free(data); // Free the image data

    // Calculate the amount of parts the binary vector should be split into
    Matrix binaryParts;
    size_t messageSize = binaryVector.size();
    size_t partCount = (messageSize + length - 1) / length;

    // Split the binary vector into parts
    for (size_t i = 0; i < partCount; ++i) { // Go over each part
        Vec part;
        size_t partStart = i * length;
        size_t partEnd = std::min(partStart + length, messageSize);
        
        part.assign(binaryVector.begin() + partStart, binaryVector.begin() + partEnd); // Add the part to the binary parts
        binaryParts.push_back(part);
    }

    return binaryParts;
}

void binaryToBmp(const Matrix& binaryParts, const std::string& filePath, int width, int height, int channels) {
    Vec binaryVector;

    // Reconstruct the binary parts into a single vector
    for (const auto& part : binaryParts) {
        binaryVector.insert(binaryVector.end(), part.begin(), part.end());
    }

    // Convert the binary vector to image data
    Vec imageData(width * height * channels);
    for (size_t i = 0; i < imageData.size(); ++i) {
        uint8_t value = 0;
        for (int bit = 0; bit < 8; ++bit) { // Go over each bit
            value |= binaryVector[i * 8 + bit] << (7 - bit); // Add the bit to the value
        }
        imageData[i] = value;
    }

    stbi_write_bmp(filePath.c_str(), width, height, channels, imageData.data()); // Save the image using stb_image_write third party library
}