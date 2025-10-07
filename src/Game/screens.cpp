#include "screens.h"
#include "../Utility/gameAssets.h"

RECT menuPlayRect = {0};
RECT menuLoadRect = {0};
RECT menuSettingsRect = {0};
RECT menuExitRect = {0};

void drawMainMenu(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    RECT rc;
    GetClientRect(hwnd, &rc);

    graphics.DrawImage(gAssets.menuBg, 0, 0, rc.right, rc.bottom);

    int btnW = rc.right / 5;
    int btnH = rc.bottom / 10;
    int btnX = (rc.right - btnW) / 2;
    int btnY = (rc.bottom - btnH) / 2;
    int spacing = btnH + rc.bottom / 30;
    int startY = (rc.bottom - (4 * btnH + 3 * rc.bottom / 30));

    graphics.DrawImage(gAssets.menuPlay, btnX, startY + 0 * spacing, btnW, btnH);
    graphics.DrawImage(gAssets.menuLoad, btnX, startY + 1 * spacing, btnW, btnH);
    graphics.DrawImage(gAssets.menuSettings, btnX, startY + 2 * spacing, btnW, btnH);
    graphics.DrawImage(gAssets.menuExit, btnX, startY + 3 * spacing, btnW, btnH);

    menuPlayRect = {btnX, startY + 0 * spacing, btnX + btnW, startY + 0 * spacing + btnH};
    menuLoadRect = {btnX, startY + 1 * spacing, btnX + btnW, startY + 1 * spacing + btnH};
    menuSettingsRect = {btnX, startY + 2 * spacing, btnX + btnW, startY + 2 * spacing + btnH};
    menuExitRect = {btnX, startY + 3 * spacing, btnX + btnW, startY + 3 * spacing + btnH};
}

void drawGameScreen(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    RECT rc;
    GetClientRect(hwnd, &rc);
}

void drawGameOver(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    RECT rc;
    GetClientRect(hwnd, &rc);
}
