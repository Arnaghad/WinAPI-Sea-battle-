#include "Textbox.h"
#include "StringService.h"
#include "DrawService.h"
#include "ComputerService.h"

LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_RETURN)
        {
            char x = ' ';
            int y = 0;
            int len = GetWindowTextLength(hwnd);
            if (len > 0)
            {
                char* buf;
                buf = (char*)GlobalAlloc(GPTR, len + 1);
                GetWindowText(hwnd, buf, len + 1);
                string coordinate = buf;
                bool flag = false; // Initialising flag as false.
                for (int i = 0; i < coordinate.length(); i++) {
                    if (isdigit(coordinate[i])) {
                        flag = true;
                        break;
                    }
                }
                if ((coordinate.length() < 4) && flag) {
                    auto result = split_string(coordinate);
                    x = result.first;
                    y = result.second;
                    if (x >= 'A' && x <= 'J' && y >= 1 && y <= 10)
                    {
                        // Valid coordinate format
                        MessageBox(GetParent(hwnd), buf, "Text Box Content", MB_OK | MB_ICONINFORMATION);

                        HWND Parenthwnd = GetParent(hwnd);
                        if ((ComputerBoard_Cells[x - 'A'][y - 1] == 1) || (ComputerBoard_Cells[int(x)][int(y)] == 2)) {
                            MessageBox(hwnd, "You already hit this cell", "Turn", MB_OK | MB_ICONWARNING);
                        }
                        else {
                            UpdateBoard(hwnd, ComputerBoard_Cells, x - 'A', int(y), ComputerShips, ComputerNum_Ships, gridHeight, gridWidth, 0, Shipsunk);
                            if (ComputerBoard_Cells[x - 'A'][y - 1] != 1) {
                                Computerturn(hwnd, PlayerBoard_Cells, gridHeight, gridWidth, PlayerShips, Shipsunk, hit_deck);
                            }
                        }
                        if (PlayerNum_Ships == 0) {
                            MessageBox(hwnd, "Computer Wins", "Turn", MB_OK | MB_ICONSTOP);
                            PostQuitMessage(0);
                            break;
                        }
                        if (ComputerNum_Ships == 0) {
                            MessageBox(hwnd, "Player Wins", "Turn", MB_OK | MB_ICONSTOP);
                            PostQuitMessage(0);
                            break;
                        }
                    }
                    else
                    {
                        // Invalid coordinate format
                        MessageBox(GetParent(hwnd), "Invalid coordinate format", "Error", MB_OK | MB_ICONERROR);
                    }

                    GlobalFree((HANDLE)buf);
                }
                else {
                    MessageBox(GetParent(hwnd), "Invalid coordinate format", "Error", MB_OK | MB_ICONERROR);
                }
            }
            else
            {
                // Text box is empty
                MessageBox(GetParent(hwnd), "Text box is empty", "Error", MB_OK | MB_ICONERROR);
            }
            return 0;
        }
        break;


    case WM_CHAR:
        if (wParam == VK_RETURN)
        {
            // Ignore Enter key
            SetWindowText(hwnd, TEXT(""));
            return 0;
        }
        break;

    }
    return CallWindowProc(DefEditProc, hwnd, msg, wParam, lParam);
}