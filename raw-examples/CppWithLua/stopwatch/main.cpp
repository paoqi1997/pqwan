#include <chrono>
#include <iostream>
#include <thread>

#include "luai.h"
#include "stopwatch.h"

int main()
{
    bool runSucc = LuaEnv::getInstance()->run();

    if (!runSucc) {
        std::cout << "LuaEnv::run failed." << std::endl;
        return 1;
    }

    Stopwatch *stopwatch = Stopwatch::getInstance();

    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(stopwatch->getTick()));
        stopwatch->onTick();
    }

    return 0;
}
