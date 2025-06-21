#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

const std::string basePath = "./data/";
const char ENCRYPTION_KEY = 'K'; // Simple XOR key

std::string xorEncryptDecrypt(const std::string &input) {
    std::string output = input;
    for (char &c : output) {
        c ^= ENCRYPTION_KEY;
    }
    return output;
}

void storeFile() {
    std::string country, state, district, description;

    std::cout << "Enter Country: ";
    std::getline(std::cin, country);
    std::cout << "Enter State: ";
    std::getline(std::cin, state);
    std::cout << "Enter District: ";
    std::getline(std::cin, district);
    std::cout << "Enter Description: ";
    std::getline(std::cin, description);

    // Construct directory path
    fs::path dirPath = basePath + country + "/" + state + "/" + district + "/";
    fs::create_directories(dirPath);

    std::string fileName = "info.dat";
    fs::path filePath = dirPath / fileName;

    std::ofstream outFile(filePath);
    if (!outFile) {
        std::cerr << "Error creating file!\n";
        return;
    }

    std::string fullContent = "Country: " + country + "\nState: " + state + "\nDistrict: " + district + "\nDescription: " + description;
    std::string encrypted = xorEncryptDecrypt(fullContent);

    outFile << encrypted;
    outFile.close();

    std::cout << "File stored successfully at: " << filePath << "\n";
}

void retrieveFile() {
    std::string country, state, district;

    std::cout << "Enter Country: ";
    std::getline(std::cin, country);
    std::cout << "Enter State: ";
    std::getline(std::cin, state);
    std::cout << "Enter District: ";
    std::getline(std::cin, district);

    fs::path filePath = basePath + country + "/" + state + "/" + district + "/info.dat";

    if (!fs::exists(filePath)) {
        std::cerr << "File not found!\n";
        return;
    }

    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cerr << "Error reading file!\n";
        return;
    }

    std::string encryptedContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    std::string decrypted = xorEncryptDecrypt(encryptedContent);

    std::cout << "\n--- Decrypted File Content ---\n";
    std::cout << decrypted << "\n";
}

int main() {
    int choice;

    while (true) {
        std::cout << "\n========= File Encryption App =========\n";
        std::cout << "1. Store a new file\n";
        std::cout << "2. Retrieve and view a file\n";
        std::cout << "3. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;
        std::cin.ignore(); // to discard newline left by std::cin

        switch (choice) {
            case 1:
                storeFile();
                break;
            case 2:
                retrieveFile();
                break;
            case 3:
                std::cout << "Exiting application.\n";
                return 0;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    }
}

