#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <windows.h>
#include <chrono>
class GameTimer
{

public:
    std::chrono::milliseconds remainingMs;
    std::chrono::steady_clock::time_point lastUpdate;
    std::chrono::milliseconds initialMs;
    bool running = false;

    GameTimer();
    void start(std::chrono::milliseconds initialTime);
    void pause();
    void update();
    void reset();
    void tick(std::chrono::milliseconds nowMs);
    bool isTimeUp() const;
};
#endif