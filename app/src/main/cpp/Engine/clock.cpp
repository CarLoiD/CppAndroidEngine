#include "clock.h"

using ChronoTimePoint = std::chrono::steady_clock::time_point;
using ChronoHighResClock = std::chrono::high_resolution_clock;
using ChronoDuration = std::chrono::duration<float>;

ChronoTimePoint Clock::GetCurrentTimeTick()
{
    return std::chrono::high_resolution_clock::now();
}

Clock::Clock()
    : mTimeStart(GetCurrentTimeTick())
{}

float Clock::GetElapsedTime()
{
    mTimeEnd = GetCurrentTimeTick();

    const ChronoDuration elapsedTime = mTimeEnd - mTimeStart;
    return elapsedTime.count();
}

void Clock::Restart()
{
    mTimeStart = GetCurrentTimeTick();
}