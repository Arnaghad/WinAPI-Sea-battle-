#include <windows.h>
#include <vector>
#include <string>
#include "Resource.h"
#include "DialogService.h"
using namespace std;

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Check if a button was clicked
        switch (wmId)
        {
        case IDC_MANUAL_BUTTON:
        case IDC_RANDOM_BUTTON:
            EndDialog(hwndDlg, wmId); // End the dialog and return the button ID
            return (INT_PTR)TRUE;
        }
    }
    break;

    case WM_CLOSE:
        // Close the dialog box
        EndDialog(hwndDlg, IDCANCEL); // Return IDCANCEL to indicate cancellation
        return (INT_PTR)TRUE; // Indicate that the message is handled

    case WM_DESTROY:
        // Do nothing on destroy
        return (INT_PTR)FALSE;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK CreateFileDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    TCHAR Name[MAX_PATH];
    const TCHAR fileName[] = TEXT("Score"); // Use TEXT macro for Unicode compatibility

    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            GetDlgItemText(hDlg, IDC_NAME, Name, MAX_PATH);

            HANDLE hFile = CreateFile(fileName, // File name
                GENERIC_WRITE, // Open for writing
                0, // Do not share
                NULL, // Default security
                OPEN_ALWAYS, // Open existing file or create new
                FILE_ATTRIBUTE_NORMAL, // Normal file
                NULL); // No template file

            if (hFile != INVALID_HANDLE_VALUE) {
                // Move the cursor to the end of the file
                SetFilePointer(hFile, 0, NULL, FILE_END);

                DWORD written;
                WriteFile(hFile, // Handle to file
                    Name, // Data to write
                    lstrlen(Name) * sizeof(TCHAR), // Number of bytes to write
                    &written, // Number of bytes written
                    NULL); // No overlapped structure

                CloseHandle(hFile); // Always close the handle
            }
            else {
                // Handle CreateFile failure
                MessageBox(hDlg, TEXT("Failed to create or open file!"), TEXT("Error"), MB_OK | MB_ICONERROR);
            }
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

