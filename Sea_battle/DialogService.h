#ifndef DIALOGSERVICE_H
#define DIALOGSERVICE_H

#include <windows.h>
#include <vector>
#include <string>

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CreateFileDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#endif 
