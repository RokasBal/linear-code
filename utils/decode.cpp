#include "../headers/decode.h"
#include "../headers/matrixMath.h"
#include <stdexcept>
#include <iostream>
#include <print>

std::vector<uint8_t> decodeMessage(const Matrix& H, const std::vector<uint8_t>& receivedMessage, syndromesTable syndromes, int n, int k) {
    printf("Decoding message\n");

    size_t partSize = n;
    size_t messageSize = receivedMessage.size();
    size_t partCount = (messageSize + partSize - 1) / partSize;
    size_t encodeLength = n - k;
    std::vector<uint8_t> decodedMessage;

    printf("Part size: %d\n", partSize);

    for (size_t i = 0; i < partCount; ++i) {
        std::cout << "Checking part " << (i+1) << std::endl;
        std::vector<uint8_t> part;
        size_t partStart = i * n;
        size_t partEnd = std::min(partStart + partSize, messageSize);
        
        part.assign(receivedMessage.begin() + partStart, receivedMessage.begin() + partEnd);

        int cosetWeight;
        for (int j = 0; j < partSize; ++j) {
            std::cout << "  Checking part " << (i+1) <<" checking vector element " <<j <<std::endl;
            Matrix syndrome = multiplyMatrices(H, transposeVector(part));
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
                std::cout << "  Found coset with weight 0, moving on to part " <<(i+2) << std::endl;
                cosetWeight = 0;
                break;
            } else {
                std::vector<uint8_t> tempPart = part;
                if (tempPart[j] == 0) {
                    tempPart[j] = 1;
                } else {
                    tempPart[j] = 0;
                }
                Matrix tempSyndrome = multiplyMatrices(H, transposeVector(tempPart));
                std::vector<uint8_t> tempSyndromeVector = transposeMatrix(syndrome)[0];
                int tempWeight = -1;
                for (const auto& [w, s] : syndromes) {
                    if (s == tempSyndromeVector) {
                        tempWeight = w;
                        break;
                    }
                }
                if (tempWeight == -1) {
                    throw std::runtime_error("Syndrome not found in table");
                } else if (tempWeight < weight) {
                    part = tempPart;
                    weight = tempWeight;
                    std::cout << "  Found coset with lower weight, moving on to part " <<(i+2) << std::endl;
                } else {
                    std::cout << "      Coset with lower weight not found, moving on to vector element " <<(j+1) <<" current weight: " <<weight << std::endl;
                }
            }
        }
        
        decodedMessage.insert(decodedMessage.end(), part.begin(), part.end() - encodeLength);
    }

    return decodedMessage;
}