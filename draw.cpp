#include "input.h"

void draw_text(s_editor &ed)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    int font_size = 30;
    int y_start = 100;
    int cursor_x = 50;
    int cursor_y = y_start + ed.cursor_row * font_size;

    for (int row = 0; row < (int)ed.lines.size(); row++)
    {
        std::string line = ed.lines[row];
        DrawText(line.c_str(), 5, y_start + (row - ed.window_scroll) * font_size, font_size, BLACK);
        DrawLine(50, y_start + (row - ed.window_scroll) * font_size + font_size, 50 + MeasureText(line.c_str(), font_size), y_start + (row - ed.window_scroll) * font_size + font_size, BLACK);
        if (row == ed.cursor_row)
        {
            std::string before_cursor = line.substr(0, ed.cursor_col);
            cursor_x = MeasureText(before_cursor.c_str(), font_size);
        }
    }

    if (ed.insert_mode)
        DrawLine(cursor_x, cursor_y, cursor_x, cursor_y + font_size, RED);
    else
        DrawRectangle(cursor_x, cursor_y, 10, font_size, RED);
    EndDrawing();
}
