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

static HDC     gBackDC   = NULL;
static HBITMAP gBackBmp  = NULL;
static HGDIOBJ gBackOld  = NULL;
static int     gBackW    = 0;
static int     gBackH    = 0;

static void DestroyBackBuffer()
{
    if (gBackDC)
    {
        if (gBackOld) SelectObject(gBackDC, gBackOld);
        gBackOld = NULL;

        if (gBackBmp) DeleteObject(gBackBmp);
        gBackBmp = NULL;

        DeleteDC(gBackDC);
        gBackDC = NULL;
    }

    gBackW = 0;
    gBackH = 0;
}

static void CreateBackBuffer(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;

    if (w <= 0 || h <= 0) return;

    if (gBackDC && gBackBmp && w == gBackW && h == gBackH) return;

    DestroyBackBuffer();

    HDC hdc = GetDC(hwnd);

    gBackDC  = CreateCompatibleDC(hdc);
    gBackBmp = CreateCompatibleBitmap(hdc, w, h);
    gBackOld = SelectObject(gBackDC, gBackBmp);

    gBackW = w;
    gBackH = h;

    ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        gameState.init();
        gAssets.load(((LPCREATESTRUCT)lParam)->hInstance);

        CreateBackBuffer(hwnd);

        SetTimer(hwnd, Timer::GameLoop, 1000 / FPS, NULL);
        return 0;
    }

    case WM_SIZE:
    {
        CreateBackBuffer(hwnd);
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }

    case WM_TIMER:
        if (wParam == Timer::GameLoop)
        {
            if (gameState.currentScreen == GameScreen::Playing)
                InvalidateRect(hwnd, NULL, FALSE);
        }
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        CreateBackBuffer(hwnd);

        if (gBackDC && gBackBmp)
        {
            RECT rc;
            GetClientRect(hwnd, &rc);

            HBRUSH bg = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(gBackDC, &rc, bg);
            DeleteObject(bg);

            switch (gameState.currentScreen)
            {
            case GameScreen::MainMenu:
                drawMainMenu(hwnd, gBackDC);
                break;
            case GameScreen::Playing:
                drawGameScreen(hwnd, gBackDC);
                break;
            case GameScreen::GameOver:
                drawGameOver(hwnd, gBackDC);
                break;
            }

            BitBlt(hdc, 0, 0, gBackW, gBackH, gBackDC, 0, 0, SRCCOPY);
        }

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_SETCURSOR:
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return TRUE;

    case WM_LBUTTONDOWN:
    {
        
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

        if (PtInRect(&playButton.rect, pt))
        {
            gameState.currentScreen = GameScreen::Playing;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (PtInRect(&loadButton.rect, pt))
            {
                if (!loadButton.disabled)
                {
                    gameState.currentScreen = GameScreen::Playing;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
        else if (PtInRect(&settingsButton.rect, pt))
        {
            // Settings not implemented yet
        }
        else if (PtInRect(&exitButton.rect, pt))
        {
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        return 0;
    }

    case WM_MOUSEMOVE:
    {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        playButton.onHover(hwnd, PtInRect(&playButton.rect, pt));
        loadButton.onHover(hwnd, PtInRect(&loadButton.rect, pt));
        settingsButton.onHover(hwnd, PtInRect(&settingsButton.rect, pt));
        exitButton.onHover(hwnd, PtInRect(&exitButton.rect, pt));
        return 0;
    }

    case WM_DESTROY:
        KillTimer(hwnd, Timer::GameLoop);
        gAssets.unload();
        DestroyBackBuffer();
        gameState.running = false;
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    hInst = hInstance;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASSEXW wc = { 0 };
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

    if (!hwnd)
    {
        MessageBoxW(NULL, L"Couldn't create window", L"Error", 0);
        return -1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}
