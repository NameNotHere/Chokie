#include "input.h"

void draw_text(s_window &win, c_editor &ed)
{
    s_file &file = win.otvoren_file;

    if (file.lines.empty())
        return;

    int font_size = 30;

    int x_start = file.x_start + win.window_x;
    int y_start = file.y_start + win.window_y;

    int first_visible_line = ed.window_scroll / font_size;
    int max_visible_lines = GetScreenHeight() / font_size + 1;

    for (int i = 0; i < max_visible_lines; i++)
    {
        int row = first_visible_line + i;
        if (row >= (int)file.lines.size())
            break;

        int y = y_start + i * font_size;

        const std::string &line = file.lines[row];

        DrawText(line.c_str(), x_start, y, font_size, WHITE);

        if (row == ed.cursor_row)
        {
            int safe_col = std::min(ed.cursor_col, (int)line.length());
            std::string before_cursor = line.substr(0, safe_col);

            int cursor_x = x_start + MeasureText(before_cursor.c_str(), font_size);
            int cursor_y = y;

            if (ed.mode == INSERT)
                DrawLine(cursor_x, cursor_y, cursor_x, cursor_y + font_size, RED);
            else
                DrawRectangle(cursor_x, cursor_y, 10, font_size, RED);
        }
    }
}
