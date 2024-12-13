#include "headers/generateMatrix.h"
#include "headers/matrixMath.h"
#include "headers/visualization.h"
#include "headers/generateSyndromes.h"
#include "headers/decode.h"
#include "headers/formatToBinary.h"
#include "headers/userInterface.h"  
#include <iostream>

int main() {
    initializeInterface();
    // int n = 32; // provided code length (column count)
    // int k = 10; // provided code dimension (row count)
    // TODO - ADD CHECKS SO THAT k <= n
    // TODO - ADD CHECKS TO MAKE SURE USER INPUT VECTOR IS OF SIZE k
    // double errorRate = 0.15;

    // std::string messageToEncode = "Hello, World!";

    // Matrix G = generateRandomMatrix(k, n);
    // Matrix H = generateParityMatrix(G);
    // std:: cout << "Generator matrix G:" << std::endl;
    // displayMatrix(G);
    // std:: cout << "Parity matrix H:" << std::endl;
    // displayMatrix(H);

    // // std::vector<uint8_t> message = {1, 0, 1};
    // std::vector<uint8_t> message = stringToBinary(messageToEncode);
    // std::cout << "Message string: " << messageToEncode << std::endl;
    // std::cout << "Message vector m:" << std::endl;
    // displayVector(message);
    
    // std::cout << "Encoding message:" << std::endl;
    // std::vector<uint8_t> encodedMessage = encodeMessage(G, message, k);
    // std::cout << "Encoded vector c:" << std::endl;
    // displayVector(encodedMessage);

    // std::vector<uint8_t> receivedMessage = introduceErrors(encodedMessage, errorRate);
    // std::cout << "Received vector r:" << std::endl;
    // displayVectorDifferences(encodedMessage, receivedMessage);

    // std::cout << "Generating syndromes:" << std::endl;
    // syndromesTable syndromes = generateSyndromes(n, k, H);
    // // std::cout << "Syndromes:" << std::endl;
    // // displaySyndromes(syndromes);

    // std::cout << "Decoding message:" << std::endl;
    // std::vector<uint8_t> decodedMessage = decodeMessage(H, receivedMessage, syndromes, n, k);
    // std::cout << "Decoded vector m':" << std::endl;
    // displayVector(decodedMessage);

    // std::cout <<"Fixed erros: " <<getFixedErrorCount(encodedMessage, receivedMessage) << std::endl;

    // std::string decodedMessageString = binaryToString(decodedMessage);
    // std::cout << "Decoded string: " << decodedMessageString << std::endl;

    // std::string unencodedMessage = binaryToString(introduceErrors(message, errorRate));
    // std::cout << "Unencoded string: " << unencodedMessage << std::endl;

    // do {
    //     std::cout << "Would you like to try again? (y/n): ";
    //     char response;
    //     std::cin >> response;
    //     if (response == 'n') {
    //         break;
    //     } else if (response == 'y') {
    //         encodedMessage = encodeMessage(G, message, k);
    //         receivedMessage = introduceErrors(encodedMessage, errorRate);
    //         decodedMessage = decodeMessage(H, receivedMessage, syndromes, n, k);
    //         decodedMessageString = binaryToString(decodedMessage);
    //         unencodedMessage = binaryToString(introduceErrors(message, errorRate));
    //         displayVectorDifferences(encodedMessage, receivedMessage);
    //         std::cout <<"Fixed erros: " <<getFixedErrorCount(encodedMessage, receivedMessage) << std::endl;
    //         std::cout << "Decoded string: " << decodedMessageString << std::endl;
    //         std::cout << "Unencoded string: " << unencodedMessage << std::endl;
    //     }
    // } while (true);

    // return 0;
}