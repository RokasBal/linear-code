#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "matrix.h"
#include "matrixMath.h"
#include "generateMatrix.h"
#include "visualization.h"
#include "generateSyndromes.h"
#include "formatToBinary.h"
#include "decode.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <filesystem>

void initializeInterface();
void getGenerativeMatrixSize(int* n, int* k);
Matrix getGenerativeMatrix(int k, int n);
int getUserInput(int minInput, int maxInput, std::string message);
float getUserInputFloat(float minInput, float maxInput, std::string message);
void showParameters(Matrix G, Matrix H, int n, int k, double errorRate, syndromesTable syndromes);
void printProgramParameters(int n, int k, double errorRate);
void updateParameters(int* n, int* k, double* errorRate, Matrix* G, Matrix* H, syndromesTable* syndromes);
void sendMessage(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate);
void sendVector(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate);
void sendImage(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate);
std::vector<uint8_t> getUserVector(int k, int id);
void clearConsole();
bool getYesNoInput();
void openImage(const std::string& filePath);

#endif // USERINTERFACE_H