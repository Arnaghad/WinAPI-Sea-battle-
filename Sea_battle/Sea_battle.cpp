#include <windows.h>
#include <Windowsx.h>
#include <vector>
#include <string>
#include <regex>
#include <math.h>
#include "Resource.h"
#include "DialogService.h"
#include "Textbox.h"
#include "PlacementService.h"
#include "DrawService.h"
#include "ComputerService.h"

using namespace std;

WNDPROC DefEditProc;
const int gridWidth = 10;
const int gridHeight = 10;
vector<vector<int>> ComputerBoard_Cells(gridWidth, vector<int>(gridHeight));
static double cellSize = 20;
vector<vector<int>> PlayerBoard_Cells(gridWidth, vector<int>(gridHeight));
const int ShipSize[10]{ 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
vector<Ships> PlayerShips(10);
vector<Ships> ComputerShips(10);
int PlayerNum_Ships = 0;
int ComputerNum_Ships = 10;
HWND hwndEdit;
bool isPlaying = false;
bool isManual = false;
static int cursorX = 0; // Initial cursor position
static int cursorY = 0;
static int cursorLength = 1;
bool isHorizontal = true;
int Side;
bool Shipsunk = true;
HWND hwndButtons[4];
HWND hwndListBox;
int hit_deck = 0;
int computerX, computerY;
int first_X, first_Y;
int Computer_isHorizontal = -1;
int Points = 0;

bool CheckAllZero(HWND hwnd) {
	bool allZero = true;
	for (int i = 0; i < 4; ++i)
	{
		HWND hwndCount = GetDlgItem(hwnd, 10 + i);
		char countText[5];
		GetWindowText(hwndCount, countText, sizeof(countText));
		int currentCount = atoi(countText);
		if (currentCount > 0)
		{
			allZero = false;
			break;
		}
	}
	return allZero;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int margin = 20;
	int Computer_PaddingX = 640 - margin - gridWidth * cellSize;
	int paddingY = margin;
	int windowWidth = 640;
	int windowHeight = 480;
	int Player_PaddingX = margin; // This will position the non-interactive grid right next to the left side of the window
	// Off-screen buffer
	static HWND hwndSeaBattle;
	static HWND hwndPlayButton;
	static HWND hwndQuitButton;

	if ((PlayerNum_Ships == 0) && (isPlaying)) {
		MessageBox(hwnd, "Computer Wins", "Turn", MB_OK | MB_ICONSTOP);
	}
	if ((ComputerNum_Ships == 0) && (isPlaying)) {
		MessageBox(hwnd, "Player Wins", "Turn", MB_OK | MB_ICONSTOP);
	}

	switch (msg)
	{
	case WM_CREATE:
	{
		// Calculate the center coordinates for the Play button
		int buttonWidth = 100;
		int buttonHeight = 30;
		int buttonX = (640 - buttonWidth) / 2; // Adjust the values based on your window size
		int buttonY = (480 - buttonHeight) / 2; // Adjust the values based on your window size
		int quitButtonWidth = 100;
		int quitButtonHeight = 30;
		int quitButtonX = (640 - quitButtonWidth) / 2; // Adjust the values based on your window size
		int quitButtonY = buttonY + buttonHeight + 10; // Adjust the values based on your layout

		hwndQuitButton = CreateWindow("BUTTON", "Quit", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, quitButtonX, quitButtonY, quitButtonWidth, quitButtonHeight, hwnd, (HMENU)3, GetModuleHandle(NULL), NULL);
		hwndPlayButton = CreateWindow("BUTTON", "Play", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, buttonX, buttonY, buttonWidth, buttonHeight, hwnd, (HMENU)2, GetModuleHandle(NULL), NULL);

		// Create the "Sea Battle" text at the top
		hwndSeaBattle = CreateWindow("STATIC", "Sea Battle", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 640, 60, hwnd, NULL, GetModuleHandle(NULL), NULL);
		HFONT hFont = CreateFont(40, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
		SendMessage(hwndSeaBattle, WM_SETFONT, WPARAM(hFont), TRUE);
		break;
	}

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		return (int)GetStockObject(NULL_BRUSH);
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == 2) // Play button clicked
		{
			int ShipPlacement = DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_SHIP_PLACEMENT_DIALOG), hwnd, DialogProc);
			if (ShipPlacement != IDCANCEL)
			{
				ShowWindow(hwndPlayButton, SW_HIDE);
				ShowWindow(hwndQuitButton, SW_HIDE);
				ShowWindow(hwndSeaBattle, SW_HIDE);
				if (ShipPlacement == IDC_MANUAL_BUTTON)
				{
					isManual = true;
					// Create buttons in original order
					for (int i = 0; i < 4; ++i)
					{
						hwndButtons[i] = CreateWindow("BUTTON", to_string(i + 1).c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, Player_PaddingX + i * 50, paddingY + gridHeight * cellSize + 30, 50, 30, hwnd, (HMENU)(4 + i), GetModuleHandle(NULL), NULL);
					}
					for (int i = 0; i < 4; ++i) // Change the loop to start from 0
					{
						// Create static control for each ship length
						HWND hwndCount = CreateWindow("STATIC", to_string(4 - i).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER, Player_PaddingX + i * 50, paddingY + gridHeight * cellSize + 90, 50, 20, hwnd, NULL, GetModuleHandle(NULL), NULL);
						// Set unique identifier for each static control
						SetWindowLongPtr(hwndCount, GWLP_ID, 10 + i); // Set ID for reference later (13 is the starting ID for the reverse order)
					}
					HWND ConfirmButton = CreateWindow("BUTTON", "Confirm", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, windowWidth - 80, windowHeight - 80, 60, 30, hwnd, (HMENU)(100), GetModuleHandle(NULL), NULL);
					EnableWindow(ConfirmButton, FALSE);
				}
				else if (ShipPlacement == IDC_RANDOM_BUTTON) {
					PlayerNum_Ships = 10;
					RandomPlacement(PlayerBoard_Cells, ShipSize, PlayerNum_Ships, 0, gridHeight, gridWidth, PlayerShips);
					RandomPlacement(ComputerBoard_Cells, ShipSize, ComputerNum_Ships, 1, gridHeight, gridWidth, ComputerShips);
					hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, Computer_PaddingX, paddingY + gridHeight * cellSize + 10, gridWidth * cellSize, 20, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
					DefEditProc = (WNDPROC)SetWindowLongPtr(hwndEdit, GWLP_WNDPROC, (LONG_PTR)EditProc);
					isPlaying = true;
					hwndListBox = CreateWindowEx(0, "LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
						20, 240, 200, 200, hwnd, NULL, GetModuleHandle(NULL), NULL);
				}
				InvalidateRect(hwnd, NULL, FALSE);
				SetFocus(hwnd);
			}
		}
		else if (LOWORD(wParam) == 3 && !isPlaying) // Quit button clicked when not playing
		{
			// Display a confirmation dialog
			int result = MessageBox(hwnd, "Are you sure you want to quit?", "Quit Confirmation", MB_YESNO | MB_ICONQUESTION);

			if (result == IDYES)
			{
				// User clicked Yes, close the program
				DestroyWindow(hwnd);
			}
			// If the user clicked No, do nothing
			SetFocus(hwnd);
		}
		else if (LOWORD(wParam) >= 4 && LOWORD(wParam) <= 7) // Cursor length buttons clicked
		{
			// Set the cursor length to the button number
			cursorLength = LOWORD(wParam) - 3;
			InvalidateRect(hwnd, NULL, FALSE);
			SetFocus(hwnd);
		}
		else if (LOWORD(wParam) == 100) // Confirm button clicked
		{
			isManual = false;
			isPlaying = true;
			hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, Computer_PaddingX, paddingY + gridHeight * cellSize + 10, gridWidth * cellSize, 20, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);
			DefEditProc = (WNDPROC)SetWindowLongPtr(hwndEdit, GWLP_WNDPROC, (LONG_PTR)EditProc);

			for (int i = 0; i < 4; ++i)
			{
				ShowWindow(hwndButtons[i], SW_HIDE);
				ShowWindow(GetDlgItem(hwnd, 10 + i), SW_HIDE);
			}
			RandomPlacement(ComputerBoard_Cells, ShipSize, ComputerNum_Ships, 1, gridHeight, gridWidth, ComputerShips);
			hwndListBox = CreateWindowEx(0, "LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_STANDARD | LBS_NOTIFY,
				20, 240, 200, 200, hwnd, NULL, GetModuleHandle(NULL), NULL);
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:
			UpdateCursor(-1, 0, isHorizontal, cursorX, cursorY, cursorLength, gridWidth, gridHeight);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case VK_RIGHT:
			UpdateCursor(1, 0, isHorizontal, cursorX, cursorY, cursorLength, gridWidth, gridHeight);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case VK_UP:
			UpdateCursor(0, -1, isHorizontal, cursorX, cursorY, cursorLength, gridWidth, gridHeight);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case VK_DOWN:
			UpdateCursor(0, 1, isHorizontal, cursorX, cursorY, cursorLength, gridWidth, gridHeight);
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case 'Z': // Clockwise rotation
			isHorizontal = !isHorizontal; // Reverse direction first
			// Then check if cursor is too close to the edge after rotation
			if (isHorizontal && cursorX + cursorLength > gridWidth) {
				cursorX = gridWidth - cursorLength;
			}
			else if (!isHorizontal && cursorY + cursorLength > gridHeight) {
				cursorY = gridHeight - cursorLength;
			}
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		case VK_RETURN: // Enter key pressed
			if (isManual)
			{
				// Prompt user to confirm ship placement
				int result = MessageBox(hwnd, ("Do you want to place a " + to_string(cursorLength) + "-deck ship at position " + to_string(cursorX) + ", " + to_string(cursorY) + "?").c_str(), "Ship Placement Confirmation", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES)
				{
					if (cursorLength > 0) // Ensure cursor length is greater than 0
					{
						// Check if stock is available for the current cursor length
						HWND hwndCount = GetDlgItem(hwnd, cursorLength + 9); // Get handle to static control corresponding to current cursor length
						char countText[5]; // Assuming the count is less than 10000
						GetWindowText(hwndCount, countText, sizeof(countText)); // Get the current count text
						int currentCount = atoi(countText); // Convert text to integer
						if (currentCount == 0) // If stock is zero
						{
							MessageBox(hwnd, "Ships of this size are out of stock", "Out of Stock", MB_OK | MB_ICONWARNING);
							break;
						}

						bool isValidPlacement = CheckSurroundingCells(cursorX, cursorY, PlayerBoard_Cells, isHorizontal, cursorLength, gridHeight, gridWidth);

						if ((currentCount > 0) && isValidPlacement)
						{
							currentCount--;
							ManualPlacement(hwnd, cursorX, cursorY, cursorLength, PlayerBoard_Cells, isHorizontal, PlayerShips, PlayerNum_Ships);

							SetWindowText(hwndCount, to_string(currentCount).c_str());

							EnableWindow(hwndButtons[cursorLength - 1], currentCount > 0);
						}
						else
						{
							MessageBox(hwnd, "The ship cannot be placed here. Please choose another position.", "Invalid Placement", MB_OK | MB_ICONERROR);
						}
					}
					InvalidateRect(hwnd, NULL, FALSE);
					if (CheckAllZero(hwnd)) {
						HWND ConfirmButton = GetDlgItem(hwnd, 100);
						EnableWindow(ConfirmButton, TRUE);
					}
				}
				else
				{
					MessageBox(hwnd, "No more ships of this length can be placed.", "Ship Placement Error", MB_OK | MB_ICONERROR);
				}

			}
			break;
		default:
			break;
		}
		break;
	}


	case WM_DESTROY:
	{
		// Delete the custom brush used for the static control
		DeleteObject((HGDIOBJ)GetClassLongPtr(hwndSeaBattle, GCLP_HBRBACKGROUND));

		PostQuitMessage(0);
		break;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// Create an off-screen DC for double-buffering
		HDC hdcBuffer = CreateCompatibleDC(hdc);
		HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
		HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

		// Fill the buffer with the background color
		HBRUSH hBrush = CreateSolidBrush(RGB(211, 211, 211));
		RECT rect;
		GetClientRect(hwnd, &rect);
		FillRect(hdcBuffer, &rect, hBrush);
		DeleteObject(hBrush);

		// Draw the new content
		if (isManual)
		{
			// Draw the Player Board with cursor if manual mode is active
			DrawGrid(hdcBuffer, PlayerBoard_Cells, Player_PaddingX, paddingY, gridWidth, gridHeight, cellSize, true, cursorX, cursorY, cursorLength, isManual, isHorizontal);
		}
		if (isPlaying) {
			DrawGrid(hdcBuffer, PlayerBoard_Cells, Player_PaddingX, paddingY, gridWidth, gridHeight, cellSize, true, cursorX, cursorY, cursorLength, false, isHorizontal);
			DrawGrid(hdcBuffer, ComputerBoard_Cells, Computer_PaddingX, paddingY, gridWidth, gridHeight, cellSize, false, -1, -1, cursorLength, false, true);
		}
		// Transfer the off-screen buffer to the screen
		BitBlt(hdc, 0, 0, windowWidth, windowHeight, hdcBuffer, 0, 0, SRCCOPY);

		// Cleanup: Select the old bitmap back into the off-screen DC
		SelectObject(hdcBuffer, hbmOldBuffer);

		// Delete the off-screen bitmap and DC
		DeleteObject(hbmBuffer);
		DeleteDC(hdcBuffer);

		EndPaint(hwnd, &ps);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		if (isPlaying)
		{
			double x = (GET_X_LPARAM(lParam) - Computer_PaddingX) / cellSize;
			double y = (GET_Y_LPARAM(lParam) - paddingY) / cellSize;

			// Check if the click is within the bounds of the 10x10 grid
			if (floor(x) >= 0 && floor(x) < gridWidth && y >= 0 && y < gridHeight)
			{
				if ((ComputerBoard_Cells[int(x)][int(y)] == 1) || (ComputerBoard_Cells[int(x)][int(y)] == 2)) {
					MessageBox(hwnd, "You already hit this cell", "Turn", MB_OK | MB_ICONWARNING);
				}
				else {
					UpdateBoard(hwnd, ComputerBoard_Cells, int(x), int(y), ComputerShips, ComputerNum_Ships, gridHeight, gridWidth, 0, Shipsunk);
					if (ComputerBoard_Cells[int(x)][int(y)] != 1) {
						Computerturn(hwnd, PlayerBoard_Cells, gridHeight, gridWidth, PlayerShips, Shipsunk, hit_deck);
					}
				}
			}
		}
	}
	break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const char CLASS_NAME[] = "Sample Window Class";
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window
	HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Sea Battle", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL)
	{
		return 0;
	}

	// Calculate screen width and height
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Calculate window position to center it on the screen
	int windowX = (screenWidth - 640) / 2;  // Adjust the window width accordingly
	int windowY = (screenHeight - 480) / 2; // Adjust the window height accordingly

	// Set the window position
	SetWindowPos(hwnd, NULL, windowX, windowY, 640, 480, SWP_SHOWWINDOW);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

