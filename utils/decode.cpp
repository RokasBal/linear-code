#include "../headers/decode.h"
#include "../headers/calculations.h"
#include <stdexcept>
#include <iostream>

std::vector<uint8_t> decodeMessage(const Matrix& H, const std::vector<uint8_t>& receivedMessage, syndromesTable syndromes, int n, int k) {
    size_t encodeLength = n - k; // Get the length of the message part that was added during encoding
    Vec message = receivedMessage;

    int cosetWeight = -1; // Initialize the coset weight to -1 to allow checking for problems with syndrome generation
    for (int j = 0; j < n; ++j) {
        // Calculate syndrome for message or it's part
        Matrix syndrome;
        Matrix messageT;
        transposeVector(message, messageT);
        multiplyMatrices(H, messageT, syndrome);
        Vec syndromeVector;
        transposeMatrixToVector(syndrome, syndromeVector);

        // Find the coresponding weight for the syndrome based on generated syndromes
        int weight = -1; // Set the weight to -1 to allow checking for problems with syndrome generation
        for (const auto& [w, s] : syndromes) {
            if (s == syndromeVector) {
                // Once corresponding syndrome is found - set the weight and stop the loop
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
            tempPart[j] ^= 1; // Flip the bit at position j

            // Calculate syndrome for part with flipped bit
            Matrix tempSyndrome;
            Matrix tempPartT;
            transposeVector(tempPart, tempPartT);
            multiplyMatrices(H, tempPartT, tempSyndrome);
            Vec tempSyndromeVector;
            transposeMatrixToVector(tempSyndrome, tempSyndromeVector);

            int tempWeight = -1; // Initialize the weight to -1 to allow checking for problems with syndrome generation
            // Get weight corresponding to the syndrome
            for (const auto& [w, s] : syndromes) {
                if (s == tempSyndromeVector) {
                    // Once corresponding syndrome is found - set the weight and stop the loop
                    tempWeight = w;
                    break;
                }
            }

            if (tempWeight == -1) {
                // Throw error if syndrome is not found
                // If this happens - syndrome generation is not working correctly
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
                continue;
            }
        }
    }

    // After decoding loop is completed, restore the message to original length
    // This is able to be done due to the generating matrix starting with an identity matrix
    message.resize(message.size() - encodeLength);
    return message;
}