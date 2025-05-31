#include <cassert>
#include <cstdio>
#include <vector>

#include "util.h"

using namespace pqwan;

File::File(const char *sFileName, std::ios_base::openmode mode)
    : is(sFileName, mode)
{
    if (!is.is_open()) {
        std::printf("Failed to open %s.\n", sFileName);
    } else {
        is.seekg(0, std::ios::end);
        std::size_t iLength = is.tellg();
        is.seekg(0, std::ios::beg);
        std::vector<char> pool(iLength);
        is.read(pool.data(), pool.size());
        fileContent.assign(pool.begin(), pool.end());
        assert(pool.size() == fileContent.size());
    }
}

File::~File()
{
    if (is.is_open()) {
        is.close();
    }
}
