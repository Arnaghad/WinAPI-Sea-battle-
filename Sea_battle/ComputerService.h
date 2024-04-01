#ifndef COMPUTERSERVICE_H
#define COMPUTERSERVICE_H

#include <windows.h>
#include <vector>
#include <string>
#include "PlacementService.h"
using namespace std;
extern int PlayerNum_Ships;
extern int computerX, computerY;
extern int first_X, first_Y;
extern int Computer_isHorizontal;
extern int Side;
void Computerturn(HWND& hwnd, vector<vector<int>>& Player_board, int LENGTH_SIZE, int WIDTH_SIZE, vector<Ships>& Player_Ships, bool& Shipsunk, int& hit_deck);
#endif