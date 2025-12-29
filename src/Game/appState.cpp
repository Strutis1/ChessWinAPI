#include "appState.h"

void AppState::init()
{
    running = true;
    currentScreen = GameScreen::MainMenu;
    currentDifficulty = "sillyBot";
    playerColor = PieceColor::WHITE;
}

void AppState::changeScreen(GameScreen gs)
{
    currentScreen = gs;
}
