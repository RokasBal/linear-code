#include "headers/matrixGen.h"
#include <iostream>

// n - provided code length (column count)
// k - provided code dimension (row count)
int n, k;

int main() {
    n = 6;
    k = 2;
    Matrix testMatrix = generateRandomMatrix(k, n);
    Matrix parityMatrix = generateParityMatrix(testMatrix);
    std:: cout << "Generator matrix G:" << std::endl;
    displayMatrix(testMatrix);
    std:: cout << "Parity matrix H:" << std::endl;
    displayMatrix(parityMatrix);
    return 0;
}