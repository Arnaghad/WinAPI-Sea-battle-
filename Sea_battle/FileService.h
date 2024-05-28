#ifndef FILESERVICE_H
#define FILESERVICE_H
#include <Windows.h>
void checkAndEraseLines();
BOOL FileExists(const char* filename);
void sortFile(const std::string& filename);
#endif