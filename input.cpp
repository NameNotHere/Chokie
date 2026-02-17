#include "input.h"
#include <algorithm>

static s_window* get_active_window(c_editor &ed)
{
    if (ed.windows.empty())
        return nullptr;

    if (ed.focused_window >= (int)ed.windows.size())
        return nullptr;

    return &ed.windows[ed.focused_window];
}

static s_file* get_active_file(c_editor &ed)
{
    s_window *win = get_active_window(ed);
    if (!win)
        return nullptr;

    return &win->otvoren_file;
}


void remove_last_char(c_editor &ed)
{
    s_file *file = get_active_file(ed);
    if (!file || file->lines.empty())
        return;

    if (ed.cursor_col > 0)
    {
        file->lines[ed.cursor_row].erase(ed.cursor_col - 1, 1);
        ed.cursor_col--;
    }
    else if (ed.cursor_row > 0)
    {
        ed.cursor_col = (int)file->lines[ed.cursor_row - 1].size();
        file->lines[ed.cursor_row - 1] += file->lines[ed.cursor_row];
        file->lines.erase(file->lines.begin() + ed.cursor_row);
        ed.cursor_row--;
    }
}

static void handle_char_input(c_editor &ed)
{
    s_file *file = get_active_file(ed);
    if (!file)
        return;

    int key = GetCharPressed();

    while (key > 0)
    {
        if (ed.just_enter_input_mode)
        {
            while (key > 0)
                key = GetCharPressed();

            ed.just_enter_input_mode = false;
            return;
        }

        char c = static_cast<char>(key);

        if (c >= 32 && c <= 126 && ed.mode == INSERT)
        {
            file->lines[ed.cursor_row].insert(ed.cursor_col, 1, c);
            ed.cursor_col++;
        }

        key = GetCharPressed();
    }
}

static void handle_tab(c_editor &ed)
{
    if (ed.mode != INSERT)
        return;

    s_file *file = get_active_file(ed);
    if (!file)
        return;

    if (IsKeyPressed(KEY_TAB))
    {
        file->lines[ed.cursor_row].insert(ed.cursor_col, "    ");
        ed.cursor_col += 4;
    }
}

void handle_scroll(c_editor &ed)
{
    s_file *file = get_active_file(ed);
    if (!file)
        return;

    float dt = GetFrameTime();
    float scroll_speed = 300.0f;
    float friction = 8.0f;

    float max_scroll = std::max(
        0.0f,
        (float)file->lines.size() * 30.0f - GetScreenHeight());

    if (IsKeyDown(KEY_UP))
        ed.scroll_velocity = -scroll_speed;
    else if (IsKeyDown(KEY_DOWN))
        ed.scroll_velocity = scroll_speed;
    else
        ed.scroll_velocity -= ed.scroll_velocity * friction * dt;

    ed.window_scroll += ed.scroll_velocity * dt;

    ed.window_scroll = std::clamp(ed.window_scroll, 0.0f, max_scroll);
}

static void handle_enter(c_editor &ed)
{
    if (ed.mode != INSERT)
        return;

    s_file *file = get_active_file(ed);
    if (!file)
        return;

    if (IsKeyPressed(KEY_ENTER))
    {
        std::string remainder =
            file->lines[ed.cursor_row].substr(ed.cursor_col);

        file->lines[ed.cursor_row] =
            file->lines[ed.cursor_row].substr(0, ed.cursor_col);

        file->lines.insert(
            file->lines.begin() + ed.cursor_row + 1,
            remainder);

        ed.cursor_row++;
        ed.cursor_col = 0;
    }
}

static void handle_backspace(c_editor &ed)
{
    if (ed.mode != INSERT) return;
    if (IsKeyPressed(KEY_BACKSPACE))
        remove_last_char(ed);
}

static void handle_escape(c_editor &ed)
{
    if (IsKeyPressed(KEY_ESCAPE))
        ed.mode = NORMAL;
}

void handle_insert_mode(c_editor &ed)
{
    handle_char_input(ed);
    handle_tab(ed);
    handle_enter(ed);
    handle_backspace(ed);
    handle_escape(ed);
}

void handle_normal_mode(c_editor &ed)
{
    s_file *file = get_active_file(ed);
    if (!file)
        return;

    if (IsKeyPressed(KEY_H) && ed.cursor_col > 0)
        ed.cursor_col--;

    if (IsKeyPressed(KEY_L) &&
        ed.cursor_col < (int)file->lines[ed.cursor_row].size())
        ed.cursor_col++;

    if (IsKeyPressed(KEY_K) && ed.cursor_row > 0)
    {
        ed.cursor_row--;
        ed.cursor_col = std::min(
            ed.cursor_col,
            (int)file->lines[ed.cursor_row].size());
    }

    if (IsKeyPressed(KEY_J) &&
        ed.cursor_row < (int)file->lines.size() - 1)
    {
        ed.cursor_row++;
        ed.cursor_col = std::min(
            ed.cursor_col,
            (int)file->lines[ed.cursor_row].size());
    }

    if (IsKeyPressed(KEY_I))
    {
        ed.just_enter_input_mode = true;
        ed.mode = INSERT;
    }
}


// =====================
// Main Keyboard Dispatcher
// =====================
void keyboard_input(c_editor &ed)
{
    if (ed.mode == INSERT)
    {
        handle_insert_mode(ed);
    }
    else
    {
        handle_normal_mode(ed);
        return;  // 🔥 IMPORTANT
    }

    handle_scroll(ed);
}