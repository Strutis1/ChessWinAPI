#include "windows.h"
#include "constants.h"

HINSTANCE hInst;



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_CREATE:
            break;
        case WM_TIMER:
            break;
        case WM_PAINT:
            break;
        case WM_COMMAND:
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
    hInst = hInstance;
    MSG msg;
    WNDCLASSW wc = {0};


    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"ChessWinClass";
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAIN));

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(wc.lpszClassName, L"Chess Game", 
    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | WS_CLIPCHILDREN,
    CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInst, 0);


    if(hwnd == NULL){
        MessageBoxW(NULL, L"Couldn't create window",L"Error", 0);
        return -1;
    }



    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}