#ifndef APPSTATE_H
#define APPSTATE_H

#include <windows.h>
#include <string>

enum class GameScreen
{
    MainMenu,
    DifficultySelect,
    Playing,
    GameOver
};

struct AppState
{
    GameScreen currentScreen;
    bool running;
    bool hasUnfinishedGame = false;
    std::string currentDifficulty = "sillyBot";

    void init();
    void changeScreen(GameScreen newScreen);
};

extern AppState appState;

#endif
