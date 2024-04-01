#ifndef DRAWSERVICE_H
#define DRAWSERVICE_H

#include <windows.h>
#include <vector>
#include <string>
#include "PlacementService.h"

using namespace std;
extern int Computer_isHorizontal;
extern int Side;
void DrawGrid(HDC& hdc, vector<vector<int>>& Board_Cells, int& x, int& y, const int& width, const int& height, double& cellSize, const bool& drawCursor, int cursorX, int cursorY, int& cursorLength, bool isManual, bool isHorizontal);
void UpdateCursor(int dx, int dy, bool& isHorizontal, int& cursorX, int& cursorY, int& cursorLength, const int& gridWidth, const int& gridHeight);
void UpdateBoard(HWND& hwnd, vector<vector<int>>& Board_Cells, int x, int y, vector<Ships>& Board_Ships, int& Num_Ships, int LENGTH_SIZE, int WIDTH_SIZE, int Turn, bool& Shipsunk);
#endif 
