#ifndef APPSTATE_H
#define APPSTATE_H

#include <windows.h>

enum class GameScreen
{
    MainMenu,
    Playing,
    GameOver
};

struct AppState
{
    GameScreen currentScreen;
    bool running;
    bool hasUnfinishedGame = false;

    void init();
    void changeScreen(GameScreen newScreen);
};

extern AppState appState;

#endif