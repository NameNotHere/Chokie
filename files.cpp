#include "input.h"
#include <fstream>
#include <iostream>

void save_to_file(const c_editor &ed, const std::string &filename)
{
    if (ed.windows.empty())
        return;

    const s_file &file = ed.windows[0].otvoren_file;

    std::ofstream out(filename);
    if (!out.is_open())
        return;

    for (size_t i = 0; i < file.lines.size(); i++)
    {
        out << file.lines[i];
        if (i != file.lines.size() - 1)
            out << "\n";
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

bool is_file_valid(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
        return false;
    return true;
}
