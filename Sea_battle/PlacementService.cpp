#include "PlacementService.h"
#include <windows.h>
#include <time.h>

void ManualPlacement(HWND& hwnd, int& cursorX, int& cursorY, int& cursorLength, vector<vector<int>>& PlayerBoard_Cells, bool& isHorizontal, vector<Ships>& PlayerShips, int& Cnt_Player_Ships)
{
    PlayerShips[Cnt_Player_Ships].x_head = cursorX;
    PlayerShips[Cnt_Player_Ships].y_head = cursorY;
    PlayerShips[Cnt_Player_Ships].size = cursorLength;
    PlayerShips[Cnt_Player_Ships].orientation = isHorizontal;
    if (isHorizontal)
    {
        for (int i = cursorX; i < cursorX + cursorLength; ++i)
        {
            PlayerBoard_Cells[i][cursorY] = 'S';
        }
    }
    else
    {
        for (int i = cursorY; i < cursorY + cursorLength; ++i)
        {
            PlayerBoard_Cells[cursorX][i] = 'S';
        }
    }
    Cnt_Player_Ships++;
    InvalidateRect(hwnd, NULL, FALSE);
}

void RandomPlacement(vector<vector<int>>& Board_Cells, const int shipSizes[], int NUM_SHIPS, int flag, int LENGTH_SIZE, int WIDTH_SIZE, vector<Ships>& Board_Ships) {
    srand((unsigned)time(0) + flag);
    for (int i = 0; i < NUM_SHIPS; i++) {
        int x1, y1;
        int size_Ship = shipSizes[i];
        bool isHorizontal;
        x1 = rand() % WIDTH_SIZE;
        y1 = rand() % LENGTH_SIZE;
        isHorizontal = rand() % 2;
        bool isShipPlaced = false;
        while (!isShipPlaced) {
            bool isValidPlacement = true;
            if (((x1 + (isHorizontal == true) * shipSizes[i]) > WIDTH_SIZE) || ((y1 + (isHorizontal == false) * shipSizes[i]) > LENGTH_SIZE)) {
                isValidPlacement = false;
            }
            if (!CheckSurroundingCells(x1, y1, Board_Cells, isHorizontal, shipSizes[i], LENGTH_SIZE, WIDTH_SIZE)) {
                isValidPlacement = false;
            }
            if (isValidPlacement) {
                Board_Ships[i].x_head = x1;
                Board_Ships[i].y_head = y1;
                Board_Ships[i].size = shipSizes[i];
                Board_Ships[i].orientation = isHorizontal;
                for (int j = 0; j < size_Ship; j++) {
                    int x = x1 + (isHorizontal == 1) * j;
                    int y = y1 + (isHorizontal == 0) * j;
                    Board_Cells[x][y] = 'S';
                }
                isShipPlaced = true;
            }
            else {
                x1 = rand() % WIDTH_SIZE;
                y1 = rand() % LENGTH_SIZE;
                isHorizontal = rand() % 2;
            }
        }

    }
}

bool CheckSurroundingCells(int x, int y, vector<vector<int>> Board_Cells, bool isHorizontal, int size, int gridHeight, int gridWidth)
{
    bool isValidPlacement = true;
    for (int j = 0; j < size; j++) {
        int x1 = x + (isHorizontal == true) * j;
        int y1 = y + (isHorizontal == false) * j;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int nx = x1 + dx;
                int ny = y1 + dy;
                if ((nx >= 0) && (ny >= 0) && (nx < gridWidth) && (ny < gridHeight)) {
                    if (Board_Cells[nx][ny] == 'S') {
                        isValidPlacement = false;
                    }
                }
            }
        }
    }
    return isValidPlacement;
}


