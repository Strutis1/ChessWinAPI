#include "screens.h"
#include "../Utility/constants.h"
#include "../Utility/gameAssets.h"
#include "game.h"
#include <string>

Button playButton;
Button loadButton;
Button settingsButton;
Button exitButton;

static int RectW(const RECT& r) { return r.right - r.left; }
static int RectH(const RECT& r) { return r.bottom - r.top; }

void Button::init(Gdiplus::Image* img, int x, int y, int w, int h)
{
    image = img;
    rect = { x, y, x + w, y + h };
}


void Button::draw(Gdiplus::Graphics& g)
{
    int x = rect.left;
    int y = rect.top;
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;

    int offsetY = (!disabled && hovered) ? -(h / 10) : 0;
    y += offsetY;

    Gdiplus::RectF rf((Gdiplus::REAL)x, (Gdiplus::REAL)y, (Gdiplus::REAL)w, (Gdiplus::REAL)h);

    Gdiplus::Color fillCol;
    Gdiplus::Color borderCol;

    if (disabled)
    {
        fillCol   = Gdiplus::Color(200, 35, 35, 35);
        borderCol = Gdiplus::Color(200, 80, 80, 80);
    }
    else if (hovered)
    {
        fillCol   = Gdiplus::Color(255, 70, 70, 70);
        borderCol = Gdiplus::Color(255, 200, 200, 200);
    }
    else
    {
        fillCol   = Gdiplus::Color(255, 40, 40, 40);
        borderCol = Gdiplus::Color(255, 130, 130, 130);
    }

    Gdiplus::SolidBrush fill(fillCol);
    Gdiplus::Pen border(borderCol, 2.0f);

    g.FillRectangle(&fill, rf);
    g.DrawRectangle(&border, rf);
}

static RECT GetVisualRect(const Button& b)
{
    RECT r = b.rect;
    int h = r.bottom - r.top;
    int offsetY = (!b.disabled && b.hovered) ? -(h / 10) : 0;
    r.top += offsetY;
    r.bottom += offsetY;
    return r;
}
 
static void DrawCenteredText(Gdiplus::Graphics& g, const RECT& r, const wchar_t* text, float sizePx, bool bold)
{
    Gdiplus::RectF rf((Gdiplus::REAL)r.left, (Gdiplus::REAL)r.top,
                      (Gdiplus::REAL)(r.right - r.left), (Gdiplus::REAL)(r.bottom - r.top));

    Gdiplus::StringFormat fmt;
    fmt.SetAlignment(Gdiplus::StringAlignmentCenter);
    fmt.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gdiplus::FontFamily ff(L"Segoe UI");
    Gdiplus::Font font(&ff, sizePx, bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

    {
        Gdiplus::SolidBrush shadow(Gdiplus::Color(180, 0, 0, 0));
        Gdiplus::RectF sh = rf;
        sh.X += 2.0f; sh.Y += 2.0f;
        g.DrawString(text, -1, &font, sh, &fmt, &shadow);
    }

    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 235, 235, 235));
    g.DrawString(text, -1, &font, rf, &fmt, &brush);
}



void Button::onHover(HWND hwnd, bool isHovering)
{
    if (disabled) isHovering = false;

    if (hovered != isHovering)
    {
        hovered = isHovering;
        InvalidateRect(hwnd, NULL, FALSE);
    }
}


static void LayoutMainMenuButtons(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    int w = rc.right;
    int h = rc.bottom;

    int btnW = (int)(w * 0.25);
    int btnH = (int)(h * 0.01);


    if (btnW < 220) btnW = 220;
    if (btnH < 48)  btnH = 48;

    int btnX = (w - btnW) / 2;
    int gap  = (int)(h * 0.03);
    if (gap < 12) gap = 12;

    int totalH = 4 * btnH + 3 * gap;
    int startY = (h - totalH) / 2 + (int)(h * 0.08);

    playButton.init(nullptr, btnX, startY + 0 * (btnH + gap), btnW, btnH);
    loadButton.init(nullptr, btnX, startY + 1 * (btnH + gap), btnW, btnH);
    settingsButton.init(nullptr, btnX, startY + 2 * (btnH + gap), btnW, btnH);
    exitButton.init(nullptr, btnX, startY + 3 * (btnH + gap), btnW, btnH);

}

static void DrawButtonLabel(Gdiplus::Graphics& g, const RECT& r, const wchar_t* text, bool hovered)
{
    Gdiplus::RectF rf((Gdiplus::REAL)r.left, (Gdiplus::REAL)r.top, (Gdiplus::REAL)RectW(r), (Gdiplus::REAL)RectH(r));

    Gdiplus::StringFormat fmt;
    fmt.SetAlignment(Gdiplus::StringAlignmentCenter);
    fmt.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gdiplus::FontFamily ff(L"Segoe UI");
    Gdiplus::Font font(&ff, (Gdiplus::REAL)(RectH(r) * 0.42), Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

    Gdiplus::SolidBrush brush(hovered ? Gdiplus::Color(255, 240, 240, 240) : Gdiplus::Color(255, 210, 210, 210));
    g.DrawString(text, -1, &font, rf, &fmt, &brush);
}

void drawMainMenu(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics g(hdc);

    g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
    g.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
    

    RECT rc;
    GetClientRect(hwnd, &rc);

    LayoutMainMenuButtons(hwnd);
    loadButton.disabled = !gameState.hasUnfinishedGame;

    if(gAssets.mainmenuBg)
    {
        g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);
        g.DrawImage(gAssets.mainmenuBg, 0, 0, rc.right, rc.bottom);
    }
    else if(gAssets.woodmenuBg)
    {
        g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);
        g.DrawImage(gAssets.woodmenuBg, 0, 0, rc.right, rc.bottom);
    }
    else
    {
        Gdiplus::SolidBrush bgFill(Gdiplus::Color(255, 30, 30, 30));
        g.FillRectangle(&bgFill, Gdiplus::RectF(0, 0, (Gdiplus::REAL)RectW(rc), (Gdiplus::REAL)RectH(rc)));
    }

    RECT titleRect{
        0,
        (int)(rc.bottom * 0.05f),
        rc.right,
        (int)(rc.bottom * 0.25f)
    };

    float titleSize = (float)(rc.bottom * 0.09f);
    if (titleSize < 36.0f) titleSize = 36.0f;
    if (titleSize > 96.0f) titleSize = 96.0f;

    DrawCenteredText(g, titleRect, L"Simple Chess", titleSize, true);

    playButton.draw(g);
    loadButton.draw(g);
    settingsButton.draw(g);
    exitButton.draw(g);

    float labelSize = (float)((playButton.rect.bottom - playButton.rect.top) * 0.45f);
    if (labelSize < 18.0f) labelSize = 18.0f;

    RECT rPlay = GetVisualRect(playButton);
    RECT rCont = GetVisualRect(loadButton);
    RECT rSet  = GetVisualRect(settingsButton);
    RECT rExit = GetVisualRect(exitButton);

    DrawCenteredText(g, rPlay, L"New Game", labelSize, true);
    DrawCenteredText(g, rCont, L"Continue", labelSize, true);
    DrawCenteredText(g, rSet,  L"Settings", labelSize, true);
    DrawCenteredText(g, rExit, L"Exit", labelSize, true);
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
