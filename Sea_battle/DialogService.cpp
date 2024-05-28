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
    char Name[MAX_PATH];
    const char fileName[] = "Score"; // Changed fileName to const

    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            GetDlgItemText(hDlg, IDC_NAME, Name, MAX_PATH);

            FILE* file;
            if (fopen_s(&file, fileName, "a") == 0 && file != NULL) { // Changed file opening mode to "a"
                fprintf(file, Name);
                fclose(file);
            }
            else {
                // Handle fopen_s failure
                MessageBox(hDlg, "Failed to create file!", "Error", MB_OK | MB_ICONERROR);
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
