#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "FileService.h"

using namespace std;

// Structure to hold name and number pair
struct NameNumber {
    std::string name;
    int number;

    // Constructor to initialize the structure from a string
    NameNumber(const std::string& str) {
        std::istringstream iss(str);
        iss >> name >> number;
    }

    // Convert the structure back to a string
    std::string toString() const {
        return name + " " + std::to_string(number);
    }
};

void checkAndEraseLines() {
    ifstream inFile("Score");
    vector<string> lines;
    string line;

    // Read all lines from the file
    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    inFile.close();

    // If there are more than 10 lines, erase the extra lines
    if (lines.size() > 10) {
        lines.erase(lines.begin(), lines.begin() + (lines.size() - 10));
    }

    // Write the remaining lines back to the file
    ofstream outFile("Score");
    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();
}

// Function to check if the file exists
BOOL FileExists(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r") == 0) {
        fclose(file);
        return 1; // File exists
    }
    return 0; // File does not exist
}

// Comparator function to sort by number in descending order, then by name
bool compare(const NameNumber& a, const NameNumber& b) {
    if (a.number == b.number) {
        return a.name < b.name;
    }
    return a.number > b.number;
}

// Function to read data from a file, sort it, and write the sorted data back to the file
void sortFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        return;
    }

    std::vector<NameNumber> data;
    std::string line;

    // Read each line from the file and store it in the vector
    while (std::getline(file, line)) {
        data.emplace_back(line);
    }

    file.close();

    // Sort the vector using the custom comparator
    std::sort(data.begin(), data.end(), compare);

    // Write the sorted data back to the file
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        return;
    }

    for (const auto& item : data) {
        outFile << item.toString() << std::endl;
    }

    outFile.close();
}

