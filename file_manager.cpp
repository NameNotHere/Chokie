#include    <filesystem>
#include    "./input.h"

namespace fs = std::filesystem;

s_window	create_window(int xpos, int ypos, std::string file, bool is_special)
{
	s_window	win;
	win.otvoren_file.filename = file;
	win.window_x = xpos;
	win.window_y = ypos;
	win.is_special = is_special;
	win.otvoren_file = open_file(file);

	return (win);
}
void open_tree_view(c_editor &ed)
{
    static int active_file = 0;

    int filex = 10;
    int filey = 20;

    fs::path current_dir = fs::current_path();
    std::vector<fs::path> entries;

    // Collect entries
    for (const auto &entry : fs::directory_iterator(current_dir))
        entries.push_back(entry.path());

    if (entries.empty())
        return;

    // Clamp selection
    if (active_file < 0)
        active_file = 0;
    if (active_file >= (int)entries.size())
        active_file = entries.size() - 1;

    // Draw entries
    for (int i = 0; i < (int)entries.size(); i++)
    {
        std::string label;

        if (fs::is_directory(entries[i]))
            label = "[DIR]  ";
        else if (fs::is_regular_file(entries[i]))
            label = "[FILE] ";
        else
            label = "[OTHER] ";

        label += entries[i].filename().string();

        // Highlight active file
        if (i == active_file)
            DrawRectangle(5, filey - 2, 400, 12, DARKGRAY);

        draw_line_text(filex, filey, label);
        filey += 25;
    }

    // Navigation
    if (IsKeyPressed(KEY_DOWN))
        active_file++;

    if (IsKeyPressed(KEY_UP))
        active_file--;

    // Open selected file
    if (IsKeyPressed(KEY_ENTER))
    {
        fs::path selected = entries[active_file];

        if (fs::is_regular_file(selected))
        {
            ed.windows.push_back(create_window(0, 0, selected.string(), false));
            ed.focused_window = ed.windows.size() - 1;
        }
        else if (fs::is_directory(selected))
        {
            fs::current_path(selected);  // Enter directory
            active_file = 0;
        }
    }
}