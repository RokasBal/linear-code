#include "headers/userInterface.h"  
#include "headers/benchmark.h"
#include <iostream>
#include <clocale>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // Uncomment for compile
    // std::setlocale(LC_ALL, "lt_LT.UTF-8"); 

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // std::cout << "benchmark 1 - 0.01" << std::endl;
    // benchmark(1, 25, 1, 25, 0.01);
    // std::cout << "benchmark 1 - 0.025" << std::endl;
    // benchmark(1, 25, 1, 25, 0.025);
    // std::cout << "benchmark 1 - 0.05" << std::endl;
    // benchmark(1, 25, 1, 25, 0.05);
    // std::cout << "benchmark 2 - 0.1" << std::endl;
    // benchmark(1, 25, 1, 25, 0.1);
    // std::cout << "benchmark 2 - 0.15" << std::endl;
    // benchmark(1, 25, 1, 25, 0.15);
    // std::cout << "benchmark 2 - 0.25" << std::endl;
    // benchmark(1, 25, 1, 25, 0.25);
    // std::cout << "benchmark 2 - 0.4" << std::endl;
    // benchmark(1, 25, 1, 25, 0.4);
    // std::cout << "benchmark 2 - 0.5" << std::endl;
    // benchmark(1, 25, 1, 25, 0.5);
    initializeInterface();

    return 0;
}