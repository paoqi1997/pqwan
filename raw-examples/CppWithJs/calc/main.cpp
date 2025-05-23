#include <iostream>

#include "JsEnv.h"
#include "util.h"

int main()
{
    JsEnv* jsEnv = JsEnv::getInstance();

    std::string result = jsEnv->runJsFile("main.js");

    std::cout << result << std::endl;

    return 0;
}
