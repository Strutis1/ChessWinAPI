#include "windows.h"
#include "constants.h"
#include <gdiplus.h>
#include <stdio.h>

HINSTANCE hInst;


using namespace Gdiplus;



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_CREATE:
            break;
        case WM_TIMER:
            break;
        case WM_PAINT:{
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect = {50, 120, 450, 15};
            HBRUSH brush = CreateSolidBrush(RGB(50, 151, 151));
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, brush);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_COMMAND:
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
    hInst = hInstance;

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    MSG msg;
    WNDCLASSEXW wc = {0};

    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(Resources::IconMain));
    wc.hIconSm       = LoadIcon(hInst, MAKEINTRESOURCE(Resources::IconMain));
    wc.lpszClassName = L"ChessWinClass";

    RegisterClassExW(&wc);
    


    HWND hwnd = CreateWindowW(
    wc.lpszClassName, L"Chess Game",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    CW_USEDEFAULT, CW_USEDEFAULT, Window::Width, Window::Height,
    NULL, NULL, hInst, 0);






    if(hwnd == NULL){
        MessageBoxW(NULL, L"Couldn't create window",L"Error", 0);
        return -1;
    }




    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);


    return (int) msg.wParam;
}