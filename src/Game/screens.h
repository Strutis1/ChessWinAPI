#ifndef SCREENS_H
#define SCREENS_H

#include "windows.h"

extern RECT menuPlayRect;
extern RECT menuLoadRect;
extern RECT menuSettingsRect;
extern RECT menuExitRect;

void drawMainMenu(HWND hwnd, HDC hdc);   // menu, button, background
void drawGameScreen(HWND hwnd, HDC hdc); // board, background, pieces
void drawGameOver(HWND hwnd, HDC hdc);   // buttons/options to either exit, play again or back to menu

#endif