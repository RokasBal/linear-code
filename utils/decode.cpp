#include "../headers/decode.h"
#include "../headers/calculations.h"
#include <stdexcept>
#include <iostream>
#include <print>

std::vector<uint8_t> decodeMessage(const Matrix& H, const std::vector<uint8_t>& receivedMessage, syndromesTable syndromes, int n, int k) {
    // size_t partSize = n;
    // size_t messageSize = receivedMessage.size();
    // size_t partCount = (messageSize + partSize - 1) / partSize;
    size_t encodeLength = n - k;
    // std::vector<uint8_t> decodedMessage;

    // for (size_t i = 0; i < partCount; ++i) {
    //     // std::cout << "Checking part " << (i+1) << std::endl;
    //     std::vector<uint8_t> part;
    //     size_t partStart = i * n;
    //     size_t partEnd = std::min(partStart + partSize, messageSize);
        
    //     part.assign(receivedMessage.begin() + partStart, receivedMessage.begin() + partEnd);

    //     int cosetWeight = -1;
    //     for (int j = 0; j < partSize; ++j) {
    //         // std::cout << "  Checking part " << (i+1) << " checking vector element " << j << std::endl;
    //         Matrix syndrome = multiplyMatrices(H, transposeVector(part));
    //         std::vector<uint8_t> syndromeVector = transposeMatrix(syndrome)[0];

    //         int weight = -1;
    //         for (const auto& [w, s] : syndromes) {
    //             if (s == syndromeVector) {
    //                 weight = w;
    //                 break;
    //             }
    //         }
            
    //         if (weight == -1) {
    //             throw std::runtime_error("Syndrome not found in table");
    //         } else if (weight == 0) {
    //             // std::cout << "  Found coset with weight 0, moving on to part " << (i+2) << std::endl;
    //             cosetWeight = 0;
    //             break;
    //         } else {
    //             std::vector<uint8_t> tempPart = part;
    //             tempPart[j] ^= 1; // Flip the bit
    //             Matrix tempSyndrome = multiplyMatrices(H, transposeVector(tempPart));
    //             std::vector<uint8_t> tempSyndromeVector = transposeMatrix(tempSyndrome)[0];
    //             int tempWeight = -1;
    //             for (const auto& [w, s] : syndromes) {
    //                 if (s == tempSyndromeVector) {
    //                     tempWeight = w;
    //                     break;
    //                 }
    //             }
    //             if (tempWeight == -1) {
    //                 throw std::runtime_error("Syndrome not found in table");
    //             } else if (tempWeight == 0) {
    //                 part = tempPart;
    //                 // std::cout << "  Found coset with weight 0 after flipping bit, moving on to part " << (i+2) << std::endl;
    //                 break;
    //             } else if (tempWeight < weight) {
    //                 part = tempPart;
    //                 weight = tempWeight;
    //                 cosetWeight = tempWeight;
    //                 // std::cout << "  Found coset with lower weight, moving on to part " << (i+2) << std::endl;
    //             } else {
    //                 // std::cout << "      Coset with lower weight not found, moving on to vector element " << (j+1) << " current weight: " << weight << std::endl;
    //             }
    //         }
    //     }
        
    //     decodedMessage.insert(decodedMessage.end(), part.begin(), part.end() - encodeLength);
    // }

    // return decodedMessage;

    Vec message = receivedMessage;

    int cosetWeight = -1;
    for (int j = 0; j < n; ++j) {
        // std::cout << "  Checking part " << (i+1) << " checking vector element " << j << std::endl;
        Matrix syndrome = multiplyMatrices(H, transposeVector(message));
        std::vector<uint8_t> syndromeVector = transposeMatrix(syndrome)[0];

        int weight = -1;
        for (const auto& [w, s] : syndromes) {
            if (s == syndromeVector) {
                weight = w;
                break;
            }
        }
        
        if (weight == -1) {
            throw std::runtime_error("Syndrome not found in table");
        } else if (weight == 0) {
            // std::cout << "  Found coset with weight 0, moving on to part " << (i+2) << std::endl;
            cosetWeight = 0;
            break;
        } else {
            std::vector<uint8_t> tempPart = message;
            tempPart[j] ^= 1; // Flip the bit
            Matrix tempSyndrome = multiplyMatrices(H, transposeVector(tempPart));
            std::vector<uint8_t> tempSyndromeVector = transposeMatrix(tempSyndrome)[0];
            int tempWeight = -1;
            for (const auto& [w, s] : syndromes) {
                if (s == tempSyndromeVector) {
                    tempWeight = w;
                    break;
                }
            }
            if (tempWeight == -1) {
                throw std::runtime_error("Syndrome not found in table");
            } else if (tempWeight == 0) {
                message = tempPart;
                // std::cout << "  Found coset with weight 0 after flipping bit, moving on to part " << std::endl;
                break;
            } else if (tempWeight < weight) {
                message = tempPart;
                weight = tempWeight;
                cosetWeight = tempWeight;
                // std::cout << "  Found coset with lower weight, moving on to part " << std::endl;
            } else {
                // std::cout << "      Coset with lower weight not found, moving on to vector element " << " current weight: " << weight << std::endl;
            }
        }
    }

    message.resize(message.size() - encodeLength);
    return message;
}