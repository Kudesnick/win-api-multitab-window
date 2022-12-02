#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>

/**************************************************************************************************
 * Text editor
 *************************************************************************************************/

 // these are used to ID the controls/windows, and are used to execute an event (e.g. click)
#define IDM_EDUNDO  111
#define IDM_EDCUT   112
#define IDM_EDCOPY  113
#define IDM_EDPASTE 114
#define IDM_EDDEL   115

#define ID_EDITCHILD 100

LRESULT CALLBACK WindowProcEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    static HWND hwndConsole;
    static HWND hwndInput;

    switch (uMsg)
    {
        case WM_CREATE:
        {
            TCHAR lpszLatin[] = L"Lorem ipsum dolor sit amet, consectetur "
                L"adipisicing elit, sed do eiusmod tempor "
                L"incididunt ut labore et dolore magna "
                L"aliqua. Ut enim ad minim veniam, quis "
                L"nostrud exercitation ullamco laboris nisi "
                L"ut aliquip ex ea commodo consequat. Duis "
                L"aute irure dolor in reprehenderit in "
                L"voluptate velit esse cillum dolore eu "
                L"fugiat nulla pariatur. Excepteur sint "
                L"occaecat cupidatat non proident, sunt "
                L"in culpa qui officia deserunt mollit "
                L"anim id est laborum.";

            hwndConsole = CreateWindowEx(
                0, L"EDIT",   // predefined class 
                NULL,         // no window title 
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | /*WS_HSCROLL |*/
                ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                0, 0, 0, 0,   // set size in WM_SIZE message 
                hwnd,         // parent window 
                (HMENU)ID_EDITCHILD,   // edit control ID 
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);        // pointer not needed 

            hwndInput = CreateWindowEx(
                0, L"EDIT",   // predefined class 
                NULL,         // no window title 
                WS_CHILD | WS_VISIBLE | 
                ES_LEFT | ES_AUTOHSCROLL,
                0, 0, 0, 0,   // set size in WM_SIZE message 
                hwnd,         // parent window 
                (HMENU)ID_EDITCHILD,   // edit control ID 
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);        // pointer not needed

            // Add text to the window. 
            SendMessage(hwndConsole, WM_SETTEXT, 0, (LPARAM)lpszLatin);
            SendMessage(hwndInput, WM_SETTEXT, 0, (LPARAM)L"Input text");
        }
        break;

        case WM_COMMAND:
        {
            switch (wParam)
            {
            case IDM_EDUNDO:
                // Send WM_UNDO only if there is something to be undone. 

                if (SendMessage(hwndConsole, EM_CANUNDO, 0, 0))
                    SendMessage(hwndConsole, WM_UNDO, 0, 0);
                else
                {
                    MessageBox(hwndConsole,
                        L"Nothing to undo.",
                        L"Undo notification",
                        MB_OK);
                }
                break;

            case IDM_EDCUT:
                SendMessage(hwndConsole, WM_CUT, 0, 0);
                break;

            case IDM_EDCOPY:
                SendMessage(hwndConsole, WM_COPY, 0, 0);
                break;

            case IDM_EDPASTE:
                SendMessage(hwndConsole, WM_PASTE, 0, 0);
                break;

            case IDM_EDDEL:
                SendMessage(hwndConsole, WM_CLEAR, 0, 0);
                break;
            }
        }
        break;

        case WM_SETFOCUS:
        {
            SetFocus(hwndConsole);
        }
        break;

        case WM_SIZE:
        {
            // Make the edit control the size of the window's client area. 

            MoveWindow(hwndInput,
                4,     // starting x-
                HIWORD(lParam) - 4 - 18,// and y-coordinates 
                LOWORD(lParam) - 8,     // width of client area 
                HIWORD(lParam) - 23,    // height of client area 
                TRUE);                  // repaint window 

            MoveWindow(hwndConsole,
                4, 4,                  // starting x- and y-coordinates 
                LOWORD(lParam) - 8,    // width of client area 
                HIWORD(lParam) - 8  - 4 - 18,    // height of client area 
                TRUE);                 // repaint window 
        }
        break;
    }

    return result;
};

/**************************************************************************************************
 * Main window
 *************************************************************************************************/

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Main window";

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        wc.lpszClassName,               // Window class
        L"RegExMicro v0.0.1",           // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    result = WindowProcEdit(hwnd, uMsg, wParam, lParam);
    if (result) return result;

    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
            EndPaint(hwnd, &ps);
        }
        break;

        default:
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return result;
}
