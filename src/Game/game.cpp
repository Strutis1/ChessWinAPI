#include "game.h"
#include "screens.h"
#include "appState.h"



void ChessGame::init()
{
    currentTurn = PieceColor::WHITE;
    gameOver = false;
    theBoard.reset();
    setTheBoardUp();
    selectedX = -1;
    selectedY = -1;
    hasUnsavedChanges = false;

    winner = PieceColor::NONE;
}


bool ChessGame::isLegalMove(const Move& move)
{
    Piece from = theBoard.getPieceAt(move.fromX, move.fromY);
    if (from.type == PieceType::NONE) return false;
    if (from.color != currentTurn) return false;
    if (gameOver) return false;

    if (!isPseudoLegalMove(theBoard, move))
        return false;

    Board temp = theBoard;
    temp.movePiece(move.fromX, move.fromY, move.toX, move.toY);

    if (isInCheck(temp, currentTurn))
        return false;

    return true;
}


void ChessGame::makeMove(const Move& move)
{
    if (isLegalMove(move))
    {
        theBoard.movePiece(move.fromX, move.fromY, move.toX, move.toY);
        switchTurn();
        checkGameOver();
        hasUnsavedChanges = true;
    }
}
void ChessGame::checkGameOver()
{
    bool mate = isMate();
    bool draw = false;

    if (mate)
    {
        winner = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
        endGame();
    }
    else
    {
        draw = stalemate();
        if (draw)
        {
            winner = PieceColor::NONE;
            endGame();
        }
    }

    if (gameOver)
    {
        appState.hasUnfinishedGame = false;
        appState.currentScreen = GameScreen::GameOver;
    }
}


void ChessGame::checkForSavedGame()
{
    std::ifstream in(kSaveFile);
    if (!in) { appState.hasUnfinishedGame = false; return; }

    std::string content((std::istreambuf_iterator<char>(in)), {});
    bool ok = content.find("\"formatVersion\"") != std::string::npos &&
              content.find("\"turn\"") != std::string::npos &&
              content.find("\"board\"") != std::string::npos;
    appState.hasUnfinishedGame = ok;
}

 
bool ChessGame::saveGame()
{
    std::ofstream out(kSaveFile, std::ios::trunc);
    if (!out) return false;

    out << "{\n";
    out << "  \"formatVersion\": 1,\n";
    out << "  \"turn\": \"" << (currentTurn == PieceColor::WHITE ? "white" : "black") << "\",\n";
    out << "  \"difficulty\": \"" << appState.currentDifficulty << "\",\n";
    out << "  \"board\": [\n";
    for (int y = 0; y < 8; ++y) {
        out << "    [";
        for (int x = 0; x < 8; ++x) {
            out << "\"" << pieceToCode(theBoard.getPieceAt(x, y)) << "\"";
            if (x < 7) out << ",";
        }
        out << "]";
        if (y < 7) out << ",";
        out << "\n";
    }
    out << "  ]\n";
    out << "}\n";

    appState.hasUnfinishedGame = true;
    hasUnsavedChanges = false;
    return true;
}


bool ChessGame::loadGame()
{
    std::ifstream in(kSaveFile);
    if (!in) return false;

    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    auto getString = [&](const std::string& key, std::string& out) -> bool {
        size_t kpos = content.find("\"" + key + "\"");
        if (kpos == std::string::npos) return false;
        size_t first = content.find('"', content.find(':', kpos));
        if (first == std::string::npos) return false;
        size_t second = content.find('"', first + 1);
        if (second == std::string::npos) return false;
        out = content.substr(first + 1, second - first - 1);
        return true;
    };

    std::string turnStr;
    if (!getString("turn", turnStr)) return false;
    PieceColor loadedTurn = (turnStr == "white") ? PieceColor::WHITE :
                            (turnStr == "black") ? PieceColor::BLACK : PieceColor::NONE;
    if (loadedTurn == PieceColor::NONE) return false;

    std::string difficultyStr = "sillyBot";
    getString("difficulty", difficultyStr); 

    size_t boardPos = content.find("\"board\"");
    if (boardPos == std::string::npos) return false;
    size_t idx = content.find('[', boardPos);
    if (idx == std::string::npos) return false;

    std::vector<std::string> codes;
    while (idx < content.size() && codes.size() < 64) {
        size_t q1 = content.find('"', idx);
        if (q1 == std::string::npos) break;
        size_t q2 = content.find('"', q1 + 1);
        if (q2 == std::string::npos) break;
        std::string token = content.substr(q1 + 1, q2 - q1 - 1);
        if (token.size() == 2) codes.push_back(token);
        idx = q2 + 1;
    }
    if (codes.size() != 64) return false;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            theBoard.setPieceAt(x, y, codeToPiece(codes[y * 8 + x]));
        }
    }

    currentTurn = loadedTurn;
    gameOver = false;
    winner = PieceColor::NONE;
    selectedX = -1;
    selectedY = -1;
    appState.hasUnfinishedGame = true;
    appState.currentDifficulty = difficultyStr;
    hasUnsavedChanges = false;
    return true;
}



bool ChessGame::findKing(const Board& board, PieceColor kingColor, int& outX, int& outY) const
{
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece p = board.getPieceAt(x, y);
            if (p.type == PieceType::KING && p.color == kingColor)
            {
                outX = x; outY = y;
                return true;
            }
        }
    }
    return false;
}

bool ChessGame::isCheck()
{
    return isInCheck(theBoard, currentTurn);
}


bool ChessGame::isMate()
{
    if (!isInCheck(theBoard, currentTurn))
        return false;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece p = theBoard.getPieceAt(x, y);
            if (p.color != currentTurn) continue;

            for (int ty = 0; ty < 8; ++ty)
            {
                for (int tx = 0; tx < 8; ++tx)
                {
                    Move m(x, y, tx, ty);
                    if (isLegalMove(m))
                        return false;
                }
            }
        }
    }
    return true;
}


bool ChessGame::stalemate()
{
    if (isInCheck(theBoard, currentTurn))
        return false;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece p = theBoard.getPieceAt(x, y);
            if (p.color != currentTurn) continue;

            for (int ty = 0; ty < 8; ++ty)
            {
                for (int tx = 0; tx < 8; ++tx)
                {
                    Move m(x, y, tx, ty);
                    if (isLegalMove(m))
                        return false;
                }
            }
        }
    }
    return true;
}




bool ChessGame::getObstructionFreeMove(const Board& board, const Move& move) const
{
    Piece movingPiece = board.getPieceAt(move.fromX, move.fromY);
    if (movingPiece.type == PieceType::KNIGHT)
    {
        return true; 
    }

    int deltaX = (move.toX - move.fromX);
    int deltaY = (move.toY - move.fromY);
    int stepX = (deltaX == 0) ? 0 : (deltaX / abs(deltaX));
    int stepY = (deltaY == 0) ? 0 : (deltaY / abs(deltaY));

    int currentX = move.fromX + stepX;
    int currentY = move.fromY + stepY;

    while (currentX != move.toX || currentY != move.toY)
    {
        if (board.getPieceAt(currentX, currentY).type != PieceType::NONE)
        {
            return false; 
        }
        currentX += stepX;
        currentY += stepY;
    }

    return true; 
}

bool ChessGame::isValidPieceMovement(const Board& board, const Move& move) const
{
    Piece movingPiece = board.getPieceAt(move.fromX, move.fromY);
    int deltaX = move.toX - move.fromX;
    int deltaY = move.toY - move.fromY;

    switch (movingPiece.type)
    {
    case PieceType::PAWN:
        if (movingPiece.color == PieceColor::WHITE)
        {
            if (deltaX == 0 && deltaY == -1 && board.getPieceAt(move.toX, move.toY).type == PieceType::NONE)
                return true; 
            if (deltaX == 0 && deltaY == -2 && move.fromY == 6 &&
                board.getPieceAt(move.toX, move.toY).type == PieceType::NONE &&
                board.getPieceAt(move.fromX, move.fromY - 1).type == PieceType::NONE)
                return true;
 
            if (abs(deltaX) == 1 && deltaY == -1 && board.getPieceAt(move.toX, move.toY).type != PieceType::NONE &&
                board.getPieceAt(move.toX, move.toY).color != movingPiece.color)
                return true; 
        }
        else
        {
            if (deltaX == 0 && deltaY == 1 && board.getPieceAt(move.toX, move.toY).type == PieceType::NONE)
                return true; 
            if (deltaX == 0 && deltaY == 2 && move.fromY == 1 &&
                board.getPieceAt(move.toX, move.toY).type == PieceType::NONE &&
                board.getPieceAt(move.fromX, move.fromY + 1).type == PieceType::NONE)
                return true;
 
            if (abs(deltaX) == 1 && deltaY == 1 && board.getPieceAt(move.toX, move.toY).type != PieceType::NONE &&
                board.getPieceAt(move.toX, move.toY).color != movingPiece.color)
                return true; 
        }
        return false;

    case PieceType::ROOK:
        return (deltaX == 0 || deltaY == 0);

    case PieceType::BISHOP:
        return (abs(deltaX) == abs(deltaY));

    case PieceType::QUEEN:
        return (deltaX == 0 || deltaY == 0 || abs(deltaX) == abs(deltaY));

    case PieceType::KING:
        return (abs(deltaX) <= 1 && abs(deltaY) <= 1);

    case PieceType::KNIGHT:
        return ( (abs(deltaX) == 2 && abs(deltaY) == 1) || (abs(deltaX) == 1 && abs(deltaY) == 2) );
    default:
        return false;
    }
}


bool ChessGame::isPseudoLegalMove(const Board& board, const Move& move) const
{
    Piece from = board.getPieceAt(move.fromX, move.fromY);
    if (from.type == PieceType::NONE) return false;

    Piece to = board.getPieceAt(move.toX, move.toY);

    if (to.color == from.color && to.color != PieceColor::NONE)
        return false;

    if (!getObstructionFreeMove(board, move))
        return false;

    if (!isValidPieceMovement(board, move))
        return false;

    return true;
}
bool ChessGame::isInCheck(const Board& board, PieceColor kingColor) const
{
    int kingX, kingY;
    if (!findKing(board, kingColor, kingX, kingY))
        return false;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece p = board.getPieceAt(x, y);
            if (p.color != kingColor && p.color != PieceColor::NONE)
            {
                Move potentialMove(x, y, kingX, kingY);
                if (isPseudoLegalMove(board, potentialMove))
                {
                    return true; 
                }
            }
        }
    }
    return false; 
}


void ChessGame::switchTurn()
{
    if (currentTurn == PieceColor::WHITE)
        currentTurn = PieceColor::BLACK;
    else
        currentTurn = PieceColor::WHITE;
}


void ChessGame::endGame()
{
    gameOver = true;
}


Piece ChessGame::detectSelection(int x, int y)
{
    return theBoard.getPieceAt(x, y);
}



void ChessGame::setTheBoardUp()
{
    theBoard.setPieceAt(0, 0, Piece(PieceType::ROOK, PieceColor::BLACK));
    theBoard.setPieceAt(1, 0, Piece(PieceType::KNIGHT, PieceColor::BLACK));
    theBoard.setPieceAt(2, 0, Piece(PieceType::BISHOP, PieceColor::BLACK));
    theBoard.setPieceAt(3, 0, Piece(PieceType::QUEEN, PieceColor::BLACK));
    theBoard.setPieceAt(4, 0, Piece(PieceType::KING, PieceColor::BLACK));
    theBoard.setPieceAt(5, 0, Piece(PieceType::BISHOP, PieceColor::BLACK));
    theBoard.setPieceAt(6, 0, Piece(PieceType::KNIGHT, PieceColor::BLACK));
    theBoard.setPieceAt(7, 0, Piece(PieceType::ROOK, PieceColor::BLACK));
    for (int col = 0; col < 8; ++col)
    {
        theBoard.setPieceAt(col, 1, Piece(PieceType::PAWN, PieceColor::BLACK));
        theBoard.setPieceAt(col, 6, Piece(PieceType::PAWN, PieceColor::WHITE));
    }
    theBoard.setPieceAt(0, 7, Piece(PieceType::ROOK, PieceColor::WHITE));
    theBoard.setPieceAt(1, 7, Piece(PieceType::KNIGHT, PieceColor::WHITE));
    theBoard.setPieceAt(2, 7, Piece(PieceType::BISHOP, PieceColor::WHITE));
    theBoard.setPieceAt(3, 7, Piece(PieceType::QUEEN, PieceColor::WHITE));
    theBoard.setPieceAt(4, 7, Piece(PieceType::KING, PieceColor::WHITE));
    theBoard.setPieceAt(5, 7, Piece(PieceType::BISHOP, PieceColor::WHITE));
    theBoard.setPieceAt(6, 7, Piece(PieceType::KNIGHT, PieceColor::WHITE));
    theBoard.setPieceAt(7, 7, Piece(PieceType::ROOK, PieceColor::WHITE));

}
