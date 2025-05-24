#include <iostream>

#include "JsEnv.h"

int main()
{
    bool runSucc = JsEnv::getInstance()->run();

    if (!runSucc) {
        std::cout << "JsEnv::run failed." << std::endl;
        return 1;
    }

    return 0;
}
