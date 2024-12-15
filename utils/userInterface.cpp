#include "../headers/userInterface.h"
#include "../headers/stb_image.h"
#include "../headers/stb_image_write.h"

#include <chrono> // for benchmarking

int maxSyndromeWeight = 25;
int addedBits = 0;
bool showProgress = false;

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

    getGenerativeMatrixSize(&n, &k);
    Matrix G = getGenerativeMatrix(k, n);
    clearConsole();
    errorRate = getUserInputFloat(0, 1, "Įveskite klaidų darymo procentą (0-1): ");

    clearConsole();
    printf("Ar norite įjungti progreso žinutes? (Žinutės sulėtina programos veikimą) (y/n): ");
    showProgress = getYesNoInput();

    // Find needed syndromes
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

    Vec message(k, 0); 
    message = getUserVector(k, 0);

    clearConsole();
    printf("Vektorius siunčiamas...\n");

    Vec encodedMessage = encodeMessage(G, message, k, &addedBits);
    Vec receivedMessage = introduceErrors(encodedMessage, errorRate);

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
    
    Vec decodedMessage = decodeMessage(H, receivedMessage, syndromes, n, k);
    clearConsole();
    printf("Orginalus vektorius: \n");
    displayVector(message);
    printf("\nUžkoduotas vektorius: \n");
    displayVector(encodedMessage);
    printf("\nIš kanalo gautas vektorius: \n");
    displayVectorWithMistakes(encodedMessage, receivedMessage);
    printf("\nDekoduotas vektorius: \n");
    displayVector(decodedMessage);
    printf("\nSpauskite Enter norėdami sugrįžti...");
    std::cin.get();
    return;
}

void sendMessage(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate) {
    clearConsole();

    std::string message;
    printf("Įrašykite tekstą siuntimui: ");
    std::getline(std::cin >> std::ws, message);

    clearConsole();
    printf("Tekstas siunčiamas...\n");
    
    Matrix binaryMessage = stringToBinary(message, k);
    
    Matrix encodedMessage;
    Matrix receivedMessage;
    Matrix decodedMessage;
    Matrix unencodedMessageBinary;

    int progressUpdateInterval = 5; 
    size_t binaryMessageSize = binaryMessage.size();

    for (size_t i = 0; i < binaryMessageSize; ++i) {
        if (showProgress && i % progressUpdateInterval == 0) {
            printf("Apdorojama: %d%%\r", static_cast<int>(i * 100 / binaryMessageSize));
            fflush(stdout);
        }

        Vec encodedPart = encodeMessage(G, binaryMessage[i], k, &addedBits);
        encodedMessage.push_back(encodedPart);

        Vec receivedPart = introduceErrors(encodedPart, errorRate);
        receivedMessage.push_back(receivedPart);

        Vec decodedPart = decodeMessage(H, receivedPart, syndromes, n, k);
        decodedMessage.push_back(decodedPart);

        Vec unencodedPart = introduceErrors(binaryMessage[i], errorRate);
        unencodedMessageBinary.push_back(unencodedPart);
    }

    std::string decodedMessageString = binaryToString(decodedMessage);
    std::string unencodedMessage = binaryToString(unencodedMessageBinary);

    clearConsole();
    printf("Rezultatai:\n");
    printf("Orginalus tekstas: %s\n", message.c_str());
    printf("Dekoduotas tekstas: %s\n", decodedMessageString.c_str());
    printf("Tekstas gautas nenaudojant kodavimo: %s\n", unencodedMessage.c_str());
    // printf("Encoded VS snet \n");
    // displayVectorDifferences(encodedMessage, receivedMessage);
    // printf("Decoded vs orignial \n");
    // displayVectorDifferences(binaryMessage, decodedMessage);

    printf("\nSpauskite Enter norint sugrįžti...");
    std::cin.get();
    return;
}

void sendImage(Matrix G, Matrix H, syndromesTable syndromes, int n, int k, double errorRate) {
    clearConsole();

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
    uint8_t* data = stbi_load(filePath.string().c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        std::cout << "Nepavyko užkrauti nuotraukos. Bandykite dar kartą." << std::endl;
        return;
    }
    std::vector<uint8_t> imageData(data, data + width * height * channels);
    Matrix binaryImageData = bmpToBinary(filePath.string(), k);
    stbi_image_free(data);

    clearConsole();
    printf("Siunčiama nuotrauka, tai gali užtrukti...\n");
    auto startEncode = std::chrono::high_resolution_clock::now(); // Start timer for benchmarking / identifying how long decoding takes

    Matrix encodedImage;
    Matrix receivedImage;
    Matrix decodedImage;
    Matrix sentImage;

    int headerSize = 54;
    int partsToSkip = (headerSize * 8 + k - 1) / k;
    size_t binaryImageDataSize = binaryImageData.size();

    int progressUpdateInterval = 100; 

    for (size_t i = 0; i < binaryImageDataSize; ++i) {
        if (i < partsToSkip) {
            encodedImage.push_back(binaryImageData[i]);
            receivedImage.push_back(binaryImageData[i]);
            decodedImage.push_back(binaryImageData[i]);
            sentImage.push_back(binaryImageData[i]);
        } else {
            if (showProgress && i % progressUpdateInterval == 0) {
                printf("Siunčiama nuotrauka: %d%%\r", static_cast<int>((i - partsToSkip) * 100 / (binaryImageDataSize - partsToSkip)));
                fflush(stdout);
            }
            Vec encodedPart = encodeMessage(G, binaryImageData[i], k, &addedBits);
            encodedImage.push_back(encodedPart);

            Vec receivedPart = introduceErrors(encodedPart, errorRate);
            receivedImage.push_back(receivedPart);

            Vec decodedPart = decodeMessage(H, receivedPart, syndromes, n, k);
            decodedImage.push_back(decodedPart);

            Vec sentPart = introduceErrors(binaryImageData[i], errorRate);
            sentImage.push_back(sentPart);
        }
    }

    std::string decodedFileName = fileName.substr(0, fileName.find_last_of('.')) + "_decoded.bmp";
    std::string uncodedFileName = fileName.substr(0, fileName.find_last_of('.')) + "_uncoded.bmp";
    std::filesystem::path decodedFilePath = std::filesystem::path("images") / decodedFileName;
    std::filesystem::path uncodedFilePath = std::filesystem::path("images") / uncodedFileName;
    binaryToBmp(decodedImage, "images/" + decodedFileName, width, height, channels);
    binaryToBmp(sentImage, "images/" + uncodedFileName, width, height, channels);

    clearConsole();
    auto endEncode = std::chrono::high_resolution_clock::now(); // Ends timer for benchmarking, shows how long it took
    std::chrono::duration<double> encodeDuration = endEncode - startEncode; 
    printf("Rezultatai:\n");
    std::cout << "Išsiųsti bei dekoduoti užtruko " << encodeDuration.count() << "s" << std::endl;

    openImage(filePath.string());
    printf("Orginali nuotrauka: %s\n", filePath.string().c_str());
    std::cout << "Spauskite Enter norint tęsti...";
    std::cin.get();

    openImage(decodedFilePath.string());
    printf("Dekoduota nuotrauka: %s\n", decodedFilePath.string().c_str());
    std::cout << "Spauskite Enter norint tęsti...";
    std::cin.get();

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
        if (id == 1) std::cout << "Įveskite redaguotą vektorių (" << k << " simboliai atskirti tarpais): ";
        else std::cout << "Įveskite vektorių (" << k << " simboliai atskirti tarpais): ";
        std::getline(std::cin >> std::ws, input);

        std::istringstream iss(input);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.size() != k) {
            std::cout << "Netinkamas formatas. Įveskite " << k << " simbolius, atskirtus tarpais." << std::endl;
            continue;
        }

        bool validInput = true;
        for (const std::string& t : tokens) {
            if (t != "0" && t != "1") {
                std::cout << "Netinkama įvestis. Vektorių gali sudaryti naudojant tik '0' arba '1' simbolius." << std::endl;
                validInput = false;
                break;
            }
        }

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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    if (selection == 4) return;
    if (selection == 1) {
        clearConsole();
        getGenerativeMatrixSize(n, k);
        *G = getGenerativeMatrix(*k, *n);
        *H = generateParityMatrix(*G);
        clearConsole();
        printf("Programa ruošiama, tai gali užtrukti...\n");
        *syndromes = generateSyndromes(*n, *k, *H, showProgress);
    } else if (selection == 2) {
        clearConsole();
        *errorRate = getUserInputFloat(0, 1, "Įveskite klaidų darymo procentą (0-1): ");
    } else if (selection == 3) {
        clearConsole();
        maxSyndromeWeight = getUserInput(1, 64, "Įveskite didžiausią galimą skirtumą: "); // TODO - update text?
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
        case 1:
            clearConsole();
            printf("Generuojanti matrica G:\n");
            displayMatrix(G);
            printf("\nSpauskite Enter norint sugrįžti...");
            std::cin.get();
            break;
        case 2:
            clearConsole();
            printf("Pasitikrinimo matrica H:\n");
            displayMatrix(H);
            printf("\nSpauskite Enter norint sugrįžti...");
            std::cin.get();
            break;
        case 3:
            printProgramParameters(n, k, errorRate);
            break;
        case 4:
            clearConsole();
            printf("Sugeneruoti sindromai:\n");
            displaySyndromes(syndromes);
            printf("\nSpauskite Enter norint sugrįžti...");
            std::cin.get();
            break;
        case 5:
            return;
    }
}

void printProgramParameters(int n, int k, double errorRate) {
    clearConsole();
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
        *n = getUserInput(1, 64, "Įveskite matricos ilgį (n): ");
        *k = getUserInput(1, *n - 1, "Įveskite matricjos dimensiją (k < n): ");
        if (*n - *k >= maxSyndromeWeight) {
            printf("Skirtumas tarp k ir n perdidelis.\n", maxSyndromeWeight); // TODO - UPDATE
        } else {
            break;
        }
    }
}

void setProgramSettings() {
    clearConsole();
    if (showProgress) {
        printf("[1] Išjungti progreso žinutes.\n");
    } else {
        printf("[1] Rodyti progreso žinutes (tai pakenks programos veikimo greičiui).\n");
    }
    printf("[2] Sugrįžti\n\n");
    int selection = getUserInput(1, 2, "Įveskite pasirinkimą: ");
    switch (selection) {
        case 1:
            showProgress = !showProgress;
            break;
        case 2:
            return;
    }
}

Matrix getGenerativeMatrix(int k, int n) {
    clearConsole();
    printf("[1] Sugeneruoti atsitiktinę matricą\n");
    printf("[2] Įvesti pasirinktą matricą\n\n");
    int selection = getUserInput(1, 2, "Įveskite pasirinkimą: ");
    clearConsole();
    if (selection == 1) {
        return generateRandomMatrix(k, n);
    } else {
        return generateUserMatrix(k, n);
    }
}

int getUserInput(int minInput, int maxInput, std::string message) {
    int input;
    while (true) {
        printf("%s", message.c_str());
        std::cin >> input;
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
        printf("%s", message.c_str());
        std::cin >> stringInput;
        std::replace(stringInput.begin(), stringInput.end(), ',', '.');
        try {
            input = std::stof(stringInput);
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
#ifdef _WIN32
    command = "start " + filePath;
#elif __APPLE__
    command = "open " + filePath;
#else
    command = "xdg-open " + filePath;
#endif
    system(command.c_str());
}