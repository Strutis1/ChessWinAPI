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
    int selectedX;
    int selectedY;

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
    void setSelectedPosition(int x, int y) { selectedX = x; selectedY = y; };
    void clearSelectedPiece() { currentSelected = Piece(); selectedX = -1; selectedY = -1; };
    void clearSelectedPosition() { selectedX = -1; selectedY = -1; };


    Piece getSelectedPiece() const { return currentSelected; };
    int getSelectedPosX() const { return selectedX; };
    int getSelectedPosY() const { return selectedY; };
    PieceColor getCurrentTurn() const { return currentTurn; }
    const Board& getBoard() const { return theBoard; }



};

extern ChessGame chessGame;


#endif