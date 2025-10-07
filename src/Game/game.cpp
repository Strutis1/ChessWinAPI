#include "game.h"

void GameState::init()
{
    running = true;
    currentScreen = GameScreen::MainMenu;
}

void GameState::changeScreen(GameScreen gs)
{
    currentScreen = gs;
}