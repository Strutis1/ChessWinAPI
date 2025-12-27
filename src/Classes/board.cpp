#include "board.h"


Board::Board()
{
    reset();
}

void Board::reset()
{
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            squares[y][x] = Piece();
        }
    }
}

void Board::clearAt(int x, int y)
{
    squares[y][x] = Piece();
}

Piece Board::getPieceAt(int x, int y) const
{
    return squares[y][x];
}

void Board::setPieceAt(int x, int y, const Piece& piece)
{
    squares[y][x] = piece;
}

void Board::movePiece(int fromX, int fromY, int toX, int toY)
{
    Piece movingPiece = getPieceAt(fromX, fromY);
    setPieceAt(toX, toY, movingPiece);
    setPieceAt(fromX, fromY, Piece());
}
