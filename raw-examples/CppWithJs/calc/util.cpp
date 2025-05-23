#include <fstream>
#include <iostream>
#include <sstream>

#include "util.h"

std::string getFileContent(const std::string& filePath)
{
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cout << filePath << " is not open." << std::endl;
        return "";
    }

    std::stringstream inBuffer;
    inBuffer << inFile.rdbuf();

    return inBuffer.str();
}
