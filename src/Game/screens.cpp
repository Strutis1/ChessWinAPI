#include "screens.h"
#include "../Utility/constants.h"
#include "../Utility/gameAssets.h"

Button playButton;
Button loadButton;
Button settingsButton;
Button exitButton;

void Button::init(Gdiplus::Image *img, int x, int y, int w, int h)
{
    image = img;
    rect = {x, y, x + w, y + h};
    hovered = false;
}

void Button::draw(Gdiplus::Graphics &g)
{
    int offsetY = hovered ? -10 : 0;
    int x = rect.left;
    int y = rect.top + offsetY;
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;

    g.DrawImage(image, x, y, w, h);
}

void Button::onHover(HWND hwnd, bool isHovering)
{
    if (hovered != isHovering)
    {
        hovered = isHovering;
        InvalidateRect(hwnd, NULL, FALSE);
    }
}

void drawMainMenu(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    RECT rc;
    GetClientRect(hwnd, &rc);

    static bool initialized = false;
    if (!initialized)
    {
        int btnW = rc.right / 5;
        int btnH = rc.bottom / 10;
        int btnX = (rc.right - btnW) / 2;
        int spacing = btnH + rc.bottom / 30;
        int startY = (rc.bottom - (4 * btnH + 3 * rc.bottom / 30));

        playButton.init(gAssets.menuPlay, btnX, startY + 0 * spacing, btnW, btnH);
        loadButton.init(gAssets.menuLoad, btnX, startY + 1 * spacing, btnW, btnH);
        settingsButton.init(gAssets.menuSettings, btnX, startY + 2 * spacing, btnW, btnH);
        exitButton.init(gAssets.menuExit, btnX, startY + 3 * spacing, btnW, btnH);

        initialized = true;
    }

    graphics.DrawImage(gAssets.menuBg, 0, 0, rc.right, rc.bottom);
    playButton.draw(graphics);
    loadButton.draw(graphics);
    settingsButton.draw(graphics);
    exitButton.draw(graphics);
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
