#ifndef BOARD_H
#define BOARD_H

#include "piece.h"


struct Board
{
    Piece squares[8][8];

    Board();
    void reset();
    void clearAt(int x, int y);
    Piece getPieceAt(int x, int y) const;
    void setPieceAt(int x, int y, const Piece& piece);
    void movePiece(int fromX, int fromY, int toX, int toY);
};




#endif