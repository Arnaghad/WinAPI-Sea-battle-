#include <windows.h>
#include <vector>
#include <string>
#include "Resource.h"
#include "DialogService.h"

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

