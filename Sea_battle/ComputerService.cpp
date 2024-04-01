#include "PlacementService.h"
#include "ComputerService.h"
#include <time.h>
#include "DrawService.h"

void Computerturn(HWND& hwnd, vector<vector<int>>& Player_board, int LENGTH_SIZE, int WIDTH_SIZE, vector<Ships>& Player_Ships, bool& Shipsunk, int& hit_deck) {
	int Previous_orientation, Previous_computerX, Previous_computerY;
	srand((unsigned)time(NULL) + 2);
	if (Shipsunk == true) {
		do {
			hit_deck = 0;
			do {
				computerX = rand() % WIDTH_SIZE;
				computerY = rand() % LENGTH_SIZE;
			} while (Player_board[computerX][computerY] == 1 || Player_board[computerX][computerY] == 2);
			UpdateBoard(hwnd, Player_board, computerX, computerY, Player_Ships, PlayerNum_Ships, LENGTH_SIZE, WIDTH_SIZE, 1, Shipsunk);
			if (Player_board[computerX][computerY] == 1) {
				hit_deck++;
				first_X = computerX;
				first_Y = computerY;
				Computer_isHorizontal = rand() % 2;
				Side = rand() % 2;
			}
		} while ((Shipsunk == true) && (hit_deck == 1));
	}
	if ((Shipsunk == false)) {
		Previous_computerX = computerX;
		Previous_computerY = computerY;
		do {
			if ((Computer_isHorizontal == 0)) {
				if (Side == 0) {
					computerY--;
				}
				else {
					if (Side == 1) {
						computerY++;
					}
				}
			}
			else {
				if (Side == 0) {
					computerX--;
				}
				else {
					if (Side == 1) {
						computerX++;
					}
				}
			}
			if (((computerX < 0) || (computerX > WIDTH_SIZE - 1) || (computerY < 0) || (computerY > LENGTH_SIZE - 1)) && (hit_deck == 1)) {
				Computer_isHorizontal = rand() % 2;
				Side = rand() % 2;
				computerX = Previous_computerX;
				computerY = Previous_computerY;
			}
			else if (((computerX < 0) || (computerX > WIDTH_SIZE - 1) || (computerY < 0) || (computerY > LENGTH_SIZE - 1)) && (hit_deck > 1)) {
				Side = 1 - Side;
				computerX = first_X;
				computerY = first_Y;
			}

			if ((Player_board[computerX][computerY] == 2) && (hit_deck == 1)) {
				Computer_isHorizontal = rand() % 2;
				Side = rand() % 2;
				computerX = Previous_computerX;
				computerY = Previous_computerY;
			}
			else if ((Player_board[computerX][computerY] == 2) && (hit_deck > 1)) {
				Side = 1 - Side;
				computerX = first_X;
				computerY = first_Y;
			}

			if (Player_board[computerX][computerY] != 1) {
				UpdateBoard(hwnd, Player_board, computerX, computerY, Player_Ships, PlayerNum_Ships, LENGTH_SIZE, WIDTH_SIZE, 1, Shipsunk);
				if (Player_board[computerX][computerY] == 1) {
					hit_deck++;
				}
				if (Player_board[computerX][computerY] == 2) {
					computerX = Previous_computerX;
					computerY = Previous_computerY;
					break;
				}
			}
			if (Shipsunk == true) {
				do {
					computerX = rand() % WIDTH_SIZE;
					computerY = rand() % LENGTH_SIZE;
				} while (Player_board[computerX][computerY] == 1 || Player_board[computerX][computerY] == 2);
				UpdateBoard(hwnd, Player_board, computerX, computerY, Player_Ships, PlayerNum_Ships, LENGTH_SIZE, WIDTH_SIZE, 1, Shipsunk);
				if (Player_board[computerX][computerY] == 1) {
					hit_deck = 1;
					first_X = computerX;
					first_Y = computerY;
					Computer_isHorizontal = rand() % 2;
					Side = rand() % 2;
				}
			}
			Previous_computerX = computerX;
			Previous_computerY = computerY;
		} while (Player_board[computerX][computerY] != 2);
		if ((Player_board[computerX][computerY] == 2) && (Shipsunk == false)) {
			if (hit_deck == 1) {
				Computer_isHorizontal = rand() % 2;
				Side = rand() % 2;
				computerX = Previous_computerX;
				computerY = Previous_computerY;
				if (Computer_isHorizontal == 1) {
					computerX += (Side == 1) ? 1 : -1;
				}
				else {
					computerY += (Side == 1) ? 1 : -1;
				}
			}
			else if (hit_deck > 1) {
				Side = 1 - Side;
				computerX = first_X;
				computerY = first_Y;
				if (Computer_isHorizontal == 1) {
					computerX += (Side == 1) ? 1 : -1;
				}
				else {
					computerY += (Side == 1) ? 1 : -1;
				}
			}
		}
	}
}