#include "input.h"
#include <fstream>
#include <iostream>

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

void save_to_file(const s_editor &ed, const std::string &filename)
{
    std::ofstream out(filename);

    if (!out.is_open())
    {
        std::cerr << "Failed to open file\n";
        return;
    }

    for (size_t i = 0; i < ed.lines.size(); i++)
    {
        out << ed.lines[i];

        // Avoid extra newline at end of file
        if (i < ed.lines.size() - 1)
            out << '\n';
    }

    out.close();
}
