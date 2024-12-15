#include "headers/userInterface.h"  
#include "headers/benchmark.h"
#include <iostream>
#include <clocale>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    std::setlocale(LC_ALL, "lt_LT.UTF-8");

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    benchmark(1, 23, 1, 23, 0.1);
    // initializeInterface();

    return 0;
}