#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>

// ------------------------
// Function to generate a dataset and save to file
// ------------------------
void generateDataset(const std::string& filename, size_t size, int minVal, int maxVal) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minVal, maxVal);

    for (size_t i = 0; i < size; ++i) {
        outFile << dis(gen) << "\n"; // write each number in a new line
    }

    std::cout << "Generated " << size << " numbers and saved to " << filename << "\n";
}

// ------------------------
// Function to read a dataset from a file
// ------------------------
std::vector<int> readDataset(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for reading: " << filename << "\n";
return std::vector<int>();
    }

    std::vector<int> data;
    int number;
    while (inFile >> number) {
        data.push_back(number);
    }

    std::cout << "Read " << data.size() << " numbers from " << filename << "\n";
    return data;
}

// ------------------------
// Function to print first n elements of a dataset
// ------------------------
void printSample(const std::vector<int>& data, size_t n = 10) {
    n = std::min(n, data.size());
    for (size_t i = 0; i < n; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << "...\n";
}







