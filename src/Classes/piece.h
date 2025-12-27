#ifndef PIECE_H
#define PIECE_H



enum class PieceType
{
    NONE,
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

enum class PieceColor
{
    NONE,
    WHITE,
    BLACK
};

struct Piece
{
    PieceType type;
    PieceColor color;

    Piece() : type(PieceType::NONE), color(PieceColor::NONE) {}
    Piece(PieceType t, PieceColor c) : type(t), color(c) {}
};

#endif