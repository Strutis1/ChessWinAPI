#ifndef MOVE_H
#define MOVE_H

struct Move
{
    int fromX;
    int fromY;
    int toX;
    int toY;

    Move(int fx, int fy, int tx, int ty)
        : fromX(fx), fromY(fy), toX(tx), toY(ty) {}
};

struct MoveResult
{
    bool success;
    MoveResult(bool s = false, bool p = false)
        : success(s) {}
};

#endif