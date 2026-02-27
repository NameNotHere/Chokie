#       include         <iostream>
#       include         "input.h"
#       include         "fstream"

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
    // Clamp selection
    if (ed.tree_active_file < 0)
        ed.tree_active_file = 0;
    if (ed.tree_active_file >= (int)entries.size())
        ed.tree_active_file = entries.size() - 1;

    DrawRectangle(WINDOW_WIDTH / 2, (ed.file_view * 25) + 70, 100, 25, BLACK);

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

        if (i == ed.tree_active_file)
            DrawRectangle(5, filey + 5, 400, 20, RED);
        draw_line_text(filex, filey, label, ed.font_size);
        filey += 25;
    }
    draw_line_text(WINDOW_WIDTH / 2, 0, "file explorer", 60);
    filey = 70;
    filex = WINDOW_WIDTH / 2;
    for (const auto &entry : ed.windows)
    {
        draw_line_text(filex, filey, entry.otvoren_file.FILEPATH, ed.font_size);
        filey += 25;
    }
}

int handle_windows(c_editor &ed)
{
    if (ed.windows.empty())
        return -1;
    if (ed.focused_window >= (int)ed.windows.size())
        return -1;
    if (ed.mode != TREE_DIRECTORY)
	{
		draw_text(ed.windows[ed.focused_window], ed);
        if (ed.mode == COMMAND)
    		draw_line_text(0, WINDOW_HEIGHT - 40, ed.command_input, ed.font_size);
	}
	else if (ed.mode == TREE_DIRECTORY)
	{
		open_tree_view(ed);
    }
    return (1);
}

int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cerr << "Usage: " << av[0] << " <filepath>" << std::endl;
        return 1;
    }

    c_editor ed;
    ed.mode = NORMAL;

    ed.windows.push_back(create_window(0, 0, av[1], false));
    ed.command_input = ":";
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chokie");
    SetExitKey(KEY_NULL);

    while (!WindowShouldClose())
    {
        if (keyboard_input(ed) == 1)
            break ;
        BeginDrawing();
        ClearBackground(DARKGRAY);
	    if (handle_windows(ed) == -1)
            return 1;
        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}
