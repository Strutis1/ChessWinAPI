#ifndef APPSTATE_H
#define APPSTATE_H

#include <windows.h>
#include <string>
#include "../Classes/piece.h"

enum class GameScreen
{
    MainMenu,
    DifficultySelect,
    ColorSelect,
    Playing,
    GameOver
};

struct AppState
{
    GameScreen currentScreen;
    bool running;
    bool hasUnfinishedGame = false;
    std::string currentDifficulty = "sillyBot";
    PieceColor playerColor = PieceColor::WHITE;

    void init();
    void changeScreen(GameScreen newScreen);
};

extern AppState appState;

#endif
