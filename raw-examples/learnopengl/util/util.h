#ifndef PQWAN_UTIL_H
#define PQWAN_UTIL_H

#include <fstream>
#include <string>

namespace pqwan
{

class File
{
public:
    File(const char *sFileName, std::ios_base::openmode mode = std::ios_base::in);
    ~File();
    const char* getFileContent() const { return fileContent.c_str(); }
private:
    std::ifstream is;
    std::string fileContent;
};

} // namespace pqwan

#endif // PQWAN_UTIL_H
