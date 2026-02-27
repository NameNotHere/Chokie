#include "input.h"
#include <fstream>
#include <iostream>

void save_to_file(const c_editor &ed, const std::string &filepath)
{
    if (ed.windows.empty())
        return;

    const s_file &file = ed.windows[ed.focused_window].otvoren_file;

    std::ofstream out(filepath);
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

std::vector<std::string> read_file(const std::string &filepath)
{
    std::vector<std::string> lines;
    std::ifstream file(filepath);
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

s_file open_file(const std::string &filepath)
{
    s_file file;
    file.FILEPATH = filepath;
    file.lines = read_file(filepath);

    // default positioning/appearance state
    file.x = 0;
    file.y = 0;
    file.scroll = ScrollState{};

    return file;
}

bool is_file_valid(const std::string& filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open())
        return false;
    return true;
}
