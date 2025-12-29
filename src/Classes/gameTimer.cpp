#include "gameTimer.h"



GameTimer::GameTimer()
    : remainingMs(0), lastUpdate(std::chrono::steady_clock::now()), initialMs(0), running(false)
{
}

GameTimer::GameTimer(std::chrono::milliseconds initialTime)
    : remainingMs(initialTime), lastUpdate(std::chrono::steady_clock::now()), initialMs(initialTime), running(false)
{
}

void GameTimer::start(std::chrono::milliseconds initialTime)
{
    initialMs = initialTime;
    remainingMs = initialTime;
    lastUpdate = std::chrono::steady_clock::now();
}
void GameTimer::resume()
{
    lastUpdate = std::chrono::steady_clock::now();
    running = true;
}

void GameTimer::pause()
{
    running = false;
}

void GameTimer::update()
{
    if (!running) return;
    auto now = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
    if (delta.count() > 0)
    {
        if (remainingMs > delta)
            remainingMs -= delta;
        else
            remainingMs = std::chrono::milliseconds(0);
        lastUpdate = now;
    }
}
void GameTimer::reset()
{
    remainingMs = initialMs;
    lastUpdate = std::chrono::steady_clock::now();
    running = false;
}
bool GameTimer::isTimeUp() const
{
    return remainingMs.count() <= 0;
}

void GameTimer::setRemaining(std::chrono::milliseconds ms)
{
    remainingMs = ms;
}

std::chrono::milliseconds GameTimer::getRemainingTime() const
{
    return remainingMs;
}
