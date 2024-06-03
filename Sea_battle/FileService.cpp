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

void ShowErrorMessage(const std::wstring& message, DWORD errorCode) {
    std::wstring fullMessage = message + L"\nError Code: " + std::to_wstring(errorCode);
    MessageBoxW(NULL, fullMessage.c_str(), L"Error", MB_ICONERROR);
}

void TruncateFileAfterTenLines(const std::wstring& filePath) {
    HANDLE hFile = CreateFileW(
        filePath.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        ShowErrorMessage(L"Failed to open file", GetLastError());
        return;
    }

    // Allocate a buffer for file reading
    const DWORD bufferSize = 4096;
    char buffer[bufferSize] = { 0 };
    DWORD bytesRead;
    std::vector<std::string> lines;

    // Read file content and store lines
    bool fileEnd = false;
    while (!fileEnd && ReadFile(hFile, buffer, bufferSize, &bytesRead, NULL)) {
        if (bytesRead == 0) break;
        std::string chunk(buffer, bytesRead);
        size_t pos = 0;
        while ((pos = chunk.find('\n')) != std::string::npos) {
            lines.push_back(chunk.substr(0, pos + 1));
            chunk.erase(0, pos + 1);
            if (lines.size() > 10) {
                fileEnd = true;
                break;
            }
        }
    }

    if (lines.size() <= 10) {
        CloseHandle(hFile);
        return; // File has 10 or fewer lines, nothing to truncate
    }

    // Prepare new file content
    std::string newContent;
    for (size_t i = 0; i < 10; ++i) {
        newContent += lines[i];
    }

    // Move file pointer to the beginning
    if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
        ShowErrorMessage(L"Failed to set file pointer", GetLastError());
        CloseHandle(hFile);
        return;
    }

    // Write the truncated content back to the file
    DWORD bytesWritten;
    if (!WriteFile(hFile, newContent.c_str(), static_cast<DWORD>(newContent.size()), &bytesWritten, NULL)) {
        ShowErrorMessage(L"Failed to write to file", GetLastError());
        CloseHandle(hFile);
        return;
    }

    // Set the end of the file after the new content
    if (SetEndOfFile(hFile) == 0) {
        ShowErrorMessage(L"Failed to set end of file", GetLastError());
        CloseHandle(hFile);
        return;
    }

    CloseHandle(hFile);
}

BOOL FileExists(const char* filename) {
    return GetFileAttributesA(filename) != INVALID_FILE_ATTRIBUTES;
}

// Comparator function to sort by number in descending order, then by name
bool compare(const NameNumber& a, const NameNumber& b) {
    if (a.number == b.number) {
        return a.name < b.name;
    }
    return a.number > b.number;
}

// Fu
void sortFile(const std::string& filename) {
    // Use CreateFile to open or create the file
    HANDLE hFile = CreateFile(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        // If the file does not exist, create it
        hFile = CreateFile(filename.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            MessageBox(nullptr, ("Failed to open or create file " + filename).c_str(), "Error", MB_ICONERROR);
            return;
        }
    }

    // Read the file content into a buffer
    DWORD bytesRead;
    char buffer[1024];
    std::vector<std::string> lines;
    while (ReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        std::istringstream ss(buffer);
        std::string line;
        while (std::getline(ss, line)) {
            lines.push_back(line);
        }
    }
    CloseHandle(hFile);

    // Sort the lines
    std::vector<NameNumber> data;
    for (const auto& line : lines) {
        data.emplace_back(line);
    }
    std::sort(data.begin(), data.end(), compare);

    // Write the sorted data back to the file
    hFile = CreateFile(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBox(nullptr, ("Failed to open file " + filename).c_str(), "Error", MB_ICONERROR);
        return;
    }

    for (const auto& item : data) {
        std::string outputLine = item.toString() + "\n";
        DWORD bytesWritten;
        WriteFile(hFile, outputLine.c_str(), outputLine.size(), &bytesWritten, NULL);
    }
    CloseHandle(hFile);
}



