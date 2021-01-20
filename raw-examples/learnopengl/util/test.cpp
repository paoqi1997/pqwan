#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "util.h"

using std::cout;
using std::endl;

int main()
{
    // currentpath: C:\Windows\System32\drivers\etc
    pqwan::File oFile("C:\\Windows\\System32\\drivers\\etc\\hosts");

    cout << oFile.getFileContent() << endl;

#ifdef _WIN32
    const char *cmd = "pause";
    int status = std::system(cmd);
    if (status != 0) {
        std::printf("Failed to call std::system(\"%s\")\n", cmd);
    }
#endif

    return 0;
}
