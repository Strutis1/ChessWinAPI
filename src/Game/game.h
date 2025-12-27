#ifndef GAME_H
#define GAME_H

#include "../Classes/board.h"
#include "../Classes/piece.h"
#include "../Classes/move.h"


class ChessGame
{
private:
    PieceColor currentTurn;
    Piece currentSelected;
    bool gameOver;
    Board theBoard;

public:


    void init();
    bool isLegalMove(const Move& move);
    void makeMove(const Move& move);
    void checkGameOver();
    bool saveGame();
    bool loadGame();
    bool isCheck();
    bool isMate();
    void switchTurn();
    void endGame();

    void setTheBoardUp();


    Piece detectSelection(int x, int y);

    void setSelectedPiece(const Piece& piece) { currentSelected = piece; };


    Piece getSelectedPiece() const { return currentSelected; };
    PieceColor getCurrentTurn() const { return currentTurn; }
    const Board& getBoard() const { return theBoard; }



};

extern ChessGame chessGame;


#endif