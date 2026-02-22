#       include         <iostream>
#       include         "raylib.h"
#       include         "input.h"
#       include         <fstream>

int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cerr << "Usage: " << av[0] << " <filename>" << std::endl;
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
        keyboard_input(ed);
        BeginDrawing();
        ClearBackground(DARKGRAY);
        if (ed.mode == INSERT)
            draw_text(ed.windows[ed.focused_window], ed);
        else if (ed.mode == COMMAND)
            draw_line_text(0, WINDOW_HEIGHT - 40, ed.command_input);
        else if (ed.mode == TREE_DIRECTORY)
            open_tree_view(ed);
        EndDrawing();
    }

    CloseWindow();

    save_to_file(ed, av[1]);

    return 0;
}
