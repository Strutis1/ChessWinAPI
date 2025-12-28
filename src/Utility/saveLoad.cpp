#include "saveLoad.h"


std::string pieceToCode(const Piece& p)
{
    if (p.type == PieceType::NONE) return "--";
    char color = (p.color == PieceColor::WHITE) ? 'w' : 'b';
    char type = '?';
    switch (p.type) {
    case PieceType::PAWN:   type = 'P'; break;
    case PieceType::ROOK:   type = 'R'; break;
    case PieceType::KNIGHT: type = 'N'; break;
    case PieceType::BISHOP: type = 'B'; break;
    case PieceType::QUEEN:  type = 'Q'; break;
    case PieceType::KING:   type = 'K'; break;
    default:                type = '?'; break;
    }
    return std::string() + color + type;
}

Piece codeToPiece(const std::string& code)
{
    if (code.size() != 2 || code == "--") return Piece();
    PieceColor color = (code[0] == 'w') ? PieceColor::WHITE :
                       (code[0] == 'b') ? PieceColor::BLACK : PieceColor::NONE;
    PieceType type = PieceType::NONE;
    switch (code[1]) {
    case 'P': type = PieceType::PAWN;   break;
    case 'R': type = PieceType::ROOK;   break;
    case 'N': type = PieceType::KNIGHT; break;
    case 'B': type = PieceType::BISHOP; break;
    case 'Q': type = PieceType::QUEEN;  break;
    case 'K': type = PieceType::KING;   break;
    default:  type = PieceType::NONE;   break;
    }
    return Piece(type, color);
}
