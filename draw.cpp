#include "input.h"

// utility used only in this file
static void get_visible_line_range(int &first_line, int &max_lines,
                                   c_editor &ed)
{
    int font_size = 30;
    first_line = static_cast<int>(ed.scroll.offset) / font_size;
    max_lines = GetScreenHeight() / font_size + 1;
}

void draw_line_text(int x_start, int y, const std::string &line)
{
    int font_size = 30;
    DrawText(line.c_str(), x_start, y, font_size, WHITE);
}

void draw_cursor(int x_start, int y, const std::string &line, c_editor &ed)
{
    int font_size = 30;
    int safe_col = std::min(ed.cursor.col, (int)line.length());
    std::string before_cursor = line.substr(0, safe_col);

    int cursor_x = x_start + MeasureText(before_cursor.c_str(), font_size);
    int cursor_y = y;

    if (ed.mode == INSERT)
        DrawLine(cursor_x, cursor_y, cursor_x, cursor_y + font_size, RED);
    else
        DrawRectangle(cursor_x, cursor_y, 10, font_size, RED);
}

void draw_text(s_window &win, c_editor &ed)
{
    s_file &file = win.otvoren_file;
    if (file.lines.empty())
        return;

    int font_size = 30;
    int x_start = win.window_x;
    int y_start = win.window_y;

    int first_visible_line, max_visible_lines;
    get_visible_line_range(first_visible_line, max_visible_lines, ed);

    for (int i = 0; i < max_visible_lines; i++)
    {
        int row = first_visible_line + i;
        if (row >= (int)file.lines.size())
            break;

        int y = y_start + i * font_size;
        const std::string &line = file.lines[row];

        draw_line_text(x_start, y, line);

        if (row == ed.cursor.row)
            draw_cursor(x_start, y, line, ed);
    }
}