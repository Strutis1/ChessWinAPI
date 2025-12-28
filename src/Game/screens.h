#ifndef SCREENS_H
#define SCREENS_H

#include "windows.h"

#include <gdiplus.h>

struct Button
{
    RECT rect = {0, 0, 0, 0};
    Gdiplus::Image *image = nullptr;
    bool hovered = false;
    bool disabled = false;

    void init(Gdiplus::Image *img, int x, int y, int w, int h);

    void draw(Gdiplus::Graphics &g);
    void onHover(HWND hwnd, bool state);
};

extern Button playButton, loadButton, settingsButton, exitButton;
extern Button retryButton, goExitButton;
extern Button sillyBotButton, backButton;

void drawMainMenu(HWND hwnd, HDC hdc);   // menu, button, background
void drawDifficultyScreen(HWND hwnd, HDC hdc); // difficulty selection
void drawGameScreen(HWND hwnd, HDC hdc); // board, background, pieces
void drawGameOver(HWND hwnd, HDC hdc);   // buttons/options to either exit, play again or back to menu



struct BoardLayout
{
    int originX;
    int originY;
    int squareSize;
    int width() const { return squareSize * 8; }
    int height() const { return squareSize * 8; }

    BoardLayout(int x = 0, int y = 0, int size = 60)
        : originX(x), originY(y), squareSize(size) {}
};

extern BoardLayout currentBoardLayout;

void updateBoardLayout(HWND hwnd);
void drawBoard(HWND hwnd, Gdiplus::Graphics& g, const BoardLayout& layout);
bool pointInBoard(const BoardLayout& layout, int x, int y);



#endif
