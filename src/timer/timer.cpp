#include "timer.h"
#include <algorithm>

Timer::Timer()
    : elapsed(0.0f), maxTime(30.0f * 60.0f)
{
}

void Timer::update(float dt)
{
    elapsed += dt;
    elapsed = std::min(elapsed, maxTime);
}

float Timer::getTime() const {
    return elapsed;
}

int Timer::getMinutes() const {
    return static_cast<int>(elapsed) / 60;
}

int Timer::getSeconds() const {
    return static_cast<int>(elapsed) % 60;
}

bool Timer::isFinished() const {
    return elapsed >= maxTime;
}
