#include "../headers/userInterface.h"
#include "../headers/stb_image.h"
#include "../headers/stb_image_write.h"

#include <chrono> // for benchmarking

int maxSyndromeWeight = 25; // Maximum weight of a syndrome to calculate
int addedBits = 0;          // Number of bits added to the message for encoding
bool showProgress = false;  // Whether to show progress messages

void initializeInterface() {
    int n, k;
    double errorRate;
    
    // Start the interface, asking the user to input basic parameters
    clearConsole();
    printf("Sveiki!\n\n");
    printf("[1] Įvesti parametrus\n");
    printf("[2] Baigti darbą\n\n");
    int selection = getUserInput(1, 2, "Įveskite pasirinkimą: ");
    if (selection == 2) exit(0);

    // Set up generative matrix
    getGenerativeMatrixSize(&n, &k);
    Matrix G = getGenerativeMatrix(k, n);

    clearConsole();
    errorRate = getUserInputFloat(0, 1, "Įveskite klaidų darymo procentą (0-1): ");

    // Ask the user wether progress messages should be shown when sending text or images
    clearConsole();
    printf("Ar norite įjungti progreso žinutes? (Žinutės sulėtina programos veikimą) (y/n): ");
    showProgress = getYesNoInput();

    // Initialize syndromes and parity matrix
    clearConsole();
    printf("Programa ruošiama, tai gali užtrukti...\n");
    Matrix H = generateParityMatrix(G);
    syndromesTable syndromes = generateSyndromes(n, k, H, showProgress);

    // Start the main menu loop
    while (true) {
        clearConsole();

        printf("Pasiruošta darbui!\n\n");
        printf("[1] Siųsti vektorių\n");
        printf("[2] Siųsti tekstą\n");
        printf("[3] Siųsti nuotrauką\n");
        printf("[4] Peržiūrėti parametrus\n");
        printf("[5] Atnaujinti parametrus\n");
        printf("[6] Programos nustatymai\n");
        printf("[7] Baigti darbą\n\n");

        selection = getUserInput(1, 7, "Įveskite pasirinkimą: ");
        switch (selection) {
            case 1:
                sendVector(G, H, syndromes, n, k, errorRate);
                break;
            case 2:
                sendMessage(G, H, syndromes, n, k, errorRate);
                break;
            case 3:
                sendImage(G, H, syndromes, n, k, errorRate);
                break;
            case 4:
                showParameters(G, H, n, k, errorRate, syndromes);
                break;
            case 5:
                updateParameters(&n, &k, &errorRate, &G, &H, &syndromes);
                break;
            case 6:
                setProgramSettings();
                break;
            case 7:
                exit(0);
                break;
        }
    }
}

void sendVector(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate) {
    clearConsole();

    // Get the vector to send
    Vec message(k, 0); 
    message = getUserVector(k, 0);

    clearConsole();
    printf("Vektorius siunčiamas...\n");

    // Encode the vector and send it trough the channel
    Vec encodedMessage = encodeMessage(G, message, k, &addedBits);
    Vec receivedMessage = introduceErrors(encodedMessage, errorRate);

    // Print the result of sending the vector trough the channel
    clearConsole();
    printf("Originalus vektorius: \n");
    displayVector(message);
    printf("\nUžkoduotas vektorius: \n");
    displayVector(encodedMessage);
    printf("\nIš kanalo gautas vektorius: \n");
    displayVectorWithMistakes(encodedMessage, receivedMessage);
    printf("\nAr norite jį redaguoti? (y/n): ");
    // Allow the received vector to be edited
    if (getYesNoInput()) {
        clearConsole();
        printf("\nIš kanalo gautas vektorius: \n");
        displayVectorWithMistakes(encodedMessage, receivedMessage);
        receivedMessage = getUserVector(n, 1);
        printf("Dekoduojamas redaguotas vektorius...\n");
    } else {
        printf("Dekoduojamas gautas vektorius...\n");
    }
    
    // After getting the user's choice of what to do with the received vector proceed with decoding it
    Vec decodedMessage = decodeMessage(H, receivedMessage, syndromes, n, k);

    // Print out the full results
    clearConsole();
    printf("Orginalus vektorius: \n");
    displayVector(message);
    printf("\nUžkoduotas vektorius: \n");
    displayVector(encodedMessage);
    printf("\nIš kanalo gautas vektorius: \n");
    displayVectorWithMistakes(encodedMessage, receivedMessage);
    printf("\nDekoduotas vektorius: \n");
    displayVector(decodedMessage);

    // Allow the user to review the results and return when they want
    printf("\nSpauskite Enter norėdami sugrįžti...");
    std::cin.get();
    return;
}

void sendMessage(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate) {
    clearConsole();

    // Get the message to send
    std::string message;
    std::string line;
    printf("Įrašykite tekstą siuntimui: ");
    // Read multiple lines of input
    while (true) {
        std::getline(std::cin, line);
        if (line.empty()) {
            break;
        }
        message += line + "\n";
    }

    clearConsole();
    printf("Tekstas siunčiamas...\n");
    
    // Convert the message to binary, in the process splitting it into separate vectors
    Matrix binaryMessage = stringToBinary(message, k);
    
    // Set up the matrices for sending the message
    Matrix encodedMessage;
    Matrix receivedMessage;
    Matrix decodedMessage;
    Matrix unencodedMessageBinary;

    // As the message is smaller in length than an image, progress update interval is made smaller
    int progressUpdateInterval = 5; 
    size_t binaryMessageSize = binaryMessage.size();

    // Start the sending / decoding process
    auto startEncode = std::chrono::high_resolution_clock::now(); // Start timer for benchmarking / identifying how long decoding takes
    for (size_t i = 0; i < binaryMessageSize; ++i) {
        // If the user wants to see progress, show it as percentage of completion
        if (showProgress && i % progressUpdateInterval == 0) {
            printf("Apdorojama: %d%%\r", static_cast<int>(i * 100 / binaryMessageSize));
            fflush(stdout);
        }

        // Encode the vector part
        Vec encodedPart = encodeMessage(G, binaryMessage[i], k, &addedBits);
        encodedMessage.push_back(encodedPart);

        // Send vector part trough channel
        Vec receivedPart = introduceErrors(encodedPart, errorRate);
        receivedMessage.push_back(receivedPart);

        // Decode the received vector part
        Vec decodedPart = decodeMessage(H, receivedPart, syndromes, n, k);
        decodedMessage.push_back(decodedPart);

        // Send the same vector without using encoding to compare results
        Vec unencodedPart = introduceErrors(binaryMessage[i], errorRate);
        unencodedMessageBinary.push_back(unencodedPart);
    }

    // Remove the added bits from the last vector if they were added
    if (addedBits > 0 && !decodedMessage.empty()) {
        decodedMessage.back().resize(decodedMessage.back().size() - addedBits); // Restore to original size
        addedBits = 0; // Reset the added bits counter
    }

    // Combine vectors and format them from binary to string
    std::string decodedMessageString = binaryToString(decodedMessage);
    std::string unencodedMessage = binaryToString(unencodedMessageBinary);

    auto endEncode = std::chrono::high_resolution_clock::now(); // Ends timer for benchmarking, shows how long it took
    std::chrono::duration<double> encodeDuration = endEncode - startEncode; 

    // Show the results to user
    clearConsole();
    printf("Rezultatai:\n");
    printf("Išsiųsti bei dekoduoti užtruko %fs\n", encodeDuration.count()); // Shows how long the process took
    printf("Orginalus tekstas: %s\n", message.c_str()); // Show the text the user sent
    printf("Dekoduotas tekstas: %s\n", decodedMessageString.c_str()); // Show the text received using encoding / decoding
    printf("Tekstas gautas nenaudojant kodavimo: %s\n", unencodedMessage.c_str()); // Show the text received without encoding

    printf("\nSpauskite Enter norint sugrįžti...");
    std::cin.get();
    return;
}

void sendImage(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate) {
    clearConsole();

    // Set up needed variables
    std::string fileName;
    std::filesystem::path filePath;
    int width, height, channels;

    // Get the file from user, validating it
    while (true) {
        printf("Pridėkite nuotrauką į \"images\" aplankalą ir įveskite failo pavadinimą (pvz. image.bmp): ");
        std::getline(std::cin >> std::ws, fileName);

        filePath = std::filesystem::path("images") / fileName;

        std::ifstream file(filePath);
        if (!file) {
            std::cout << "Failas neegzistuoja arba nėra galimas jo skaitymas. Bandykite dar kartą." << std::endl;
        } else {
            file.close();
            break;
        }
    }

    // Load the image
    // The image gets loaded using stb_image third party library
    uint8_t* data = stbi_load(filePath.string().c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        std::cout << "Nepavyko užkrauti nuotraukos. Bandykite dar kartą." << std::endl;
        return;
    }
    std::vector<uint8_t> imageData(data, data + width * height * channels);
    Matrix binaryImageData = bmpToBinary(filePath.string(), k);
    stbi_image_free(data);

    // Inform the user that the process may take a while
    clearConsole();
    printf("Siunčiama nuotrauka, tai gali užtrukti...\n");
    auto startEncode = std::chrono::high_resolution_clock::now(); // Start timer for benchmarking / identifying how long decoding takes

    Matrix encodedImage;
    Matrix receivedImage;
    Matrix decodedImage;
    Matrix sentImage;

    // Skip the header of the image to avoid corrupting it
    // int headerSize = 54;
    // int partsToSkip = (headerSize * 8 + k - 1) / k;
    size_t binaryImageDataSize = binaryImageData.size();
    // printf("Dalinamasi į %d dalis\n", binaryImageDataSize - partsToSkip);

    // Set update interval
    // Interval is bigger due to images being composed of much more data than text
    int progressUpdateInterval = 100; 

    // Start the sending / decoding process
    for (size_t i = 0; i < binaryImageDataSize; ++i) {
        // If the user wants to see progress, show it as percentage of completion
        if (showProgress && i % progressUpdateInterval == 0) {
            printf("Siunčiama nuotrauka: %d%%\r", static_cast<int>((i) * 100 / (binaryImageDataSize)));
            fflush(stdout);
        }

        // Encode the image part
        // auto start = std::chrono::high_resolution_clock::now(); // Start timer for benchmarking / identifying how long decoding takes
        Vec encodedPart = encodeMessage(G, binaryImageData[i], k, &addedBits);
        encodedImage.push_back(encodedPart);
        // auto end = std::chrono::high_resolution_clock::now(); // Ends timer for benchmarking, shows how long it took
        // std::chrono::duration<double> encodeDuration = end - start;
        // std::cout << "Užkodavimas truko: " << encodeDuration.count() << "s" << std::endl;

        // Send image part trough channel
        // auto startSend = std::chrono::high_resolution_clock::now(); // Start timer for benchmarking / identifying how long decoding takes
        Vec receivedPart = introduceErrors(encodedPart, errorRate);
        receivedImage.push_back(receivedPart);
        // auto endSend = std::chrono::high_resolution_clock::now(); // Ends timer for benchmarking, shows how long it took
        // std::chrono::duration<double> sendDuration = endSend - startSend;
        // std::cout << "Kanalas truko " << sendDuration.count() << "s" << std::endl;

        // Decode the received image part
        Vec decodedPart = decodeMessage(H, receivedPart, syndromes, n, k);
        decodedImage.push_back(decodedPart);

        // Send the same image part without using encoding to compare results
        Vec sentPart = introduceErrors(binaryImageData[i], errorRate);
        sentImage.push_back(sentPart);
    }

    // Remove the added bits from the last vector if they were added
    if (addedBits > 0 && !decodedImage.empty()) {
        decodedImage.back().resize(decodedImage.back().size() - addedBits); // Restore to original size
        addedBits = 0; // Reset the added bits counter
    }

    // Get the file names for the new received images
    std::string decodedFileName = fileName.substr(0, fileName.find_last_of('.')) + "_decoded.bmp";
    std::string uncodedFileName = fileName.substr(0, fileName.find_last_of('.')) + "_uncoded.bmp";

    // Convert the binary image data to BMP format and save it
    std::filesystem::path decodedFilePath = std::filesystem::path("images") / decodedFileName;
    std::filesystem::path uncodedFilePath = std::filesystem::path("images") / uncodedFileName;
    binaryToBmp(decodedImage, "images/" + decodedFileName, width, height, channels);
    binaryToBmp(sentImage, "images/" + uncodedFileName, width, height, channels);

    auto endEncode = std::chrono::high_resolution_clock::now(); // Ends timer for benchmarking, shows how long it took
    std::chrono::duration<double> encodeDuration = endEncode - startEncode; 

    // Show the results to user
    clearConsole();
    printf("Rezultatai:\n");
    std::cout << "Išsiųsti bei dekoduoti užtruko " << encodeDuration.count() << "s" << std::endl; 

    // Opens the original provided image file
    openImage(filePath.string());
    printf("Orginali nuotrauka: %s\n", filePath.string().c_str());
    std::cout << "Spauskite Enter norint tęsti...";
    std::cin.get();

    // Opens the decoded image file
    openImage(decodedFilePath.string());
    printf("Dekoduota nuotrauka: %s\n", decodedFilePath.string().c_str());
    std::cout << "Spauskite Enter norint tęsti...";
    std::cin.get();

    // Opens the uncoded image file
    openImage(uncodedFilePath.string());
    printf("Nuotrauka siųsta be kodavimo: %s\n", uncodedFilePath.string().c_str());
    std::cout << "Spauskite Enter norint sugrįžti...";
    std::cin.get();
    return;
}

std::vector<uint8_t> getUserVector(int k, int id) {
    std::string input;
    Vec message(k, 0);

    while (true) {
        // If a required message ID is provided, show that message
        if (id == 1) std::cout << "Įveskite redaguotą vektorių (" << k << " simboliai atskirti tarpais): ";
        // Otherwise show the default message
        else std::cout << "Įveskite vektorių (" << k << " simboliai atskirti tarpais): ";
        // Get the user input
        std::getline(std::cin >> std::ws, input);

        // Parse the input to make sure it is valid
        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;

        // Split the input into parts
        while (iss >> token) {
            tokens.push_back(token);
        }

        // Check if the amount of values is correct
        if (tokens.size() != k) {
            std::cout << "Netinkamas formatas. Įveskite " << k << " simbolius, atskirtus tarpais." << std::endl;
            continue;
        }

        bool validInput = true;
        // Check that the values are valid
        for (const std::string& t : tokens) {
            if (t != "0" && t != "1") {
                std::cout << "Netinkama įvestis. Vektorių gali sudaryti naudojant tik '0' arba '1' simbolius." << std::endl;
                validInput = false;
                break;
            }
        }

        // If the input is valid convert it to a vector
        if (validInput) {
            for (int i = 0; i < k; ++i) {
                message[i] = static_cast<uint8_t>(std::stoi(tokens[i]));
            }
            break;
        }
    }

    return message;
}

bool getYesNoInput() {
    char response;
    while (true) {
        std::cin >> response;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
        // Check the input validity
        // Convert y / n to corresponding bool values
        if (response == 'y') {
            return true;
        } else if (response == 'n') {
            return false;
        } else {
            std::cout << "Netinkama įvestis. Įveskite 'y' arba 'n'." << std::endl;
        }
    }
}

void updateParameters(int* n, int* k, double* errorRate, Matrix* G, Matrix* H, syndromesTable* syndromes) {
    clearConsole();
    printf("[1] Pakeisti generuojančią matricą G\n");
    printf("[2] Pakeisti klaidų darymo procentą\n");
    printf("[3] Pakeisti didžiausią galimą skirtumą n-k\n"); // TODO - update text?
    printf("[4] Sugrįžti\n\n");
    int selection = getUserInput(1, 4, "Įveskite pasirinkimą: ");
    switch (selection) {
        // Update the generative matrix
        case 1:
            clearConsole();
            // Get the new matrix size
            getGenerativeMatrixSize(n, k);
            // Get the new matrix
            *G = getGenerativeMatrix(*k, *n);
            // Generate a new parity matrix
            *H = generateParityMatrix(*G);
            clearConsole();
            // Warn the user that syndrome generation may take a while
            printf("Programa ruošiama, tai gali užtrukti...\n");
            // Generate syndromes
            *syndromes = generateSyndromes(*n, *k, *H, showProgress);
            break;
        // Update the error rate
        case 2:
            clearConsole();
            // Get new error rate
            *errorRate = getUserInputFloat(0, 1, "Įveskite klaidų darymo procentą (0-1): ");
            break;
        // Update the maximum syndrome weight
        case 3:
            clearConsole();
            // Get a new value for biggest possible n - k difference
            maxSyndromeWeight = getUserInput(1, 64, "Įveskite didžiausią galimą skirtumą: "); // TODO - update text?
            break;
        // Return to main menu
        case 4:
            return;
    }
}

void showParameters(Matrix G, Matrix H, int n, int k, double errorRate, syndromesTable syndromes) {
    clearConsole();
    printf("[1] Rodyti generuojančią matricą G\n");
    printf("[2] Rodyti kontrolinę matricą H\n");
    printf("[3] Rodyti matricos parametrus\n");
    printf("[4] Rodyti sugeneruotus sindromus\n");
    printf("[5] Sugrįžti\n\n");
    int selection = getUserInput(1, 5, "Įveskite pasirinkimą: ");
    switch (selection) {
        // Display genearative matrix G
        case 1:
            clearConsole();
            printf("Generuojanti matrica G:\n");
            displayMatrix(G);
            printf("\nSpauskite Enter norint sugrįžti...");
            std::cin.get();
            break;
        // Display parity matrix H
        case 2:
            clearConsole();
            printf("Pasitikrinimo matrica H:\n");
            displayMatrix(H);
            printf("\nSpauskite Enter norint sugrįžti...");
            std::cin.get();
            break;
        // Display the parameters of the matrix
        // Contains: matrix size, error rate and max allowed syndrome weight
        case 3:
            printProgramParameters(n, k, errorRate);
            break;
        // Display the generated syndromes
        // TODO - update the formatting?
        case 4:
            clearConsole();
            printf("Sugeneruoti sindromai:\n");
            // displaySyndromes(syndromes);
            printf("\nSpauskite Enter norint sugrįžti...");
            std::cin.get();
            break;
        // Return to main menu
        case 5:
            return;
    }
}

void printProgramParameters(int n, int k, double errorRate) {
    clearConsole();
    // Display the main parameters of the program
    printf("Parametrai:\n");
    printf("Matricos ilgis n: %d\n", n);
    printf("Matricos dimensija k: %d\n", k);
    printf("Klaidos procentas: %f\n", errorRate);
    printf("Didžiausias galimas skirtumas: %d\n", maxSyndromeWeight);
    printf("\nSpauskite Enter norint sugrįžti...");
    // std::cin.ignore(); 
    std::cin.get();   
    return;
}

void getGenerativeMatrixSize(int* n, int* k) {
    clearConsole();
    while(true) {
        // Get the size of the generative matrix
        *n = getUserInput(1, 64, "Įveskite matricos ilgį (n): ");
        *k = getUserInput(1, *n, "Įveskite matricjos dimensiją (k <= n): ");
        // Validate the difference between n and k
        if (*n - *k >= maxSyndromeWeight) {
            printf("Skirtumas tarp k ir n perdidelis.\n", maxSyndromeWeight);
        } else {
            break;
        }
    }
}

void setProgramSettings() {
    clearConsole();
    // Depending wether progress messages are on or off, print the corresponding message
    if (showProgress) {
        printf("[1] Išjungti progreso žinutes.\n");
    } else {
        printf("[1] Rodyti progreso žinutes (tai pakenks programos veikimo greičiui).\n");
    }
    printf("[2] Sugrįžti\n\n");
    int selection = getUserInput(1, 2, "Įveskite pasirinkimą: ");
    switch (selection) {
        case 1:
            // Toggle the progress messages
            showProgress = !showProgress;
            break;
        case 2:
            // Return to main menu
            return;
    }
}

Matrix getGenerativeMatrix(int k, int n) {
    clearConsole();

    // If n = k, there is nothing for the user to input
    if (n == k) {
        return generateRandomMatrix(k, n); 
    }

    // If the generative matrix is not a square, allow user to input their own part A of matrix G
    printf("[1] Sugeneruoti atsitiktinę matricą\n");
    printf("[2] Įvesti pasirinktą matricą\n\n");

    int selection = getUserInput(1, 2, "Įveskite pasirinkimą: ");

    clearConsole();
    // Based on user choice, generate a random matrix or allow them to input their own
    if (selection == 1) {
        return generateRandomMatrix(k, n);
    } else {
        return generateUserMatrix(k, n);
    }
}

int getUserInput(int minInput, int maxInput, std::string message) {
    int input;
    while (true) {
        // Print the provided message to function, describing what the user should input
        printf("%s", message.c_str());
        std::cin >> input;
        // Validate the input
        if (std::cin.fail() || input < minInput || input > maxInput) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printf("Netinkama įvestis. Bandykite dar kartą.\n");
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    return input;
}

float getUserInputFloat(float minInput, float maxInput, std::string message) {
    std::string stringInput;
    float input;
    while (true) {
        // Print the provided message to function, describing what the user should input
        printf("%s", message.c_str());
        // Get input as string to allow for different comma formats between regions
        std::cin >> stringInput;
        std::replace(stringInput.begin(), stringInput.end(), ',', '.');

        try {
            // Convert the string to float
            input = std::stof(stringInput);
            // Validate the input
            if (input >= minInput && input <= maxInput) {
                break;
            } else {
                printf("Netinkama įvestis. Bandykite dar kartą.\n");
            }
        } catch (const std::invalid_argument&) {
            printf("Netinkama įvestis. Bandykite dar kartą.\n");
        } catch (const std::out_of_range&) {
            printf("Netinkama įvestis. Bandykite dar kartą.\n");
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return input;
}

void openImage(const std::string& filePath) {
std::string command;

// Open the image in the default image viewer
// Depending on the OS, the command to open the image is different
#ifdef _WIN32
    command = "start " + filePath;
#else
    command = "xdg-open " + filePath;
#endif
    system(command.c_str());
}