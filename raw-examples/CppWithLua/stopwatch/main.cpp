#include <chrono>
#include <thread>

#include "luai.h"
#include "stopwatch.h"

int main()
{
    LuaEnv::getInstance()->run();

    Stopwatch *stopwatch = Stopwatch::getInstance();

    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(stopwatch->getTick()));
        stopwatch->onTick();
    }

    return 0;
}
