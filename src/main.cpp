#include "windows.h"
#include "constants.h"
#include "utilz.h"
#include <gdiplus.h>
#include <stdio.h>

HINSTANCE hInst;

HBITMAP background = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        SetTimer(hwnd, Timer::GameLoop, 1000 / FPS, NULL);
        RECT rc;
        GetClientRect(hwnd, &rc);
        background = LoadPngAsHBITMAP(hInst, Resources::Menu::Background, rc.right - rc.left, rc.bottom - rc.top);
        break;
    case WM_TIMER:
        if (wParam == Timer::GameLoop)
        {
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;

    case WM_SIZE:
    {
        if (background)
        {
            DeleteObject(background);
            background = NULL;
        }

        RECT rc;
        GetClientRect(hwnd, &rc);

        if (rc.right > 0 && rc.bottom > 0)
        {
            background = LoadPngAsHBITMAP(
                hInst,
                Resources::Menu::Background,
                rc.right - rc.left,
                rc.bottom - rc.top);
        }

        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Gdiplus::Graphics graphics(hdc);
        if (background)
        {
            RECT rc;
            GetClientRect(hwnd, &rc);
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP old = (HBITMAP)SelectObject(memDC, background);
            BitBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);
            SelectObject(memDC, old);
            DeleteDC(memDC);
        }
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_COMMAND:
        break;
    case WM_DESTROY:
        delete background;
        KillTimer(hwnd, Timer::GameLoop);
        PostQuitMessage(0);
        break;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    hInst = hInstance;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    MSG msg;
    WNDCLASSEXW wc = {0};

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(Resources::IconMain));
    wc.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(Resources::IconMain));
    wc.lpszClassName = L"ChessWinClass";

    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowW(
        wc.lpszClassName, L"Chess Game",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, Window::Width, Window::Height,
        NULL, NULL, hInst, 0);

    if (hwnd == NULL)
    {
        MessageBoxW(NULL, L"Couldn't create window", L"Error", 0);
        return -1;
    }

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}