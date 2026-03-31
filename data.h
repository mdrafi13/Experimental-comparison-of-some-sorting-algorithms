#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>

void generateDataset(const std::string& filename, size_t size, int minVal, int maxVal);
std::vector<int> readDataset(const std::string& filename);
void printSample(const std::vector<int>& data, size_t n = 10);

#endif