#include "game.h"
#include "../Classes/piece.h"
#include "screens.h"
#include "appState.h"

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


void ChessGame::init()
{
    currentTurn = PieceColor::WHITE;
    gameOver = false;
    theBoard.reset();
    setTheBoardUp();
    selectedX = -1;
    selectedY = -1;

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
        appState.currentScreen = GameScreen::GameOver;
    }
}

 
bool ChessGame::saveGame()
{
    // Placeholder for saving game state
    return true;
}
bool ChessGame::loadGame()
{
    // Placeholder for loading game state
    return true;
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
