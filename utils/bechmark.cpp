#include "../headers/benchmark.h"

void benchmark(int minN, int maxN, int minK, int maxK, double errorRate) {
    int bits = 0;
    std::vector<std::tuple<int, int, double, double, int>> results;

    for (int n = minN; n <= maxN; ++n) {
        for (int k = minK; k <= std::min(n, maxK); ++k) {
            if (n == k) {
                continue; 
            }
            try {
                Matrix G = generateRandomMatrix(k, n);
                Matrix H = generateParityMatrix(G);
                // std::cout <<"Generating syndromes for n=" << n << ", k=" << k << std::endl;
                auto startSyndrome = std::chrono::high_resolution_clock::now();
                syndromesTable syndromes = generateSyndromes(n, k, H, false);
                auto endSyndrome = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> syndromeDuration = endSyndrome - startSyndrome;
                // std::cout << "Generating message for n=" << n << ", k=" << k << std::endl;

                Vec message = generateRandomVector(k);
                Vec encodedMessage;
                Vec receivedMessage;
                Vec decodedMessage;

                // std::cout << "Matrix G dimensions: " << G.size() << "x" << (G.empty() ? 0 : G[0].size()) << std::endl;
                // std::cout << "Matrix H dimensions: " << H.size() << "x" << (H.empty() ? 0 : H[0].size()) << std::endl;
                // std::cout << "Message vector size: " << message.size() << std::endl;

                auto start = std::chrono::high_resolution_clock::now();
                encodedMessage = encodeMessage(G, message, k, &bits);
                receivedMessage = introduceErrors(encodedMessage, errorRate);
                decodedMessage = decodeMessage(H, receivedMessage, syndromes, n, k);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> encodeDuration = end - start;

                int errorsLeft = getIntroducedErrorCount(message, decodedMessage);

                results.emplace_back(n, k, encodeDuration.count(), syndromeDuration.count(), errorsLeft);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << " for n=" << n << ", k=" << k << std::endl;
            }
        }
    }

    std::cout << "Benchmark Results:\n";
    std::cout << "n\tk\tEncode Time (s)\tSyndrome Time (s)\tErrors after decode\n";
    for (const auto& result : results) {
        int n, k, errorsFixed;
        double encodeTime, syndromeTime;
        std::tie(n, k, encodeTime, syndromeTime, errorsFixed) = result;
        std::cout << n << "\t" << k << "\t" << std::fixed << std::setprecision(7) << encodeTime << "\t" << syndromeTime << "\t" << errorsFixed << "\n";
    }
}