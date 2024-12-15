#include "../headers/decode.h"
#include "../headers/calculations.h"
#include <stdexcept>
#include <iostream>

std::vector<uint8_t> decodeMessage(const Matrix& H, const std::vector<uint8_t>& receivedMessage, syndromesTable syndromes, int n, int k) {
    size_t encodeLength = n - k;
    Vec message = receivedMessage;

    int cosetWeight = -1;
    for (int j = 0; j < n; ++j) {
        // Calculate syndrome for message or it's part
        Matrix syndrome = multiplyMatrices(H, transposeVector(message));
        std::vector<uint8_t> syndromeVector = transposeMatrix(syndrome)[0];

        // Find the coresponding weight for the syndrome based on generated syndromes
        int weight = -1;
        for (const auto& [w, s] : syndromes) {
            if (s == syndromeVector) {
                weight = w;
                break;
            }
        }
        
        if (weight == -1) {
            // Throw error if syndrome is not found
            // If this happens - syndrome generation is not working correctly
            throw std::runtime_error("Syndrome not found in table");
        } else if (weight == 0) {
            // If found 0 weight coset - end decoding, errors within part are corrected
            cosetWeight = 0;
            break;
        } else {
            // Otherwise continue finding the lowest weight, flipping bits in the part
            Vec tempPart = message;
            tempPart[j] ^= 1; // Flip the bit

            // Calculate syndrome for part with flipped bit
            Matrix tempSyndrome = multiplyMatrices(H, transposeVector(tempPart));
            Vec tempSyndromeVector = transposeMatrix(tempSyndrome)[0];
            int tempWeight = -1;
            // Get weight corresponding to the syndrome
            for (const auto& [w, s] : syndromes) {
                if (s == tempSyndromeVector) {
                    tempWeight = w;
                    break;
                }
            }
            if (tempWeight == -1) {
                throw std::runtime_error("Syndrome not found in table");
            } else if (tempWeight == 0) {
                // If weight 0 - errors are corrected, set message to new part with flipped bits, end decoding.
                message = tempPart;
                // std::cout << "  Found coset with weight 0 after flipping bit, moving on to part " << std::endl;
                break;
            } else if (tempWeight < weight) {
                // If the weight is lower with the flipped bit - set the message to the new part and continue decoding
                message = tempPart;
                weight = tempWeight;
                cosetWeight = tempWeight;
            } else {
                // Otherwise continue decoding without the flipped bit
            }
        }
    }

    // After decoding loop is completed, restore the message to original length
    // This is able to be done due to the generating matrix starting with an identity matrix
    message.resize(message.size() - encodeLength);
    return message;
}