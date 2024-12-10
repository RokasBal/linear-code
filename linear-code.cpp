#include "headers/matrixGen.h"
#include "headers/matrixMath.h"
#include "headers/visualization.h"
#include "headers/generateSyndromes.h"
#include "headers/decode.h"
#include "headers/formatToBinary.h"
#include <iostream>

int main() {
    int n = 25; // provided code length (column count)
    int k = 5; // provided code dimension (row count)
    // TODO - ADD CHECKS SO THAT k <= n
    // TODO - ADD CHECKS TO MAKE SURE USER INPUT VECTOR IS OF SIZE k
    double errorRate = 0.03;

    std::string messageToEncode = "Hello, World!";

    Matrix G = generateRandomMatrix(k, n);
    Matrix H = generateParityMatrix(G);
    std:: cout << "Generator matrix G:" << std::endl;
    displayMatrix(G);
    std:: cout << "Parity matrix H:" << std::endl;
    displayMatrix(H);

    // std::vector<uint8_t> message = {1, 0, 1};
    std::vector<uint8_t> message = stringToBinary(messageToEncode);
    std::cout << "Message string: " << messageToEncode << std::endl;
    std::cout << "Message vector m:" << std::endl;
    displayVector(message);
    std::vector<uint8_t> encodedMessage = encodeMessage(G, message, k);
    std::cout << "Encoded vector c:" << std::endl;
    displayVector(encodedMessage);

    std::vector<uint8_t> receivedMessage = introduceErrors(encodedMessage, errorRate);
    std::cout << "Received vector r:" << std::endl;
    displayVectorDifferences(encodedMessage, receivedMessage);

    syndromesTable syndromes = generateSyndromes(n, k, H);
    std::cout << "Syndromes:" << std::endl;
    displaySyndromes(syndromes);

    std::vector<uint8_t> decodedMessage = decodeMessage(H, receivedMessage, syndromes, n, k);
    std::cout << "Decoded vector m':" << std::endl;
    displayVector(decodedMessage);

    std::string decodedMessageString = binaryToString(decodedMessage);
    std::cout << "Decoded string: " << decodedMessageString << std::endl;

    std::string unencodedMessage = binaryToString(introduceErrors(message, errorRate));
    std::cout << "Unencoded string: " << unencodedMessage << std::endl;

    return 0;
}