#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../Game/game.h"

static const char* kSaveFile = "savegame.json";

std::string pieceToCode(const Piece& p);

Piece codeToPiece(const std::string& code);




#endif