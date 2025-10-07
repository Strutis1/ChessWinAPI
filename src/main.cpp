#include "windows.h"
#include <windowsx.h>
#include "Utility/constants.h"
#include "Utility/utilz.h"
#include <gdiplus.h>
#include <stdio.h>
#include "Utility/gameAssets.h"
#include "Game/screens.h"
#include "Game/game.h"

HINSTANCE hInst;

GameState gameState;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        gameState.init();
        gAssets.load(((LPCREATESTRUCT)lParam)->hInstance);
        SetTimer(hwnd, Timer::GameLoop, 1000 / FPS, NULL);
        break;
    case WM_TIMER:
        if (wParam == Timer::GameLoop)
        {
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;

    // case WM_SIZE:
    // {
    //     if (background)
    //     {
    //         DeleteObject(background);
    //         background = NULL;
    //     }

    //     RECT rc;
    //     GetClientRect(hwnd, &rc);

    //     if (rc.right > 0 && rc.bottom > 0)
    //     {
    //         background = LoadPngAsHBITMAP(
    //             hInst,
    //             Resources::Menu::Background,
    //             rc.right - rc.left,
    //             rc.bottom - rc.top);
    //     }

    //     InvalidateRect(hwnd, NULL, TRUE);
    //     break;
    // }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        switch (gameState.currentScreen)
        {
        case GameScreen::MainMenu:
            if (gAssets.menuBg && gAssets.menuPlay && gAssets.menuSettings && gAssets.menuLoad && gAssets.menuExit)
            {
                drawMainMenu(hwnd, hdc);
            }
            break;
        case GameScreen::Playing:
            if (1)
            {
                drawGameScreen(hwnd, hdc);
            }
            break;
        case GameScreen::GameOver:
            if (1)
            {
                drawGameOver(hwnd, hdc);
            }
            break;
        }
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_SETCURSOR:
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return TRUE;
    case WM_LBUTTONDOWN:
    {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        if (PtInRect(&menuPlayRect, pt))
        {
            gameState.currentScreen = GameScreen::Playing;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (PtInRect(&menuExitRect, pt))
        {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        break;
    }
    // handle button jump or glow or whatever
    case WM_MOUSEMOVE:
    {
    }

    case WM_COMMAND:
        break;
    case WM_DESTROY:
        KillTimer(hwnd, Timer::GameLoop);
        gAssets.unload();
        gameState.running = false; // pretty sure that does nothing because its already shutting down
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
        (WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX)) | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        Window::Width, Window::Height,
        NULL, NULL, hInst, 0);
    ShowCursor(TRUE);

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