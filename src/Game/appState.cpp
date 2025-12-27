#include "appState.h"

void AppState::init()
{
    running = true;
    currentScreen = GameScreen::MainMenu;
}

void AppState::changeScreen(GameScreen gs)
{
    currentScreen = gs;
}