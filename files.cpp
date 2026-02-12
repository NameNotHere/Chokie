#include "input.h"
#include <fstream>

std::vector<std::string> read_file(const std::string &filename)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    return lines;
}
