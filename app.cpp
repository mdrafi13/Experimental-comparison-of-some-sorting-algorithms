#include <iostream>
#include <filesystem>   // C++17 feature
#include <vector>
#include <chrono>
#include <future>       // for async and future (for timeout)
#include <string>
#include <functional>
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

struct DatasetSpec {
    std::string name;
    std::string smallFile;
    std::string largeFile;
    std::function<void(const std::string&, size_t)> smallGenerator;
    std::function<void(const std::string&, size_t)> largeGenerator;
};

void runAllSortsOnDataset(const std::vector<int>& data, const std::string& datasetLabel, bool useTimeoutForQuadratic) {
    std::cout << "=== " << datasetLabel << " (" << data.size() << " numbers) ===\n";

    double timeout = useTimeoutForQuadratic ? 40.0 : 0.0;
    timeSortWithTimeout(insertionSort, data, "Insertion Sort", datasetLabel, timeout);
    timeSortWithTimeout(selectionSort, data, "Selection Sort", datasetLabel, timeout);
    timeSortWithTimeout(bubbleSort, data, "Bubble Sort", datasetLabel, timeout);
    timeSortWithTimeout(mergeSort, data, "Merge Sort", datasetLabel);
    timeSortWithTimeout(quickSort, data, "Quick Sort", datasetLabel);
}

// ------------------------
// Main function
// ------------------------
int main() {
    const size_t smallSize = 10000;
    const size_t largeSize = 20000000;
    const int minVal = 1;
    const int maxVal = 20000000;

    std::vector<DatasetSpec> datasets = {
        {
            "random",
            "small_random_data.txt",
            "large_random_data.txt",
            [minVal, maxVal](const std::string& file, size_t size) { generateDataset(file, size, minVal, maxVal); },
            [minVal, maxVal](const std::string& file, size_t size) { generateDataset(file, size, minVal, maxVal); }
        },
        {
            "sorted",
            "small_sorted_data.txt",
            "large_sorted_data.txt",
            [](const std::string& file, size_t size) { generateSortedDataset(file, size); },
            [](const std::string& file, size_t size) { generateSortedDataset(file, size); }
        },
        {
            "reverse_sorted",
            "small_reverse_sorted_data.txt",
            "large_reverse_sorted_data.txt",
            [](const std::string& file, size_t size) { generateReverseSortedDataset(file, size); },
            [](const std::string& file, size_t size) { generateReverseSortedDataset(file, size); }
        },
        {
            "almost_sorted",
            "small_almost_sorted_data.txt",
            "large_almost_sorted_data.txt",
            [minVal, maxVal](const std::string& file, size_t size) {
                generateAlmostSortedDataset(file, size, minVal, maxVal, 0.02);
            },
            [minVal, maxVal](const std::string& file, size_t size) {
                generateAlmostSortedDataset(file, size, minVal, maxVal, 0.02);
            }
        },
        {
            "half_sorted",
            "small_half_sorted_data.txt",
            "large_half_sorted_data.txt",
            [minVal, maxVal](const std::string& file, size_t size) { generateHalfSortedDataset(file, size, minVal, maxVal); },
            [minVal, maxVal](const std::string& file, size_t size) { generateHalfSortedDataset(file, size, minVal, maxVal); }
        },
        {
            "flat",
            "small_flat_data.txt",
            "large_flat_data.txt",
            [](const std::string& file, size_t size) { generateFlatDataset(file, size, 10); },
            [](const std::string& file, size_t size) { generateFlatDataset(file, size, 10); }
        },
        {
            "few_unique",
            "small_few_unique_data.txt",
            "large_few_unique_data.txt",
            [minVal, maxVal](const std::string& file, size_t size) { generateFewUniqueDataset(file, size, minVal, maxVal, 5); },
            [minVal, maxVal](const std::string& file, size_t size) { generateFewUniqueDataset(file, size, minVal, maxVal, 5); }
        }
    };

    // ------------------------
    // Generate datasets if not exist
    // ------------------------
    for (const auto& spec : datasets) {
        if (!fs::exists(spec.smallFile)) {
            spec.smallGenerator(spec.smallFile, smallSize);
        }
        if (!fs::exists(spec.largeFile)) {
            spec.largeGenerator(spec.largeFile, largeSize);
        }
    }

    // ------------------------
    // Run all algorithms on all dataset types
    // ------------------------
    for (const auto& spec : datasets) {
        std::vector<int> smallData = readDataset(spec.smallFile);
        std::vector<int> largeData = readDataset(spec.largeFile);

        runAllSortsOnDataset(smallData, "small " + spec.name + " dataset", false);
        runAllSortsOnDataset(largeData, "large " + spec.name + " dataset", true);
    }

    return 0;
}