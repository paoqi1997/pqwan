#include <iostream>

#include <v8.h>

int main()
{
    const char* version = v8::V8::GetVersion();

    std::cout << version << std::endl;

    return 0;
}
