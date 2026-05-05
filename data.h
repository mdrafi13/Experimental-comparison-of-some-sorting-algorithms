#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>

void generateDataset(const std::string& filename, size_t size, int minVal, int maxVal);
void generateSortedDataset(const std::string& filename, size_t size, int startVal = 1, int step = 1);
void generateReverseSortedDataset(const std::string& filename, size_t size, int startVal = 1, int step = 1);
void generateAlmostSortedDataset(const std::string& filename, size_t size, int minVal, int maxVal, double disorderFraction = 0.02);
void generateHalfSortedDataset(const std::string& filename, size_t size, int minVal, int maxVal);
void generateFlatDataset(const std::string& filename, size_t size, int distinctCount = 10);
void generateFewUniqueDataset(const std::string& filename, size_t size, int minVal, int maxVal, int distinctCount = 5);
std::vector<int> readDataset(const std::string& filename);
void printSample(const std::vector<int>& data, size_t n = 10);

#endif