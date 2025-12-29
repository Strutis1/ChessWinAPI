#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <windows.h>
#include <chrono>
class GameTimer
{
    std::chrono::milliseconds remainingMs;
    std::chrono::steady_clock::time_point lastUpdate;
    std::chrono::milliseconds initialMs;
    bool running = false;
public:
    GameTimer();
    GameTimer(std::chrono::milliseconds initialTime);
    void start(std::chrono::milliseconds initialTime);
    void resume();
    void pause();
    void update();
    void reset();
    bool isTimeUp() const;
    void setRemaining(std::chrono::milliseconds ms);
    std::chrono::milliseconds getRemainingTime() const;
};
#endif