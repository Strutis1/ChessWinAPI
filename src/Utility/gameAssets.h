#ifndef GAMEASSETS_H
#define GAMEASSETS_H

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

struct GameAssets
{
    // Menu
    Image *menuBg;
    Image *menuPlay;
    Image *menuLoad;
    Image *menuSettings;
    Image *menuExit;

    // Pieces
    Image *whitePawn;
    Image *whiteQueen;
    Image *whiteKing;
    Image *whiteRook;
    Image *whiteKnight;
    Image *whiteBishop;
    Image *blackPawn;
    Image *blackQueen;
    Image *blackKing;
    Image *blackRook;
    Image *blackKnight;
    Image *blackBishop;

    // Boards
    Image *boardPlain1;
    Image *boardPlain2;
    Image *boardPlain3;
    Image *boardPlain4;
    Image *boardPlain5;

    void load(HINSTANCE hInst);
    void unload();
};

extern GameAssets gAssets;

#endif
