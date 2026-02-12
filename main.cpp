#include <iostream>
#include "raylib.h"
#include "input.h"

int main()
{
    Editor ed;
    ed.lines = {""};
    ed.cursor_row = 0;
    ed.cursor_col = 0;
    ed.insert_mode = true;

    InitWindow(800, 450, "Chokie");
    SetExitKey(KEY_NULL); // Disable default Esc behavior

    while (!WindowShouldClose())
    {
        keyboard_input(ed);
        draw_text(ed);
    }

    CloseWindow();

    for (auto &line : ed.lines)
        std::cout << line << std::endl;

    return 0;
}
