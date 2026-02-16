#include "input.h"
#include <fstream>
#include <iostream>

void save_to_file(const c_editor &ed, const std::string &filename)
{
    std::ofstream out(filename);

    if (!out.is_open())
    {
        std::cerr << "Failed to open file\n";
        return;
    }
    for (size_t i = 0; i < ed.files.size(); i++)
    {
        const s_file &file = ed.files[i];
        for (size_t j = 0; j < file.lines.size(); j++)
        {
            out << file.lines[j];
            if (j < file.lines.size() - 1)
                out << '\n';
        }
    }

    out.close();
}

std::vector<std::string> read_file(const std::string &filename)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
    }
    else
        lines.push_back("");
    return lines;
}

s_file open_file(const std::string &filename)
{
    s_file file;
    file.filename = filename;
    file.lines = read_file(filename);
    return file;
}