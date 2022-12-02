#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>
//#include <CommCtrl.h>

// these are used to ID the controls/windows, and are used to execute an event (e.g. click)
#define IDM_ABOUT   104
#define IDM_EDUNDO  111
#define IDM_EDCUT   112
#define IDM_EDCOPY  113
#define IDM_EDPASTE 114
#define IDM_EDDEL   115

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    hInst = hInstance;

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"RegExMicro",                  // Window text
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

#define ID_EDITCHILD 100

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    static HWND hwndEdit; 
 
    TCHAR lpszLatin[] =  L"Lorem ipsum dolor sit amet, consectetur "
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

    switch (uMsg)
    {
        case WM_CREATE:
        {
            hwndEdit = CreateWindowEx(
                0, L"EDIT",   // predefined class 
                NULL,         // no window title 
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                0, 0, 0, 0,   // set size in WM_SIZE message 
                hwnd,         // parent window 
                (HMENU)ID_EDITCHILD,   // edit control ID 
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);        // pointer not needed 

            // Add text to the window. 
            SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)lpszLatin);
        }
        break;

        case WM_COMMAND:
        {
            switch (wParam)
            {
            case IDM_EDUNDO:
                // Send WM_UNDO only if there is something to be undone. 

                if (SendMessage(hwndEdit, EM_CANUNDO, 0, 0))
                    SendMessage(hwndEdit, WM_UNDO, 0, 0);
                else
                {
                    MessageBox(hwndEdit,
                        L"Nothing to undo.",
                        L"Undo notification",
                        MB_OK);
                }
                break;

            case IDM_EDCUT:
                SendMessage(hwndEdit, WM_CUT, 0, 0);
                break;

            case IDM_EDCOPY:
                SendMessage(hwndEdit, WM_COPY, 0, 0);
                break;

            case IDM_EDPASTE:
                SendMessage(hwndEdit, WM_PASTE, 0, 0);
                break;

            case IDM_EDDEL:
                SendMessage(hwndEdit, WM_CLEAR, 0, 0);
                break;

            case IDM_ABOUT:
                DialogBox(hInst,                // current instance 
                    L"AboutBox",           // resource to use 
                    hwnd,                 // parent handle 
                    MB_OK);
                break;
            }
        }
        break;

        case WM_SETFOCUS:
        {
            SetFocus(hwndEdit);
        }
        break;

        case WM_SIZE:
        {
            // Make the edit control the size of the window's client area. 

            MoveWindow(hwndEdit,
                0, 0,                  // starting x- and y-coordinates 
                LOWORD(lParam),        // width of client area 
                HIWORD(lParam),        // height of client area 
                TRUE);                 // repaint window 
        }
        break;

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
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
        }
        break;

        default:
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return result;
}
