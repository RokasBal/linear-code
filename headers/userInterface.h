#ifndef USERINTERFACE_H
#define USERINTERFACE_H

// File used for handling the user interface of the program

#include "matrix.h"
#include "calculations.h"
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
#include <algorithm>

// Start the interface for the program
void initializeInterface();

// Functions handling user input
int getUserInput(int minInput, int maxInput, std::string message);
float getUserInputFloat(float minInput, float maxInput, std::string message);

// Functions for getting matrix G
void getGenerativeMatrixSize(int* n, int* k);
Matrix getGenerativeMatrix(int k, int n);

// Functions for visualizing and updating parameters
void showParameters(Matrix G, Matrix H, int n, int k, double errorRate, syndromesTable syndromes);          // Used to show the selection menu for parameter visualization
void printProgramParameters(int n, int k, double errorRate);                                                // Used to visualize selected parameters
void updateParameters(int* n, int* k, double* errorRate, Matrix* G, Matrix* H, syndromesTable* syndromes);  // Used to update the selected parameters
void setProgramSettings();                                                                                  // Used to set separate program settings that are not related to the encode / decode parameters

// Functions for sending the 3 types of messages
void sendMessage(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate);
void sendVector(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate);
void sendImage(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate);

// Helper functions
Vec getUserVector(int k, int id);  
bool getYesNoInput();                               // Gets user input for yes / no questions in 'y' or 'n' format
void openImage(const std::string& filePath);        // Opens the image in image viewer

#endif // USERINTERFACE_H