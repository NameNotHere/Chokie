#       include         <iostream>
#       include         "raylib.h"
#       include         "input.h"
#       include         <fstream>

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

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chokie");
    SetExitKey(KEY_NULL);

    while (!WindowShouldClose())
    {
        keyboard_input(ed);
        BeginDrawing();
        ClearBackground(DARKGRAY);
        for (auto &win : ed.windows)
            draw_text(win, ed);
        EndDrawing();
    }

    CloseWindow();

    save_to_file(ed, av[1]);

    return 0;
}
