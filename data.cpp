#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include "data.h"

namespace {
bool writeDataset(const std::string& filename, const std::vector<int>& data) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename << "\n";
        return false;
    }

    for (int value : data) {
        outFile << value << "\n";
    }
    return true;
}

std::mt19937 createGenerator() {
    std::random_device rd;
    return std::mt19937(rd());
}
} // namespace

// ------------------------
// Function to generate a dataset and save to file
// ------------------------
void generateDataset(const std::string& filename, size_t size, int minVal, int maxVal) {
    std::mt19937 gen = createGenerator();
    std::uniform_int_distribution<> dis(minVal, maxVal);
    std::vector<int> data;
    data.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        data.push_back(dis(gen));
    }

    if (writeDataset(filename, data)) {
        std::cout << "Generated random dataset (" << size << ") and saved to " << filename << "\n";
    }
}

void generateSortedDataset(const std::string& filename, size_t size, int startVal, int step) {
    std::vector<int> data;
    data.reserve(size);
    int value = startVal;

    for (size_t i = 0; i < size; ++i) {
        data.push_back(value);
        value += step;
    }

    if (writeDataset(filename, data)) {
        std::cout << "Generated sorted dataset (" << size << ") and saved to " << filename << "\n";
    }
}

void generateReverseSortedDataset(const std::string& filename, size_t size, int startVal, int step) {
    std::vector<int> data;
    data.reserve(size);
    int value = startVal + static_cast<int>(size) * step;

    for (size_t i = 0; i < size; ++i) {
        data.push_back(value);
        value -= step;
    }

    if (writeDataset(filename, data)) {
        std::cout << "Generated reverse sorted dataset (" << size << ") and saved to " << filename << "\n";
    }
}

void generateAlmostSortedDataset(const std::string& filename, size_t size, int minVal, int maxVal, double disorderFraction) {
    generateDataset(filename, size, minVal, maxVal);
    std::vector<int> data = readDataset(filename);
    std::sort(data.begin(), data.end());

    std::mt19937 gen = createGenerator();
    std::uniform_int_distribution<size_t> indexDist(0, data.size() - 1);
    size_t swapCount = static_cast<size_t>(data.size() * disorderFraction);

    for (size_t i = 0; i < swapCount; ++i) {
        size_t a = indexDist(gen);
        size_t b = indexDist(gen);
        std::swap(data[a], data[b]);
    }

    if (writeDataset(filename, data)) {
        std::cout << "Generated almost sorted dataset (" << size << ") and saved to " << filename << "\n";
    }
}

void generateHalfSortedDataset(const std::string& filename, size_t size, int minVal, int maxVal) {
    std::mt19937 gen = createGenerator();
    std::uniform_int_distribution<> dis(minVal, maxVal);

    std::vector<int> data;
    data.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        data.push_back(dis(gen));
    }

    size_t half = size / 2;
    std::sort(data.begin(), data.begin() + half);

    if (writeDataset(filename, data)) {
        std::cout << "Generated half sorted dataset (" << size << ") and saved to " << filename << "\n";
    }
}

void generateFlatDataset(const std::string& filename, size_t size, int distinctCount) {
    if (distinctCount < 1) distinctCount = 1;
    std::mt19937 gen = createGenerator();
    std::uniform_int_distribution<> dis(1, distinctCount);

    std::vector<int> data;
    data.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        data.push_back(dis(gen));
    }

    if (writeDataset(filename, data)) {
        std::cout << "Generated flat dataset (" << size << ", distinct=" << distinctCount
                  << ") and saved to " << filename << "\n";
    }
}

void generateFewUniqueDataset(const std::string& filename, size_t size, int minVal, int maxVal, int distinctCount) {
    if (distinctCount < 1) distinctCount = 1;

    std::mt19937 gen = createGenerator();
    std::uniform_int_distribution<> uniqueValDist(minVal, maxVal);

    std::vector<int> uniqueValues;
    uniqueValues.reserve(static_cast<size_t>(distinctCount));
    for (int i = 0; i < distinctCount; ++i) {
        uniqueValues.push_back(uniqueValDist(gen));
    }

    std::uniform_int_distribution<size_t> pickDist(0, uniqueValues.size() - 1);
    std::vector<int> data;
    data.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        data.push_back(uniqueValues[pickDist(gen)]);
    }

    if (writeDataset(filename, data)) {
        std::cout << "Generated few-unique dataset (" << size << ", distinct=" << distinctCount
                  << ") and saved to " << filename << "\n";
    }
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
void printSample(const std::vector<int>& data, size_t n) {
    n = std::min(n, data.size());
    for (size_t i = 0; i < n; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << "...\n";
}







