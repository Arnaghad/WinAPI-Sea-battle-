#include <windows.h>
#include <vector>
#include "DrawService.h"

using namespace std;

void DrawGrid(HDC& hdc, vector<vector<int>>& Board_Cells, int& x, int& y, const int& width, const int& height, double& cellSize, const bool& drawCursor, int cursorX, int cursorY, int& cursorLength, bool isManual, bool isHorizontal)
{
    // Draw grid to off-screen buffer
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            RECT cellRect = { x + i * cellSize, y + j * cellSize, x + (i + 1) * cellSize, y + (j + 1) * cellSize };
            Rectangle(hdc, cellRect.left, cellRect.top, cellRect.right, cellRect.bottom);

            if (Board_Cells[i][j] == 1)
            {
                MoveToEx(hdc, cellRect.left, cellRect.top, NULL);
                LineTo(hdc, cellRect.right, cellRect.bottom);
                MoveToEx(hdc, cellRect.left, cellRect.bottom, NULL);
                LineTo(hdc, cellRect.right, cellRect.top);
            }
            // Draw a bold dot if the cell equals 2
            else if (Board_Cells[i][j] == 2)
            {
                int centerX = (cellRect.left + cellRect.right) / 2;
                int centerY = (cellRect.top + cellRect.bottom) / 2;
                int radius = cellSize / 4; // Adjust the radius as needed
                HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // Create a pen for the bold dot
                HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); // Create a brush for the bold dot
                HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
                SelectObject(hdc, hOldPen);
                SelectObject(hdc, hOldBrush);
                DeleteObject(hPen);
                DeleteObject(hBrush);
            }
            else if ((Board_Cells[i][j] == 'S') && (cursorX != -1)) // Check if cell contains 'S'
            {
                SetBkColor(hdc, RGB(255, 255, 255));
                DrawText(hdc, "S", -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE); // Draw 'S' in cell
            }


            if (drawCursor && isManual)
            {
                if (isHorizontal && i >= cursorX && i < cursorX + cursorLength && j == cursorY)
                {
                    for (int k = 0; k < 2; ++k)
                    {
                        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); // Create blue pen
                        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);       // Select blue pen
                        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(HOLLOW_BRUSH)); // Use a transparent brush for the cursor
                        Rectangle(hdc, cellRect.left + k, cellRect.top + k, cellRect.right - k, cellRect.bottom - k);
                        SelectObject(hdc, hOldPen); // Restore old pen
                        SelectObject(hdc, hOldBrush); // Restore old brush
                        DeleteObject(hPen);          // Delete the pen object
                    }
                }
                else if (!isHorizontal && j >= cursorY && j < cursorY + cursorLength && i == cursorX)
                {
                    for (int k = 0; k < 2; ++k)
                    {
                        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); // Create blue pen
                        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);       // Select blue pen
                        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(HOLLOW_BRUSH)); // Use a transparent brush for the cursor
                        Rectangle(hdc, cellRect.left + k, cellRect.top + k, cellRect.right - k, cellRect.bottom - k);
                        SelectObject(hdc, hOldPen); // Restore old pen
                        SelectObject(hdc, hOldBrush); // Restore old brush
                        DeleteObject(hPen);          // Delete the pen object
                    }
                }
            }

            char label[3];
            sprintf_s(label, "%d", j + 1);
            SetBkColor(hdc, RGB(211, 211, 211));
            TextOut(hdc, x - 20, y + j * cellSize, label, strlen(label));
        }

        // Draw column labels
        char label[2] = { 'A' + i, '\0' };
        SetBkColor(hdc, RGB(211, 211, 211));
        TextOut(hdc, x + i * cellSize, y - 20, label, 1);
    }
}

void UpdateCursor(int dx, int dy, bool& isHorizontal, int& cursorX, int& cursorY, int& cursorLength, const int& gridWidth, const int& gridHeight)
{
    // Update cursor position based on movement
    if (isHorizontal)
    {
        cursorX = max(0, min(gridWidth - cursorLength, cursorX + dx));
        cursorY = max(0, min(gridHeight - 1, cursorY + dy));
    }
    else
    {
        cursorX = max(0, min(gridWidth - 1, cursorX + dx));
        cursorY = max(0, min(gridHeight - cursorLength, cursorY + dy));
    }

    // Ensure cursor remains within bounds after rotating
    if (isHorizontal && cursorX + cursorLength > gridWidth) // Adjusted condition here
    {
        cursorX = gridWidth - cursorLength;
    }
    else if (!isHorizontal && cursorY + cursorLength > gridHeight)
    {
        cursorY = gridHeight - cursorLength;
    }
}

void UpdateBoard(HWND& hwnd, vector<vector<int>>& Board_Cells, int x, int y, vector<Ships>& Board_Ships, int& Num_Ships, int LENGTH_SIZE, int WIDTH_SIZE, int Turn, bool& Shipsunk) {
    vector<int> k (Num_Ships);
    char x_char = static_cast<char>(x + 'A');
    if (Board_Cells[x][y] == 'S') {
        Board_Cells[x][y] = 1; // Mark as hit ship
        if (Turn == 1) {
            Shipsunk = false;
            MessageBox(hwnd, (LPCSTR)("Computer hit a ship (" + string(1, x_char) + ", " + to_string(y + 1) + ")").c_str(), "Update Board", MB_OK);
        }
        else {
            MessageBox(hwnd, (LPCSTR)("You hit a ship (" + string(1, x_char) + ", " + to_string(y + 1) + ")").c_str(), "Update Board", MB_OK);
        }
        for (int i = 0; i < Num_Ships; i++) {
            k[i] = 0;
            if (Board_Ships[i].orientation == 1) {
                for (int j = Board_Ships[i].x_head; j < Board_Ships[i].x_head + Board_Ships[i].size; j++) {
                    if (j < 0 || j >= LENGTH_SIZE) {
                        continue;
                    }
                    if (Board_Cells[j][Board_Ships[i].y_head] == 1) {
                        k[i]++;
                    }
                }
                if (k[i] == Board_Ships[i].size) {
                    MessageBox(hwnd, (LPCSTR)(to_string(Board_Ships[i].size) + "-deck ship was sunk").c_str(), "Update Board", MB_OK);
                    for (int j = Board_Ships[i].x_head; j < Board_Ships[i].x_head + Board_Ships[i].size; j++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                if ((Board_Ships[i].y_head + dy < WIDTH_SIZE) && (j + dx < LENGTH_SIZE) && (Board_Ships[i].y_head + dy > -1) && (j + dx > -1)) {
                                    if ((Board_Cells[j + dx][Board_Ships[i].y_head + dy] != 1) && (Board_Cells[j + dx][Board_Ships[i].y_head + dy] != 'S')) {
                                        Board_Cells[j + dx][Board_Ships[i].y_head + dy] = 2;
                                    }
                                }
                            }
                        }
                    }

                    Board_Ships.erase(Board_Ships.begin() + i);
                    Num_Ships--;
                    if (Turn == 1) {
                        Shipsunk = true;
                        Computer_isHorizontal = -1;
                        Side = -1;
                    }
                }
            }
            else {
                for (int j = Board_Ships[i].y_head; j < Board_Ships[i].y_head + Board_Ships[i].size; j++) {
                    if (j < 0 || j >= WIDTH_SIZE) {
                        continue;
                    }
                    if (Board_Cells[Board_Ships[i].x_head][j] == 1) {
                        k[i]++;
                    }
                }
                if (k[i] == Board_Ships[i].size) {
                    MessageBox(hwnd, (LPCSTR)(to_string(Board_Ships[i].size) + "-deck ship was sunk").c_str(), "Update Board", MB_OK);
                    for (int j = Board_Ships[i].y_head; j < Board_Ships[i].y_head + Board_Ships[i].size; j++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                if ((Board_Ships[i].x_head + dx < LENGTH_SIZE) && (j + dy < WIDTH_SIZE) && (Board_Ships[i].x_head + dx > -1) && (j + dy > -1)) {
                                    if ((Board_Cells[Board_Ships[i].x_head + dx][j + dy] != 1) && (Board_Cells[Board_Ships[i].x_head + dx][j + dy] != 'S')) {
                                        Board_Cells[Board_Ships[i].x_head + dx][j + dy] = 2;
                                    }
                                }
                            }
                        }
                    }
                    Board_Ships.erase(Board_Ships.begin() + i);
                    Num_Ships--;
                    if (Turn == 1) {
                        Shipsunk = true;
                        Computer_isHorizontal = -1;
                        Side = -1;
                    }
                }

            }
        }
    }
    else if (Board_Cells[x][y] != 1) {
        Board_Cells[x][y] = 2;// Mark as miss
        if (Turn == 1) {
            MessageBox(hwnd, (LPCSTR)("Computer miss (" + string(1, x_char) + ", " + to_string(y + 1) + ")").c_str(), "Update Board", MB_OK);
        }
        else {
            MessageBox(hwnd, (LPCSTR)("You miss (" + string(1, x_char) + ", " + to_string(y + 1) + ")").c_str(), "Update Board", MB_OK);
        }
    }
    InvalidateRect(hwnd, NULL, TRUE);
}
