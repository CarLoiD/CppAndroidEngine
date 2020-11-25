#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

class Clock final
{
private:
    using ChronoTimePoint = std::chrono::steady_clock::time_point;

    ChronoTimePoint GetCurrentTimeTick();

public:
    Clock();

    float GetElapsedTime();
    void Restart();

private:
    ChronoTimePoint mTimeStart;
    ChronoTimePoint mTimeEnd;
};

#endif // CLOCK_H