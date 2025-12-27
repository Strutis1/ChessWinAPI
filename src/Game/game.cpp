#include "game.h"
#include "../Classes/piece.h"
#include "screens.h"



void ChessGame::init()
{
    currentTurn = PieceColor::WHITE;
    gameOver = false;
    theBoard.reset();
    setTheBoardUp();
    selectedX = -1;
    selectedY = -1;
}
bool ChessGame::isLegalMove(const Move& move)
{
    // Placeholder for move legality check
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
    // Placeholder for game over check
    gameOver = false;
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
    // Placeholder for check detection
    return false;
}
bool ChessGame::isMate()
{
    // Placeholder for checkmate detection
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