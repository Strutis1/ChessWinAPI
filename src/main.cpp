#include "windows.h"
#include <windowsx.h>
#include "Utility/constants.h"
#include "Utility/utilz.h"
#include <gdiplus.h>
#include <stdio.h>
#include "Utility/gameAssets.h"
#include "Game/screens.h"
#include "Game/game.h"
#include "Game/appState.h"
#include "../Include/aiLoader.h"

#define WM_AI_MOVE (WM_APP + 1)



HINSTANCE hInst;

AppState appState;
ChessGame chessGame;
BoardLayout currentBoardLayout;


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
        appState.init();
        chessGame.init();
        chessGame.checkForSavedGame();
        gAssets.load(((LPCREATESTRUCT)lParam)->hInstance);

        CreateBackBuffer(hwnd);
        updateBoardLayout(hwnd);

        SetTimer(hwnd, Timer::GameLoop, 1000 / FPS, NULL);
        return 0;
    }

    case WM_SIZE:
    {
        updateBoardLayout(hwnd);

        CreateBackBuffer(hwnd);
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }

    case WM_TIMER:
        if (wParam == Timer::GameLoop)
        {
            if (appState.currentScreen == GameScreen::Playing)
            {
                chessGame.updateTimer();
                chessGame.checkGameOver();//checks if time is up
                InvalidateRect(hwnd, NULL, FALSE);
            }
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

            switch (appState.currentScreen)
            {
            case GameScreen::MainMenu:
                drawMainMenu(hwnd, gBackDC);
                break;
            case GameScreen::DifficultySelect:
                drawDifficultyScreen(hwnd, gBackDC);
                break;
            case GameScreen::ColorSelect:
                drawColorSelectScreen(hwnd, gBackDC);
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
    {
        SetCursor(LoadCursor(NULL, IDC_ARROW));

        POINT pt;
        if (GetCursorPos(&pt) && ScreenToClient(hwnd, &pt))
        {
            if (appState.currentScreen == GameScreen::MainMenu) {
                if (PtInRect(&playButton.rect, pt) ||
                    PtInRect(&loadButton.rect, pt) ||
                    PtInRect(&settingsButton.rect, pt) ||
                    PtInRect(&exitButton.rect, pt)) {
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                    return TRUE;
                }
            } else if (appState.currentScreen == GameScreen::DifficultySelect) {
                if (PtInRect(&sillyBotButton.rect, pt) || PtInRect(&backButton.rect, pt)) {
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                    return TRUE;
                }
            } else if (appState.currentScreen == GameScreen::ColorSelect) {
                if (PtInRect(&whiteButton.rect, pt) ||
                    PtInRect(&blackButton.rect, pt) ||
                    PtInRect(&colorBackButton.rect, pt)) {
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                    return TRUE;
                }
            } else if (appState.currentScreen == GameScreen::Playing) {
                if (pointInBoard(currentBoardLayout, pt.x, pt.y)) {
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                    return TRUE;
                }
            } else if (appState.currentScreen == GameScreen::GameOver) {
                if (PtInRect(&retryButton.rect, pt) || PtInRect(&goExitButton.rect, pt)) {
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                    return TRUE;
                }
            }
        }
        return TRUE; 
    }



    case WM_LBUTTONDOWN:
    {
        if(appState.currentScreen == GameScreen::MainMenu){
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

            if (PtInRect(&playButton.rect, pt))
            {
                appState.currentScreen = GameScreen::DifficultySelect;
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (PtInRect(&loadButton.rect, pt))
                {
                    if (!loadButton.disabled)
                    {
                        chessGame.loadGame();
                        appState.currentScreen = GameScreen::Playing;
                        chessGame.syncTimerWithTurn();
                        InvalidateRect(hwnd, NULL, TRUE);
                        if (chessGame.getCurrentTurn() != appState.playerColor && appState.currentDifficulty == "sillyBot") {
                            PostMessage(hwnd, WM_AI_MOVE, 0, 0);
                        }
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
        else if(appState.currentScreen == GameScreen::DifficultySelect){
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if (PtInRect(&sillyBotButton.rect, pt))
            {
                appState.currentDifficulty = "sillyBot";
                appState.currentScreen = GameScreen::ColorSelect;
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            else if (PtInRect(&backButton.rect, pt))
            {
                appState.currentScreen = GameScreen::MainMenu;
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
        }
        else if(appState.currentScreen == GameScreen::ColorSelect){
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if (PtInRect(&whiteButton.rect, pt))
            {
                appState.playerColor = PieceColor::WHITE;
                chessGame.init();
                appState.hasUnfinishedGame = false;
                appState.currentScreen = GameScreen::Playing;
                chessGame.startNewGameTimer(std::chrono::minutes(10));
                InvalidateRect(hwnd, NULL, TRUE);
                if (chessGame.getCurrentTurn() != appState.playerColor && appState.currentDifficulty == "sillyBot") {
                    PostMessage(hwnd, WM_AI_MOVE, 0, 0);//call bot
                }
                return 0;
            }
            else if (PtInRect(&blackButton.rect, pt))
            {
                appState.playerColor = PieceColor::BLACK;
                chessGame.init();
                appState.hasUnfinishedGame = false;
                appState.currentScreen = GameScreen::Playing;
                chessGame.startNewGameTimer(std::chrono::minutes(10));
                InvalidateRect(hwnd, NULL, TRUE);

                if (chessGame.getCurrentTurn() != appState.playerColor && appState.currentDifficulty == "sillyBot") {
                    PostMessage(hwnd, WM_AI_MOVE, 0, 0);
                }
                return 0;
            }
            else if (PtInRect(&colorBackButton.rect, pt))
            {
                appState.currentScreen = GameScreen::DifficultySelect;
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
        }
        else if(appState.currentScreen == GameScreen::Playing ){
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            if(pointInBoard(currentBoardLayout, pt.x, pt.y)){
                if (chessGame.getCurrentTurn() != appState.playerColor) {
                    return 0;
                }

                int displayX = (pt.x - currentBoardLayout.originX) / currentBoardLayout.squareSize;
                int displayY = (pt.y - currentBoardLayout.originY) / currentBoardLayout.squareSize;
                int boardX = displayToBoardCoord(displayX);
                int boardY = displayToBoardCoord(displayY);
                if(chessGame.getSelectedPiece().type != PieceType::NONE){
                    Move move(chessGame.getSelectedPosX(), chessGame.getSelectedPosY(), boardX, boardY);
                    MoveResult result = chessGame.tryMakeMove(move);
                    chessGame.clearSelectedPiece();
                    chessGame.clearSelectedPosition();
                    if(result.success){
                        InvalidateRect(hwnd, NULL, TRUE);

                        PostMessage(hwnd, WM_AI_MOVE, 0, 0); //call bot

                        return 0;
                    }
                }

                Piece selectedPiece = chessGame.detectSelection(boardX, boardY);
                chessGame.setSelectedPiece(selectedPiece);
                chessGame.setSelectedPosition(boardX, boardY);

                return 0;
            }
        }
        else if(appState.currentScreen == GameScreen::GameOver){
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

            if (PtInRect(&retryButton.rect, pt))
            {
                chessGame.init();
                appState.currentScreen = GameScreen::ColorSelect;
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            else if (PtInRect(&goExitButton.rect, pt))
            {
                chessGame.init();
                appState.currentScreen = GameScreen::MainMenu;
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }

            return 0;
        }
    }

    case WM_MOUSEMOVE:
    {
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

        switch (appState.currentScreen)
        {
        case GameScreen::MainMenu:
            playButton.onHover(hwnd, PtInRect(&playButton.rect, pt));
            loadButton.onHover(hwnd, PtInRect(&loadButton.rect, pt));
            settingsButton.onHover(hwnd, PtInRect(&settingsButton.rect, pt));
            exitButton.onHover(hwnd, PtInRect(&exitButton.rect, pt));
            break;
        case GameScreen::DifficultySelect:
            sillyBotButton.onHover(hwnd, PtInRect(&sillyBotButton.rect, pt));
            backButton.onHover(hwnd, PtInRect(&backButton.rect, pt));
            break;
        case GameScreen::ColorSelect:
            whiteButton.onHover(hwnd, PtInRect(&whiteButton.rect, pt));
            blackButton.onHover(hwnd, PtInRect(&blackButton.rect, pt));
            colorBackButton.onHover(hwnd, PtInRect(&colorBackButton.rect, pt));
            break;
        case GameScreen::GameOver:
            retryButton.onHover(hwnd, PtInRect(&retryButton.rect, pt));
            goExitButton.onHover(hwnd, PtInRect(&goExitButton.rect, pt));
            break;
        default:
            break;
        }
        return 0;
    }

    case WM_AI_MOVE:
{
    if (appState.currentScreen == GameScreen::Playing &&
        !chessGame.isGameOver() &&
        chessGame.getCurrentTurn() != appState.playerColor &&
        appState.currentDifficulty == "sillyBot")
    {
        chessGame.MakeAiMove();
        InvalidateRect(hwnd, NULL, TRUE);
    }
    return 0;
}

    case WM_DESTROY:
    {
        if (appState.currentScreen == GameScreen::Playing && chessGame.isGameOver() == false) {
            if (chessGame.saveGame()) {
                appState.hasUnfinishedGame = true;
            }
        }
        KillTimer(hwnd, Timer::GameLoop);
        gAssets.unload();
        DestroyBackBuffer();
        appState.running = false;
        PostQuitMessage(0);
        return 0;
    }

    default:
        break;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    HMODULE dll = LoadLibraryA("sillyBot.dll");
    if (!dll) {
        MessageBoxW(NULL, L"Failed to load AI DLL", L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    auto fn = (ChooseMoveFromListFn)GetProcAddress(dll, "chooseMoveFromList");
    if (!fn) {
        MessageBoxW(NULL, L"chooseMoveFromList export not found", L"Error", MB_OK | MB_ICONERROR);
        FreeLibrary(dll);
        return 1;
    }

    SetAiChooseMoveFromListFn(fn);
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
    FreeLibrary(dll);
    return (int)msg.wParam;
}
