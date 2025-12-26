#ifndef GAME_H
#define GAME_H

#include <windows.h>

enum class GameScreen
{
    MainMenu,
    Playing,
    GameOver
};

struct GameState
{
    GameScreen currentScreen;
    bool running;
    bool hasUnfinishedGame = false;

    void init();
    void changeScreen(GameScreen newScreen);
};

extern GameState gameState;

#endif