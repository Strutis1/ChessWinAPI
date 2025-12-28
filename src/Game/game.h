#ifndef GAME_H
#define GAME_H

#include "../Classes/board.h"
#include "../Classes/piece.h"
#include "../Classes/move.h"
#include "screens.h"
#include "appState.h"
#include "../Utility/saveLoad.h"



class ChessGame
{
private:
    PieceColor currentTurn;
    Piece currentSelected;
    bool gameOver;
    Board theBoard;
    int selectedX;
    int selectedY;
    bool hasUnsavedChanges = false;

    PieceColor winner;

    bool isPseudoLegalMove(const Board& board, const Move& move) const;
    bool isInCheck(const Board& board, PieceColor kingColor) const;
    bool findKing(const Board& board, PieceColor kingColor, int& outX, int& outY) const;


public:


    void init();
    bool isLegalMove(const Move& move);
    void makeMove(const Move& move);
    void checkGameOver();
    void checkForSavedGame();
    bool saveGame();
    bool loadGame();
    bool isCheck();
    bool isMate();
    bool getObstructionFreeMove(const Board& board, const Move& move) const;
    bool isValidPieceMovement(const Board& board, const Move& move) const;
    bool stalemate();
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
    PieceColor getWinner() const { return winner; }
    bool isGameOver() const { return gameOver; }
    bool hasDirtyState() const { return hasUnsavedChanges; }



};

extern ChessGame chessGame;


#endif
