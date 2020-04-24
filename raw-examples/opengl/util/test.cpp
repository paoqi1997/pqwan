#include <cstdlib>
#include <iostream>

#include "util.h"

using namespace std;

int main()
{
    pqwan::File oFile("C:\\Windows\\System32\\drivers\\etc\\hosts");

    cout << oFile.getFileContent() << endl;

    system("pause");
    return 0;
}
