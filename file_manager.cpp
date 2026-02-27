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
    int filex = 10;
    int filey = 20;
    std::vector<fs::path> entries;

    // Collect entries
    for (const auto &entry : fs::directory_iterator(ed.current_dir))
        entries.push_back(entry.path());

    if (entries.empty())
        return;
    std::cout << "lol\n";
    // Clamp selection
    if (ed.tree_active_file < 0)
        ed.tree_active_file = 0;
    if (ed.tree_active_file >= (int)entries.size())
        ed.tree_active_file = entries.size() - 1;

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
        if (i == ed.tree_active_file)
            DrawRectangle(5, filey - 2, 400, 12, DARKGRAY);

        draw_line_text(filex, filey, label);
        filey += 25;
    }
    std::cout << "drew\n";
}

void    tree_input(c_editor &ed)
{
    std::vector<fs::path> entries;

    // Collect entries
    for (const auto &entry : fs::directory_iterator(ed.current_dir))
        entries.push_back(entry.path());
   // Navigation
    if (IsKeyPressed(KEY_DOWN))
        ed.tree_active_file++;

    if (IsKeyPressed(KEY_UP))
        ed.tree_active_file--;

    // Open selected file
    if (IsKeyPressed(KEY_ENTER))
    {
        fs::path selected = entries[ed.tree_active_file];

        if (fs::is_regular_file(selected))
        {
            ed.windows.push_back(create_window(0, 0, selected.string(), false));
            ed.focused_window = ed.windows.size() - 1;
        }
        else if (fs::is_directory(selected))
        {
            fs::current_path(selected);  // Enter directory
            ed.tree_active_file = 0;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE) && ed.mode == TREE_DIRECTORY)
	    ed.mode = NORMAL;
}
