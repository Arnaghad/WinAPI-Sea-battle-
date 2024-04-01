#ifndef PLACEMENTSERVICE_H
#define PLACEMENTSERVICE_H

#include <windows.h>
#include <vector>
#include <string>
using namespace std;

struct Ships {
    int x_head;
    int y_head;
    int size;
    int orientation;
};

void ManualPlacement(HWND &hwnd, int &cursorX, int &cursorY, int &cursorLength, vector<vector<int>> &Board_Cells, bool &isHorizontal, vector<Ships> &PlayerShips, int& Cnt_Player_Ships);
bool CheckSurroundingCells(int x, int y, vector<vector<int>> Board_Cells, bool isHorizontal, int size, int gridHeight, int gridWidth);
void RandomPlacement(vector<vector<int>>& Board_Cells, const int shipSizes[], int NUM_SHIPS, int flag, int LENGTH_SIZE, int WIDTH_SIZE, vector<Ships>& Board_Ships);
#endif 

