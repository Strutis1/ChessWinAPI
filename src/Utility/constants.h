#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Window
{
    constexpr int Height = 1000;
    constexpr int Width = 920;
}

namespace Menu
{
    constexpr int FileNew = 1;
    constexpr int FileOpen = 2;
    constexpr int FileQuit = 3;
    constexpr int EditRedo = 4;
}

namespace Timer
{
    constexpr int GameLoop = 1;
}

constexpr int FPS = 30;

namespace Resources
{
    namespace Menu
    {
        constexpr int Background = 101;
        constexpr int Play = 102;
        constexpr int Load = 103;
        constexpr int Settings = 104;
        constexpr int Exit = 105;
    }

    namespace WhitePieces
    {
        constexpr int Pawn = 201;
        constexpr int Queen = 202;
        constexpr int King = 203;
        constexpr int Rook = 204;
        constexpr int Knight = 205;
        constexpr int Bishop = 206;
    }

    namespace BlackPieces
    {
        constexpr int Pawn = 301;
        constexpr int Queen = 302;
        constexpr int King = 303;
        constexpr int Rook = 304;
        constexpr int Knight = 305;
        constexpr int Bishop = 306;
    }

    namespace Boards
    {
        constexpr int Plain1 = 401;
        constexpr int Plain2 = 402;
        constexpr int Plain3 = 403;
        constexpr int Plain4 = 404;
        constexpr int Plain5 = 405;
    }

    constexpr int IconMain = 501;
}

#endif
