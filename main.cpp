#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>
#include <commctrl.h>
#include "resource.h"

static BOOL get_text_size(HWND hwnd, LPSIZE size)
{
    int length = (int)SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0);
    if (!length) return false;

    LPCWSTR buf = new WCHAR[length + 1];
    SendMessage(hwnd, WM_GETTEXT, length + 1, (LPARAM)buf);

    return GetTextExtentPoint32W(GetDC(hwnd), buf, length, size);
}

constexpr WORD MARGIN = 8;
constexpr WORD PADDING = 4;

/**************************************************************************************************
 * Text editor
 *************************************************************************************************/

 // these are used to ID the controls/windows, and are used to execute an event (e.g. click)
#define IDM_EDUNDO  111
#define IDM_EDCUT   112
#define IDM_EDCOPY  113
#define IDM_EDPASTE 114
#define IDM_EDDEL   115

#define ID_TABCTRL   101
#define ID_EDITCHILD 100

LRESULT CALLBACK WindowProcEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    static HWND hwndTab;
    static HWND hwndConsole;
    static HWND hwndInput;
    static HWND hwndSend;

    switch (uMsg)
    {
        case WM_CREATE:
        {
            //INITCOMMONCONTROLSEX icex;
            //icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            //icex.dwICC = ICC_TAB_CLASSES;
            //InitCommonControlsEx(&icex);

            hwndTab = CreateWindowEx(
                0, WC_TABCONTROL,
                NULL,
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0,
                hwnd,
                (HMENU)ID_TABCTRL,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);

            TCITEM tie;
            tie.mask = TCIF_TEXT;
            TCHAR achTemp[] = L"tabtext long long";
            tie.pszText = achTemp;
            tie.dwState = TCIS_BUTTONPRESSED;
            auto count = SendMessageW(hwndTab, TCM_GETITEMCOUNT, 0, 0);
            SendMessageW(hwndTab, TCM_INSERTITEMW, count, (LPARAM)(LPTCITEM)&tie);

            count = SendMessageW(hwndTab, TCM_GETITEMCOUNT, 0, 0);
            SendMessageW(hwndTab, TCM_INSERTITEMW, count, (LPARAM)(LPTCITEM)&tie);



            LPCWSTR lpszLatin = L"Lorem ipsum dolor sit amet, consectetur "
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
                0, WC_EDIT,   // predefined class 
                lpszLatin,         // no window title 
                WS_CHILD | /*WS_VISIBLE |*/ WS_VSCROLL |
                ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                0, 0, 0, 0,   // set size in WM_SIZE message 
                hwnd,         // parent window 
                (HMENU)ID_EDITCHILD,   // edit control ID 
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);        // pointer not needed 

            hwndInput = CreateWindowEx(
                0, WC_EDIT,   // predefined class 
                L"Input text",         // no window title 
                WS_CHILD | WS_VISIBLE | 
                ES_LEFT | ES_AUTOHSCROLL,
                0, 0, 0, 0,   // set size in WM_SIZE message 
                hwnd,         // parent window 
                (HMENU)ID_EDITCHILD,   // edit control ID 
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);        // pointer not needed

            hwndSend = CreateWindowEx(
                0, WC_BUTTON,  // Predefined class; Unicode assumed 
                L"SEND >>",    // Button text 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
                0, 0, 0, 0,   // set size in WM_SIZE message 
                hwnd,         // parent window 
                NULL,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed.
        }
        break;

        case WM_SIZE:
        {
            // Make the edit control the size of the window's client area. 

            SIZE size;
            get_text_size(hwndSend, &size);

            const auto TXTH = size.cy + PADDING;
            const auto BTNW = size.cx + PADDING;

            MoveWindow(hwndSend,
                LOWORD(lParam) - MARGIN - BTNW,     // starting x-
                HIWORD(lParam) - MARGIN - TXTH,// and y-coordinates 
                BTNW,     // width of client area
                TXTH,    // height of client area
                TRUE);                  // repaint window

            MoveWindow(hwndInput,
                MARGIN,     // starting x-
                HIWORD(lParam) - MARGIN - TXTH,// and y-coordinates 
                LOWORD(lParam) - 3 * MARGIN - BTNW,     // width of client area 
                TXTH,    // height of client area 
                TRUE);                  // repaint window 

            MoveWindow(hwndTab,
                MARGIN, MARGIN,                 // starting x- and y-coordinates 
                LOWORD(lParam) - 2 * MARGIN,    // width of client area 
                HIWORD(lParam) - 3 * MARGIN - TXTH,    // height of client area 
                TRUE);                 // repaint window 

            MoveWindow(hwndConsole,
                MARGIN, MARGIN,                 // starting x- and y-coordinates 
                LOWORD(lParam) - 2 * MARGIN,    // width of client area 
                HIWORD(lParam) - 3 * MARGIN - TXTH,    // height of client area 
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Main window";
    wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                                // Optional window styles.
        wc.lpszClassName,                 // Window class
        L"RegExMicro v0.0.1",             // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style

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
