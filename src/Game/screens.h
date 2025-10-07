#ifndef SCREENS_H
#define SCREENS_H

#include "windows.h"

#include <gdiplus.h>

struct Button
{
    RECT rect = {0, 0, 0, 0};
    Gdiplus::Image *image = nullptr;
    bool hovered = false;

    void init(Gdiplus::Image *img, int x, int y, int w, int h);

    void draw(Gdiplus::Graphics &g);
    void onHover(HWND hwnd, bool state);
};

extern Button playButton, loadButton, settingsButton, exitButton;

void drawMainMenu(HWND hwnd, HDC hdc);   // menu, button, background
void drawGameScreen(HWND hwnd, HDC hdc); // board, background, pieces
void drawGameOver(HWND hwnd, HDC hdc);   // buttons/options to either exit, play again or back to menu

#endif