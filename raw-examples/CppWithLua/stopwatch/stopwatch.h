#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <list>
#include <string>

using uint = unsigned int;

template <typename T>
std::uint64_t nowT()
{
    std::chrono::time_point<std::chrono::system_clock, T> tp = std::chrono::time_point_cast<T>(std::chrono::system_clock::now());
    return tp.time_since_epoch().count();
}

inline std::uint64_t nowSec()
{
    return nowT<std::chrono::seconds>();
}

inline std::uint64_t nowMsec()
{
    return nowT<std::chrono::milliseconds>();
}

struct Clock
{
    Clock(uint _interval, const std::string& _funcName)
        : interval(_interval), waitingTime(0), funcName(_funcName) {}
    uint interval;
    std::uint64_t waitingTime;
    std::string funcName;
};

class Stopwatch
{
public:
    static Stopwatch* getInstance() {
        if (instance == nullptr) {
            instance = new Stopwatch(20);
        }
        return instance;
    }
    Stopwatch(const Stopwatch&) = delete;
    Stopwatch& operator = (const Stopwatch&) = delete;
    // 毫秒级
    void addClock(uint interval, const std::string& funcName);
    void onTick();
    void onClock(Clock *clock);
    std::size_t getTick() const { return tick; }
    std::uint64_t getPassedTime() const { return passedTime; }
private:
    Stopwatch(std::size_t _tick) : tick(_tick), passedTime(0) {
        std::uint64_t nowTime = nowMsec();
        tickStart = tickEnd = nowTime;
    }
    static Stopwatch *instance;
    std::size_t tick; // 每隔多少毫秒触发一次
    std::uint64_t tickStart;
    std::uint64_t tickEnd;
    std::uint64_t passedTime; // 已流逝的毫秒数
    std::list<Clock> clocks;
};

#endif // STOPWATCH_H
