#include "input.h"
#include <algorithm>
#include <cstring>

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

// pure helpers
Cursor insert_char(s_file &file, Cursor cur, char c)
{
    file.lines[cur.row].insert(cur.col, 1, c);
    return {cur.row, cur.col + 1};
}

Cursor remove_last_char(s_file &file, Cursor cur)
{
    if (cur.col > 0)
    {
        file.lines[cur.row].erase(cur.col - 1, 1);
        return {cur.row, cur.col - 1};
    }
    if (cur.row > 0)
    {
        cur.col = (int)file.lines[cur.row - 1].size();
        file.lines[cur.row - 1] += file.lines[cur.row];
        file.lines.erase(file.lines.begin() + cur.row);
        return {cur.row - 1, cur.col};
    }
    return cur;
}

Cursor split_line(s_file &file, Cursor cur)
{
    std::string remainder = file.lines[cur.row].substr(cur.col);
    file.lines[cur.row].erase(cur.col);
    file.lines.insert(file.lines.begin() + cur.row + 1, remainder);
    return {cur.row + 1, 0};
}

ScrollState update_scroll(const s_file &file,
                          ScrollState sc,
                          float dt)
{
    constexpr float scroll_speed = 300.0f;
    constexpr float friction     = 8.0f;

    float max_scroll = std::max(0.0f,
        (float)file.lines.size() * 30.0f - GetScreenHeight());

    if (IsKeyDown(KEY_UP))
        sc.velocity = -scroll_speed;
    else if (IsKeyDown(KEY_DOWN))
        sc.velocity = scroll_speed;
    else
        sc.velocity -= sc.velocity * friction * dt;

    sc.offset += sc.velocity * dt;
    sc.offset = std::clamp(sc.offset, 0.0f, max_scroll);
    return sc;
}

void handle_insert_mode(c_editor &ed)
{
    s_file *file = get_active_file(ed);
    if (!file) return;

    // --- Handle text input ---
    int c = GetCharPressed();
    while (c > 0)
    {
        if (c >= 32 && c <= 126) // printable ASCII only
        {
            ed.cursor = insert_char(*file, ed.cursor, (char)c);
        }
        c = GetCharPressed();
    }

    // --- Handle control keys ---
    if (IsKeyPressed(KEY_ENTER))
        ed.cursor = split_line(*file, ed.cursor);

    if (IsKeyPressed(KEY_BACKSPACE))
        ed.cursor = remove_last_char(*file, ed.cursor);

    if (IsKeyPressed(KEY_TAB))
    {
        for (int i = 0; i < 4; i++)
            ed.cursor = insert_char(*file, ed.cursor, ' ');
    }

    if (IsKeyPressed(KEY_ESCAPE) && ed.mode == INSERT)
        ed.mode = NORMAL;
}

void handle_normal_mode(c_editor &ed)
{
    s_file *file = get_active_file(ed);
    if (!file) return;

    if (IsKeyPressed(KEY_ENTER))
        ed.mode = COMMAND;
    if (IsKeyPressed(KEY_H) && ed.cursor.col > 0)
        ed.cursor.col--;
    if (IsKeyPressed(KEY_L) &&
        ed.cursor.col < (int)file->lines[ed.cursor.row].size())
        ed.cursor.col++;
    if (IsKeyPressed(KEY_K) && ed.cursor.row > 0)
    {
        ed.cursor.row--;
        ed.cursor.col = std::min(ed.cursor.col,
                                 (int)file->lines[ed.cursor.row].size());
    }
    if (IsKeyPressed(KEY_J) &&
        ed.cursor.row < (int)file->lines.size() - 1)
    {
        ed.cursor.row++;
        ed.cursor.col = std::min(ed.cursor.col,
                                 (int)file->lines[ed.cursor.row].size());
    }
    if (IsKeyPressed(KEY_I))
    {
        ed.mode = INSERT;
    }
    if (IsKeyPressed(KEY_ESCAPE) && ed.mode == NORMAL)
	    ed.mode = TREE_DIRECTORY;
}

void handle_command_mode(c_editor &ed)
{
    int key = GetKeyPressed();

    while (key > 0)
    {
        if (key == KEY_BACKSPACE)
        {
            if (!ed.command_input.empty())
                ed.command_input.pop_back();
        }
	else if (key == KEY_ESCAPE && ed.mode == COMMAND)
	{
		ed.mode = TREE_DIRECTORY;
		break ;
	}
        else if (key == KEY_ENTER)
        {
            std::cout << "Command entered: " << ed.command_input << std::endl;

            if (ed.command_input == ":q")
            {
                ed.mode = TREE_DIRECTORY;
                ed.command_input.clear();
                ed.command_input = ":";
                return;
            }
            ed.command_input.clear();
            ed.command_input = ":";
            ed.mode = NORMAL;
            return;
        }
        else if (key >= 32 && key <= 126)  // Printable ASCII
        {
            ed.command_input += key;
        }
        key = GetKeyPressed(); // get next queued key
    }
}

void keyboard_input(c_editor &ed)
{
    if (ed.mode == INSERT)
        handle_insert_mode(ed);
    else if (ed.mode == NORMAL)
        handle_normal_mode(ed);
    else if (ed.mode == COMMAND)
        handle_command_mode(ed);
    else if (ed.mode == TREE_DIRECTORY)
        tree_input(ed);    
    s_file *file = get_active_file(ed);
    if (file)
        ed.scroll = update_scroll(*file, ed.scroll, GetFrameTime());
}
