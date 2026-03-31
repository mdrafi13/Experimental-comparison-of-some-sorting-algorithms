#include <iostream>
#include <filesystem>   // C++17 feature
#include <vector>
#include <chrono>
#include <future>       // for async and future (for timeout)
#include "data.h"

namespace fs = std::filesystem;

// ------------------------
// Sorting algorithms
// ------------------------
std::vector<int> bubbleSort(std::vector<int> data) {
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data.size() - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
            }
        }
    }
    return data;
}

std::vector<int> selectionSort(std::vector<int> data) {
    for (size_t i = 0; i < data.size(); i++) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < data.size(); j++) {
            if (data[j] < data[minIndex]) {
                minIndex = j;
            }
        }
        std::swap(data[i], data[minIndex]);
    }
    return data;
}

std::vector<int> insertionSort(std::vector<int> data) {
    for (size_t i = 1; i < data.size(); i++) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
    return data;
}

void merge(std::vector<int>& data, int left, int mid, int right) {
    std::vector<int> temp;
    int i = left, j = mid + 1;
    while (i <= mid && j <= right) {
        if (data[i] < data[j]) temp.push_back(data[i++]);
        else temp.push_back(data[j++]);
    }
    while (i <= mid) temp.push_back(data[i++]);
    while (j <= right) temp.push_back(data[j++]);
    for (int k = 0; k < temp.size(); k++) data[left + k] = temp[k];
}

void mergeSortHelper(std::vector<int>& data, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSortHelper(data, left, mid);
    mergeSortHelper(data, mid + 1, right);
    merge(data, left, mid, right);
}

std::vector<int> mergeSort(std::vector<int> data) {
    mergeSortHelper(data, 0, data.size() - 1);
    return data;
}

int partition(std::vector<int>& data, int low, int high) {
    int pivot = data[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (data[j] < pivot) {
            i++;
            std::swap(data[i], data[j]);
        }
    }
    std::swap(data[i + 1], data[high]);
    return i + 1;
}

void quickSortHelper(std::vector<int>& data, int low, int high) {
    if (low < high) {
        int pi = partition(data, low, high);
        quickSortHelper(data, low, pi - 1);
        quickSortHelper(data, pi + 1, high);
    }
}

std::vector<int> quickSort(std::vector<int> data) {
    quickSortHelper(data, 0, data.size() - 1);
    return data;
}

// ------------------------
// Timing helper with timeout
// ------------------------
template<typename Func>
double timeSortWithTimeout(Func sortFunc, std::vector<int> data, const std::string& name,
                           const std::string& datasetName, double maxSeconds = 0) {
    std::cout << "Sorting " << name << " on " << datasetName << "...\n";

    if (maxSeconds <= 0) {
        auto start = std::chrono::high_resolution_clock::now();
        sortFunc(data);
        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double>(end - start).count();
        std::cout << name << " on " << datasetName << " took " << duration << " seconds.\n\n";
        return duration;
    } else {
        auto future = std::async(std::launch::async, sortFunc, data);
        if (future.wait_for(std::chrono::seconds((int)maxSeconds)) == std::future_status::ready) {
            future.get(); // retrieve result
            std::cout << name << " on " << datasetName << " finished in less than " << maxSeconds << " seconds.\n\n";
            return maxSeconds; // approximate
        } else {
            std::cout << name << " on " << datasetName << " exceeded " << maxSeconds 
                      << " seconds and was stopped.\n\n";
            return maxSeconds;
        }
    }
}

// ------------------------
// Main function
// ------------------------
int main() {
    // ------------------------
    // Generate datasets if not exist
    // ------------------------
    if (!fs::exists("large_data.txt")) generateDataset("large_data.txt", 20000000, 1, 20000000);
    if (!fs::exists("small_data.txt")) generateDataset("small_data.txt", 10000, 1, 20000000);

    // ------------------------
    // Read datasets
    // ------------------------
    std::vector<int> largeData = readDataset("large_data.txt");
    std::vector<int> smallData = readDataset("small_data.txt");

    // ------------------------
    // Small dataset
    // ------------------------
    std::cout << "=== Small dataset (10,000 numbers) ===\n";
    timeSortWithTimeout(bubbleSort, smallData, "Bubble Sort", "small dataset");
    timeSortWithTimeout(selectionSort, smallData, "Selection Sort", "small dataset");
    timeSortWithTimeout(insertionSort, smallData, "Insertion Sort", "small dataset");
    timeSortWithTimeout(mergeSort, smallData, "Merge Sort", "small dataset");
    timeSortWithTimeout(quickSort, smallData, "Quick Sort", "small dataset");

    // ------------------------
    // Large dataset
    // ------------------------
    std::cout << "=== Large dataset (20,000,000 numbers) ===\n";
    timeSortWithTimeout(insertionSort, largeData, "Insertion Sort", "large dataset", 40);
    timeSortWithTimeout(selectionSort, largeData, "Selection Sort", "large dataset", 40);
    timeSortWithTimeout(bubbleSort, largeData, "Bubble Sort", "large dataset", 40);
    timeSortWithTimeout(mergeSort, largeData, "Merge Sort", "large dataset");
    timeSortWithTimeout(quickSort, largeData, "Quick Sort", "large dataset");

    return 0;
}