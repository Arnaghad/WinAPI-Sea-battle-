#ifndef TEXTBOX_H
#define TEXTBOX_H
#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include "PlacementService.h"

using namespace std;

extern WNDPROC DefEditProc;
extern vector<vector<int>> ComputerBoard_Cells;
extern vector<vector<int>> PlayerBoard_Cells;
extern const int gridHeight, gridWidth;
extern vector<Ships> ComputerShips;
extern vector<Ships> PlayerShips;
extern int hit_deck;
extern int ComputerNum_Ships;
extern bool Shipsunk;

LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // EDITPROC_H