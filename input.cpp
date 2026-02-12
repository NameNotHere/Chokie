#include "input.h"
#include <unistd.h>
#include <algorithm>

// Remove last character
void remove_last_char(Editor &ed)
{
    if (ed.cursor_col > 0)
    {
        ed.lines[ed.cursor_row].erase(ed.cursor_col - 1, 1);
        ed.cursor_col--;
    }
    else if (ed.cursor_row > 0)
    {
        ed.cursor_col = (int)ed.lines[ed.cursor_row - 1].size();
        ed.lines[ed.cursor_row - 1] += ed.lines[ed.cursor_row];
        ed.lines.erase(ed.lines.begin() + ed.cursor_row);
        ed.cursor_row--;
    }
}

// Insert mode functions
static void handle_char_input(Editor &ed)
{
    int key = GetCharPressed();
    while (key > 0)
    {
        char c = static_cast<char>(key);
        if (c >= 32 && c <= 126)
        {
            ed.lines[ed.cursor_row].insert(ed.cursor_col, 1, c);
            ed.cursor_col++;
        }
        key = GetCharPressed();
    }
}

static void handle_tab(Editor &ed)
{
    if (IsKeyPressed(KEY_TAB))
    {
        ed.lines[ed.cursor_row].insert(ed.cursor_col, "    ");
        ed.cursor_col += 4;
    }
}

static void handle_enter(Editor &ed)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        std::string remainder = ed.lines[ed.cursor_row].substr(ed.cursor_col);
        ed.lines[ed.cursor_row] = ed.lines[ed.cursor_row].substr(0, ed.cursor_col);
        ed.lines.insert(ed.lines.begin() + ed.cursor_row + 1, remainder);
        ed.cursor_row++;
        ed.cursor_col = 0;
    }
}

static void handle_backspace(Editor &ed)
{
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        remove_last_char(ed);
    }
}

static void handle_escape(Editor &ed)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        ed.insert_mode = false;
        usleep(10000);
    }
}

void handle_insert_mode(Editor &ed)
{
    handle_char_input(ed);
    handle_tab(ed);
    handle_enter(ed);
    handle_backspace(ed);
    handle_escape(ed);
}

// Normal mode functions
void handle_normal_mode(Editor &ed)
{
    if (IsKeyPressed(KEY_H)) if (ed.cursor_col > 0) ed.cursor_col--;
    if (IsKeyPressed(KEY_L)) if (ed.cursor_col < (int)ed.lines[ed.cursor_row].size()) ed.cursor_col++;
    if (IsKeyPressed(KEY_K)) if (ed.cursor_row > 0) ed.cursor_row--, ed.cursor_col = std::min(ed.cursor_col, (int)ed.lines[ed.cursor_row].size());
    if (IsKeyPressed(KEY_J)) if (ed.cursor_row < (int)ed.lines.size() - 1) ed.cursor_row++, ed.cursor_col = std::min(ed.cursor_col, (int)ed.lines[ed.cursor_row].size());
    if (IsKeyPressed(KEY_I)) ed.insert_mode = true;
}

void keyboard_input(Editor &ed)
{
    if (ed.insert_mode)
        handle_insert_mode(ed);
    else
        handle_normal_mode(ed);
}

void draw_text(Editor &ed)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    int font_size = 30;
    int y_start = 200;
    int cursor_x = 50;
    int cursor_y = y_start + ed.cursor_row * font_size;

    for (int row = 0; row < (int)ed.lines.size(); row++)
    {
        std::string line = ed.lines[row];
        DrawText(line.c_str(), 50, y_start + row * font_size, font_size, BLACK);
        if (row == ed.cursor_row)
        {
            std::string before_cursor = line.substr(0, ed.cursor_col);
            cursor_x = 50 + MeasureText(before_cursor.c_str(), font_size);
        }
    }

    if (ed.insert_mode)
        DrawLine(cursor_x, cursor_y, cursor_x, cursor_y + font_size, RED);
    else
        DrawRectangle(cursor_x, cursor_y, 10, font_size, RED);

    EndDrawing();
}
