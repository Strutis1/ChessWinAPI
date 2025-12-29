#include "screens.h"
#include "../Utility/constants.h"
#include "../Utility/gameAssets.h"
#include "../Utility/utilz.h"
#include "appState.h"
#include <string>
#include "game.h"

Button playButton;
Button loadButton;
Button settingsButton;
Button exitButton;
Button retryButton;
Button goExitButton;
Button sillyBotButton;
Button backButton;
Button whiteButton;
Button blackButton;
Button colorBackButton;

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
 
static std::wstring ToWString(const std::string& s)
{
    return std::wstring(s.begin(), s.end());
}

static bool PlayerIsWhite()
{
    return appState.playerColor != PieceColor::BLACK;
}

int displayToBoardCoord(int displayCoord)
{
    int maxIdx = BoardUI::BoardCols - 1;
    return PlayerIsWhite() ? displayCoord : (maxIdx - displayCoord);
}

int boardToDisplayCoord(int boardCoord)
{
    int maxIdx = BoardUI::BoardCols - 1;
    return PlayerIsWhite() ? boardCoord : (maxIdx - boardCoord);
}

static void DrawRotatedText(Gdiplus::Graphics& g, const wchar_t* text, const Gdiplus::Font& font,
                            const Gdiplus::RectF& rect, const Gdiplus::StringFormat& fmt,
                            const Gdiplus::Brush& brush, float angle)
{
    Gdiplus::GraphicsState state = g.Save();
    Gdiplus::REAL centerX = rect.X + rect.Width * 0.5f;
    Gdiplus::REAL centerY = rect.Y + rect.Height * 0.5f;
    g.TranslateTransform(centerX, centerY);
    g.RotateTransform(angle);

    Gdiplus::RectF rotatedRect(
        -rect.Width * 0.5f,
        -rect.Height * 0.5f,
        rect.Width,
        rect.Height);

    g.DrawString(text, -1, &font, rotatedRect, &fmt, &brush);
    g.Restore(state);
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

static void LayoutDifficultyButtons(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    int w = rc.right;
    int h = rc.bottom;

    int btnW = (int)(w * 0.28);
    int btnH = (int)(h * 0.08);
    if (btnW < 220) btnW = 220;
    if (btnH < 52)  btnH = 52;

    int btnX = (w - btnW) / 2;
    int gap  = (int)(h * 0.02);
    if (gap < 12) gap = 12;

    int startY = (int)(h * 0.45f);

    sillyBotButton.init(nullptr, btnX, startY, btnW, btnH);
    backButton.init(nullptr, btnX, startY + btnH + gap, btnW, btnH);
}

static void LayoutColorButtons(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    int w = rc.right;
    int h = rc.bottom;

    int btnW = (int)(w * 0.32);
    int btnH = (int)(h * 0.08);
    if (btnW < 220) btnW = 220;
    if (btnH < 52)  btnH = 52;

    int btnX = (w - btnW) / 2;
    int gap  = (int)(h * 0.02);
    if (gap < 12) gap = 12;

    int startY = (int)(h * 0.42f);

    whiteButton.init(nullptr, btnX, startY, btnW, btnH);
    blackButton.init(nullptr, btnX, startY + btnH + gap, btnW, btnH);
    colorBackButton.init(nullptr, btnX, startY + 2 * (btnH + gap), btnW, btnH);
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
    loadButton.disabled = !appState.hasUnfinishedGame;

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

    std::wstring contLabel = appState.hasUnfinishedGame
        ? std::wstring(L"Continue (") + ToWString(appState.currentDifficulty) + L")"
        : std::wstring(L"Continue");

    DrawCenteredText(g, rPlay, L"New Game", labelSize, true);
    DrawCenteredText(g, rCont, contLabel.c_str(), labelSize, true);
    DrawCenteredText(g, rSet,  L"Settings", labelSize, true);
    DrawCenteredText(g, rExit, L"Exit", labelSize, true);
}

void drawDifficultyScreen(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics g(hdc);

    g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
    g.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

    RECT rc;
    GetClientRect(hwnd, &rc);

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
        (int)(rc.bottom * 0.12f),
        rc.right,
        (int)(rc.bottom * 0.28f)
    };

    float titleSize = (float)(rc.bottom * 0.07f);
    if (titleSize < 32.0f) titleSize = 32.0f;
    if (titleSize > 72.0f) titleSize = 72.0f;

    DrawCenteredText(g, titleRect, L"Select Difficulty", titleSize, true);

    LayoutDifficultyButtons(hwnd);

    sillyBotButton.draw(g);
    backButton.draw(g);

    float labelSize = (float)((sillyBotButton.rect.bottom - sillyBotButton.rect.top) * 0.45f);
    if (labelSize < 18.0f) labelSize = 18.0f;

    RECT rSilly  = GetVisualRect(sillyBotButton);
    RECT rBack   = GetVisualRect(backButton);

    DrawCenteredText(g, rSilly, L"SillyBot", labelSize, true);
    DrawCenteredText(g, rBack,  L"Back", labelSize, true);
}

void drawColorSelectScreen(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics g(hdc);

    g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);

    RECT rc;
    GetClientRect(hwnd, &rc);

    if (gAssets.mainmenuBg)
    {
        g.DrawImage(gAssets.mainmenuBg, 0, 0, rc.right, rc.bottom);
    }
    else if (gAssets.woodmenuBg)
    {
        g.DrawImage(gAssets.woodmenuBg, 0, 0, rc.right, rc.bottom);
    }
    else
    {
        Gdiplus::SolidBrush bgFill(Gdiplus::Color(255, 28, 28, 28));
        g.FillRectangle(&bgFill, Gdiplus::RectF(0, 0, (Gdiplus::REAL)RectW(rc), (Gdiplus::REAL)RectH(rc)));
    }

    RECT titleRect{
        0,
        (int)(rc.bottom * 0.12f),
        rc.right,
        (int)(rc.bottom * 0.28f)
    };

    float titleSize = (float)(rc.bottom * 0.07f);
    if (titleSize < 32.0f) titleSize = 32.0f;
    if (titleSize > 72.0f) titleSize = 72.0f;

    DrawCenteredText(g, titleRect, L"Choose Your Color", titleSize, true);

    LayoutColorButtons(hwnd);

    whiteButton.draw(g);
    blackButton.draw(g);
    colorBackButton.draw(g);

    float labelSize = (float)((whiteButton.rect.bottom - whiteButton.rect.top) * 0.45f);
    if (labelSize < 18.0f) labelSize = 18.0f;

    RECT rWhite = GetVisualRect(whiteButton);
    RECT rBlack = GetVisualRect(blackButton);
    RECT rBack  = GetVisualRect(colorBackButton);

    DrawCenteredText(g, rWhite, L"Play as White", labelSize, true);
    DrawCenteredText(g, rBlack, L"Play as Black", labelSize, true);
    DrawCenteredText(g, rBack,  L"Back", labelSize, true);
}

static void LayoutGameOverButtons(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    int w = rc.right;
    int h = rc.bottom;

    int btnW = (int)(w * 0.30);
    int btnH = (int)(h * 0.08);

    if (btnW < 220) btnW = 220;
    if (btnH < 52)  btnH = 52;

    int btnX = (w - btnW) / 2;
    int gap  = (int)(h * 0.02);
    if (gap < 12) gap = 12;

    int startY = (int)(h * 0.55f);

    retryButton.init(nullptr, btnX, startY, btnW, btnH);
    goExitButton.init(nullptr, btnX, startY + btnH + gap, btnW, btnH);
}


void drawGameScreen(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics g(hdc);

    g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
    g.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
    g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

    RECT rc;
    GetClientRect(hwnd, &rc);

    if (gAssets.woodmenuBg)
    {
        g.DrawImage(gAssets.woodmenuBg, 0, 0, rc.right, rc.bottom);
    }

    drawBoard(hwnd, g, currentBoardLayout);
}



void drawGameOver(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics g(hdc);
    g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);
    g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
    g.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

    RECT rc;
    GetClientRect(hwnd, &rc);

    if (gAssets.mainmenuBg)
    {
        g.DrawImage(gAssets.mainmenuBg, 0, 0, rc.right, rc.bottom);
    }
    else if (gAssets.woodmenuBg)
    {
        g.DrawImage(gAssets.woodmenuBg, 0, 0, rc.right, rc.bottom);
    }
    else
    {
        Gdiplus::SolidBrush bgFill(Gdiplus::Color(255, 25, 25, 25));
        g.FillRectangle(&bgFill, Gdiplus::RectF(0, 0, (Gdiplus::REAL)RectW(rc), (Gdiplus::REAL)RectH(rc)));
    }

    // Dim overlay to separate from board
    {
        Gdiplus::SolidBrush overlay(Gdiplus::Color(180, 0, 0, 0));
        g.FillRectangle(&overlay, Gdiplus::RectF(0, 0, (Gdiplus::REAL)RectW(rc), (Gdiplus::REAL)RectH(rc)));
    }

    LayoutGameOverButtons(hwnd);

    RECT titleRect{
        0,
        (int)(rc.bottom * 0.12f),
        rc.right,
        (int)(rc.bottom * 0.32f)
    };

    float titleSize = (float)(rc.bottom * 0.10f);
    if (titleSize < 40.0f) titleSize = 40.0f;
    if (titleSize > 96.0f) titleSize = 96.0f;
    DrawCenteredText(g, titleRect, L"Game Over", titleSize, true);

    std::wstring resultText;
    PieceColor winner = chessGame.getWinner();
    if (winner == PieceColor::WHITE)
        resultText = L"White wins by checkmate";
    else if (winner == PieceColor::BLACK)
        resultText = L"Black wins by checkmate";
    else
        resultText = L"Draw (stalemate)";

    RECT infoRect{
        0,
        (int)(rc.bottom * 0.34f),
        rc.right,
        (int)(rc.bottom * 0.46f)
    };

    float infoSize = (float)(rc.bottom * 0.05f);
    if (infoSize < 22.0f) infoSize = 22.0f;
    if (infoSize > 48.0f) infoSize = 48.0f;
    DrawCenteredText(g, infoRect, resultText.c_str(), infoSize, false);

    retryButton.draw(g);
    goExitButton.draw(g);

    float labelSize = (float)((retryButton.rect.bottom - retryButton.rect.top) * 0.45f);
    if (labelSize < 18.0f) labelSize = 18.0f;

    RECT rRetry = GetVisualRect(retryButton);
    RECT rExit  = GetVisualRect(goExitButton);

    DrawCenteredText(g, rRetry, L"Retry", labelSize, true);
    DrawCenteredText(g, rExit,  L"Exit", labelSize, true);
}


void updateBoardLayout(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    const int margin = 20;

    int w = (rc.right - rc.left) - 2 * margin;
    int h = (rc.bottom - rc.top) - 2 * margin;

    int maxSquare = min(w / BoardUI::BoardCols, h / BoardUI::BoardRows);

    int squareSize = (maxSquare / 32) * 32;

    if (squareSize < 32) squareSize = 32;

    int boardW = squareSize * BoardUI::BoardCols;
    int boardH = squareSize * BoardUI::BoardRows;

    int originX = margin + (w - boardW) / 2;
    int originY = margin + (h - boardH) / 2;

    currentBoardLayout = BoardLayout(originX, originY, squareSize);
}


void drawBoard(HWND hwnd, Gdiplus::Graphics& g, const BoardLayout& layout)
{
    float coordFontSize = (float)(layout.squareSize * 0.22f);
    if (coordFontSize < 10.0f) coordFontSize = 10.0f;

    Gdiplus::FontFamily coordFontFamily(L"Segoe UI");
    Gdiplus::Font coordFont(&coordFontFamily, coordFontSize, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

    Gdiplus::StringFormat fileFormat;
    fileFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    fileFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);

    Gdiplus::StringFormat rankFormat;
    rankFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    rankFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    for (int displayRow = 0; displayRow < BoardUI::BoardRows; ++displayRow)
    {
        for (int displayCol = 0; displayCol < BoardUI::BoardCols; ++displayCol)
        {
            int boardCol = displayToBoardCoord(displayCol);
            int boardRow = displayToBoardCoord(displayRow);
            bool isLightSquare = ((boardRow + boardCol) % 2 == 0);
            Gdiplus::Color squareColor = isLightSquare ? Gdiplus::Color(255, 210, 210, 210)
                                                      : Gdiplus::Color(255, 95, 95, 95);
                                                
            Gdiplus::SolidBrush brush(squareColor);

            int x = layout.originX + displayCol * layout.squareSize;
            int y = layout.originY + displayRow * layout.squareSize;
            

            Gdiplus::Rect squareRect((Gdiplus::REAL)x, (Gdiplus::REAL)y,
                                      (Gdiplus::REAL)layout.squareSize, (Gdiplus::REAL)layout.squareSize);

            g.FillRectangle(&brush, squareRect);

            //handle selected piece highlighting
            if(chessGame.getSelectedPosX() == boardCol && chessGame.getSelectedPosY() == boardRow)
            {
                Gdiplus::Pen pen(Gdiplus::Color(255, 250, 170, 0), 4.0f);
                g.DrawRectangle(&pen, squareRect);
            }


            Piece piece = chessGame.getBoard().getPieceAt(boardCol, boardRow);
            if (piece.type != PieceType::NONE)
            {
                Gdiplus::Image* pieceImage = nullptr;
                switch (piece.type)
                {
                case PieceType::PAWN:
                    pieceImage = (piece.color == PieceColor::WHITE) ? gAssets.whitePawn : gAssets.blackPawn;
                    break;
                case PieceType::ROOK:
                    pieceImage = (piece.color == PieceColor::WHITE) ? gAssets.whiteRook : gAssets.blackRook;
                    break;
                case PieceType::KNIGHT:
                    pieceImage = (piece.color == PieceColor::WHITE) ? gAssets.whiteKnight : gAssets.blackKnight;
                    break;
                case PieceType::BISHOP:
                    pieceImage = (piece.color == PieceColor::WHITE) ? gAssets.whiteBishop : gAssets.blackBishop;
                    break;
                case PieceType::QUEEN:
                    pieceImage = (piece.color == PieceColor::WHITE) ? gAssets.whiteQueen : gAssets.blackQueen;
                    break;
                case PieceType::KING:
                    pieceImage = (piece.color == PieceColor::WHITE) ? gAssets.whiteKing : gAssets.blackKing;
                    break;
                default:
                    break;
                }

                if (pieceImage)
                {
                    g.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
                    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
                    g.DrawImage(pieceImage, x, y, layout.squareSize, layout.squareSize);
                }
            }
        }
    }

    // draw file letters (a-h) above and below the board
    Gdiplus::SolidBrush labelBrush(Gdiplus::Color(255, 230, 230, 230));
    Gdiplus::REAL topY    = (Gdiplus::REAL)(layout.originY - coordFontSize - 6.0f);
    Gdiplus::REAL bottomY = (Gdiplus::REAL)(layout.originY + layout.height() + 2.0f);

    for (int displayCol = 0; displayCol < BoardUI::BoardCols; ++displayCol)
    {
        int boardCol = displayToBoardCoord(displayCol);
        wchar_t fileText[2] = { (wchar_t)(L'a' + boardCol), L'\0' };

        Gdiplus::RectF topRect(
            (Gdiplus::REAL)(layout.originX + displayCol * layout.squareSize),
            topY,
            (Gdiplus::REAL)layout.squareSize,
            (Gdiplus::REAL)(coordFontSize + 8.0f));

        Gdiplus::RectF bottomRect(
            (Gdiplus::REAL)(layout.originX + displayCol * layout.squareSize),
            bottomY,
            (Gdiplus::REAL)layout.squareSize,
            (Gdiplus::REAL)(coordFontSize + 8.0f));

        g.DrawString(fileText, -1, &coordFont, bottomRect, &fileFormat, &labelBrush);
        DrawRotatedText(g, fileText, coordFont, topRect, fileFormat, labelBrush, 180.0f);
    }

    // draw rank numbers (1-8) on the sides, from White's perspective
    Gdiplus::REAL leftX  = (Gdiplus::REAL)(layout.originX - coordFontSize - 8.0f);
    Gdiplus::REAL rightX = (Gdiplus::REAL)(layout.originX + layout.width() + 4.0f);

    for (int displayRow = 0; displayRow < BoardUI::BoardRows; ++displayRow)
    {
        int boardRow = displayToBoardCoord(displayRow);
        int rank = BoardUI::BoardRows - boardRow;
        wchar_t rankText[2] = { (wchar_t)(L'0' + rank), L'\0' };

        Gdiplus::RectF leftRect(
            leftX,
            (Gdiplus::REAL)(layout.originY + displayRow * layout.squareSize),
            (Gdiplus::REAL)(coordFontSize + 10.0f),
            (Gdiplus::REAL)layout.squareSize);

        Gdiplus::RectF rightRect(
            rightX,
            (Gdiplus::REAL)(layout.originY + displayRow * layout.squareSize),
            (Gdiplus::REAL)(coordFontSize + 10.0f),
            (Gdiplus::REAL)layout.squareSize);

        g.DrawString(rankText, -1, &coordFont, leftRect, &rankFormat, &labelBrush);
        DrawRotatedText(g, rankText, coordFont, rightRect, rankFormat, labelBrush, 180.0f);
    }
}

void drawTimer(HWND hwnd, HDC hdc)
{
    Gdiplus::Graphics g(hdc);

    g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBilinear);

    RECT rc;
    GetClientRect(hwnd, &rc);
    RECT timerRect{
        rc.right - 150,
        10,
        rc.right - 10,
        10 + 40
    };
    float timerSize = 28.0f;
    
    
}

bool pointInBoard(const BoardLayout& layout, int x, int y)
{
    return (x >= layout.originX && x < layout.originX + layout.width() &&
            y >= layout.originY && y < layout.originY + layout.height());
}
