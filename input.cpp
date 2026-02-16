#include "input.h"
#include <unistd.h>

void remove_last_char(s_editor &ed)
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

static void handle_char_input(s_editor &ed)
{
    int key = GetCharPressed();
    char c;
    
    while (key > 0)
    {
        if (ed.just_enter_input_mode)
        {
            while (key > 0)
                key = GetCharPressed();
            ed.just_enter_input_mode = false;
            continue ;
        }
        c = static_cast<char>(key);
        if (c >= 32 && c <= 126 && ed.insert_mode == true)
        {
            ed.lines[ed.cursor_row].insert(ed.cursor_col, 1, c);
            ed.cursor_col++;
        }
        key = GetCharPressed();
    }
}

static void handle_tab(s_editor &ed)
{
    if (IsKeyPressed(KEY_TAB))
    {
        ed.lines[ed.cursor_row].insert(ed.cursor_col, "    ");
        ed.cursor_col += 4;
    }
}

void handle_scroll(s_editor &ed)
{
    float dt = GetFrameTime();   // frame time (smooth movement)
    float scroll_speed = 10.0f; // pixels per second
    float friction = 8.0f;       // how fast it slows down
    float max_scroll = ed.lines.size() * 30.0f;

    if (IsKeyDown(KEY_UP))
        ed.scroll_velocity = -scroll_speed;
    else if (IsKeyDown(KEY_DOWN))
        ed.scroll_velocity = scroll_speed;
    else
        ed.scroll_velocity -= ed.scroll_velocity * friction * dt;
    ed.window_scroll += ed.scroll_velocity * dt;
    ed.cursor_row = (int)(ed.window_scroll / 30.0f);
    ed.cursor_col = std::min(ed.cursor_col, (int)ed.lines[ed.cursor_row].size());
    max_scroll = ed.lines.size() * 30.0f;

    if (ed.window_scroll < 0)
        ed.window_scroll = 0;

    if (ed.window_scroll > max_scroll)
        ed.window_scroll = max_scroll;
    ed.cursor_row = ed.cursor_row * scroll_speed * dt;
}

static void handle_enter(s_editor &ed)
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

static void handle_backspace(s_editor &ed)
{
    if (IsKeyPressed(KEY_BACKSPACE))
        remove_last_char(ed);
}

static void handle_escape(s_editor &ed)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        ed.insert_mode = false;
    }
}

void handle_insert_mode(s_editor &ed)
{
    handle_char_input(ed);
    handle_tab(ed);
    handle_enter(ed);
    handle_backspace(ed);
    handle_escape(ed);
}

void handle_normal_mode(s_editor &ed)
{
    if (IsKeyPressed(KEY_H))
        if (ed.cursor_col > 0) ed.cursor_col--;
    if (IsKeyPressed(KEY_L))
        if (ed.cursor_col < (int)ed.lines[ed.cursor_row].size())
            ed.cursor_col++;
    if (IsKeyPressed(KEY_K))
    {
        if (ed.cursor_row > 0)
        {
            ed.cursor_row--;
            ed.cursor_col = std::min(ed.cursor_col, (int)ed.lines[ed.cursor_row].size());
        }
    }
    if (IsKeyPressed(KEY_J))
    {
        if (ed.cursor_row < (int)ed.lines.size() - 1)
        {
            ed.cursor_row++;
            ed.cursor_col = std::min(ed.cursor_col, (int)ed.lines[ed.cursor_row].size());
        }
    }
    if (IsKeyPressed(KEY_I))
    {
        ed.just_enter_input_mode = true;
        ed.insert_mode = true;
    }
}

void keyboard_input(s_editor &ed)
{
    if (ed.insert_mode)
        handle_insert_mode(ed);
    else
        handle_normal_mode(ed);
    handle_scroll(ed);
}