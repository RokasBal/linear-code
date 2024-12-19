#include "../headers/decode.h"
#include "../headers/calculations.h"
#include <stdexcept>
#include <chrono>
#include <iostream>

std::vector<uint8_t> decodeMessage(const Matrix& H, const std::vector<uint8_t>& receivedMessage, const syndromesTable syndromes, int n, int k) {
    size_t encodeLength = n - k; // Get the length of the message part that was added during encoding
    Vec message = receivedMessage;

    Matrix syndrome;
    Matrix messageT;
    Vec syndromeVector;
    Matrix tempSyndrome;
    Matrix tempPartT;
    Vec tempSyndromeVector;
    Vec tempPart;
    
    // Create a map for quick syndrome lookup
    // std::unordered_map<Vec, int, VectorHash> syndromeMap;
    // for (const auto& [w, s] : syndromes) {
    //     syndromeMap[s] = w;
    // }

    int cosetWeight = -1; // Initialize the coset weight to -1 to allow checking for problems with syndrome generation
    for (int j = 0; j < n; ++j) {
        // Calculate syndrome for message or it's part
        transposeVector(message, messageT);
        multiplyMatrices(H, messageT, syndrome);
        transposeMatrixToVector(syndrome, syndromeVector);

        // Find the coresponding weight for the syndrome based on generated syndromes
        auto it = syndromes.find(syndromeVector);
        int weight = (it != syndromes.end()) ? it->second : -1;
        
        if (weight == 0) {
            // If found 0 weight coset - end decoding, errors within part are corrected
            cosetWeight = 0;
            break;
        } else {
            // Otherwise continue finding the lowest weight, flipping bits in the part
            tempPart = message;
            tempPart[j] ^= 1; // Flip the bit at position j

            // Calculate syndrome for part with flipped bit
            transposeVector(tempPart, tempPartT);
            multiplyMatrices(H, tempPartT, tempSyndrome);
            transposeMatrixToVector(tempSyndrome, tempSyndromeVector);

            auto tempIt = syndromes.find(tempSyndromeVector);
            int tempWeight = (tempIt != syndromes.end()) ? tempIt->second : -1;
            
            if (tempWeight < weight) {
                // If the weight is lower with the flipped bit - set the message to the new part and continue decoding
                message = tempPart;
                weight = tempWeight;
                cosetWeight = tempWeight;
            } // Otherwise continue decoding without the flipped bit
        }
    }

    // After decoding loop is completed, restore the message to original length
    // This is able to be done due to the generating matrix starting with an identity matrix
    message.resize(message.size() - encodeLength);
    return message;
}