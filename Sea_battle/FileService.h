#ifndef FILESERVICE_H
#define FILESERVICE_H
#include <Windows.h>
void TruncateFileAfterTenLines(const std::wstring& filePath);
BOOL FileExists(const char* filename);
void sortFile(const std::string& filename);
#endif