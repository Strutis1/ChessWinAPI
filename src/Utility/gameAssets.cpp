#include "gameAssets.h"
#include "utilz.h"
#include "resource.h"

GameAssets gAssets;

void GameAssets::load(HINSTANCE hInst)
{
    // Menu
    mainmenuBg = LoadPngFromResource(hInst, IDB_MAINMENU_BG);
    woodmenuBg = LoadPngFromResource(hInst, IDB_WOODMENU_BG);
    // menuPlay = LoadPngFromResource(hInst, IDB_MENU_PLAY);
    // menuLoad = LoadPngFromResource(hInst, IDB_MENU_LOAD);
    // menuSettings = LoadPngFromResource(hInst, IDB_MENU_SETTINGS);
    // menuExit = LoadPngFromResource(hInst, IDB_MENU_EXIT);

    // White pieces
    whitePawn = LoadPngFromResource(hInst, IDB_PIECES_WPAWN);
    whiteQueen = LoadPngFromResource(hInst, IDB_PIECES_WQUEEN);
    whiteKing = LoadPngFromResource(hInst, IDB_PIECES_WKING);
    whiteRook = LoadPngFromResource(hInst, IDB_PIECES_WROOK);
    whiteKnight = LoadPngFromResource(hInst, IDB_PIECES_WKNIGHT);
    whiteBishop = LoadPngFromResource(hInst, IDB_PIECES_WBISHOP);

    // Black pieces
    blackPawn = LoadPngFromResource(hInst, IDB_PIECES_BPAWN);
    blackQueen = LoadPngFromResource(hInst, IDB_PIECES_BQUEEN);
    blackKing = LoadPngFromResource(hInst, IDB_PIECES_BKING);
    blackRook = LoadPngFromResource(hInst, IDB_PIECES_BROOK);
    blackKnight = LoadPngFromResource(hInst, IDB_PIECES_BKNIGHT);
    blackBishop = LoadPngFromResource(hInst, IDB_PIECES_BBISHOP);

    // Boards
    // boardPlain1 = LoadPngFromResource(hInst, IDB_BOARDS_PLAIN1);
    // boardPlain2 = LoadPngFromResource(hInst, IDB_BOARDS_PLAIN2);
    // boardPlain3 = LoadPngFromResource(hInst, IDB_BOARDS_PLAIN3);
    // boardPlain4 = LoadPngFromResource(hInst, IDB_BOARDS_PLAIN4);
    // boardPlain5 = LoadPngFromResource(hInst, IDB_BOARDS_PLAIN5);
}

void GameAssets::unload()
{
    delete mainmenuBg;
    delete woodmenuBg;
    // delete menuPlay;
    // delete menuLoad;
    // delete menuSettings;
    // delete menuExit;

    delete whitePawn;
    delete whiteQueen;
    delete whiteKing;
    delete whiteRook;
    delete whiteKnight;
    delete whiteBishop;
    delete blackPawn;
    delete blackQueen;
    delete blackKing;
    delete blackRook;
    delete blackKnight;
    delete blackBishop;

    // delete boardPlain1;
    // delete boardPlain2;
    // delete boardPlain3;
    // delete boardPlain4;
    // delete boardPlain5;
}
