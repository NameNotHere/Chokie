#       include         <iostream>
#       include         "raylib.h"
#       include         "input.h"
#       include         <fstream>

bool is_file_valid(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
        return false;

    return true;
}

int main(int ac, char **av)
{
    c_editor ed;
    ed.files.push_back(open_file((std::string)av[1]));
    ed.cursor_row = 0;
    ed.cursor_col = 0;
    ed.window_scroll = 0;
    ed.window_scroll = 0.0f;
    ed.scroll_velocity = 0.0f;

    if (ac < 2)
    {
        std::cerr << "Usage: " << av[0] << " <filename>" << std::endl;
        return 1;
    }

    InitWindow(800, 450, "Chokie");
    SetExitKey(KEY_NULL);

    while (!WindowShouldClose())
    {
        keyboard_input(ed);
        draw_text(ed);
    }

    CloseWindow();
    save_to_file(ed, (std::string)av[1]);
    return 0;
}
