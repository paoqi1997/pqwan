#include "stopwatch.h"

Stopwatch *Stopwatch::instance = nullptr;

void Stopwatch::addClock(uint interval, const std::string& funcName)
{
    clocks.emplace_back(interval, funcName);
}

void Stopwatch::onTick()
{
    std::uint64_t nowTime = nowMsec();

    tickEnd = nowTime;

    std::uint64_t thisTick = tickEnd - tickStart;

    passedTime += thisTick;

    for (auto it = clocks.begin(); it != clocks.end(); ) {
        it->waitingTime += thisTick;

        if (it->waitingTime >= it->interval) {
            it->waitingTime -= it->interval;
            onClock(&*it);
        }

        ++it;
    }

    tickStart = nowTime;
}
