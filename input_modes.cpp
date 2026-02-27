#include "input.h"
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

void    tree_input(c_editor &ed)
{
    std::vector<fs::path> entries;

    // Collect entries
    for (const auto &entry : fs::directory_iterator(ed.current_dir))
        entries.push_back(entry.path());

    if (IsKeyPressed(KEY_DOWN))
        ed.tree_active_file++;
    if (IsKeyPressed(KEY_UP))
        ed.tree_active_file--;
    if (IsKeyPressed(KEY_ESCAPE) && ed.mode == TREE_DIRECTORY)
        ed.mode = NORMAL;
    if (IsKeyPressed(KEY_ENTER))
    {
        fs::path selected = entries[ed.tree_active_file];

        if (fs::is_regular_file(selected))
        {
            ed.windows.push_back(create_window(0, 0, selected.string(), false));
            ed.focused_window = ed.windows.size() - 1;
        }
        else if (fs::is_directory(selected))
        {
            fs::current_path(selected);
            ed.tree_active_file = 0;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE) && ed.mode == TREE_DIRECTORY)
	    ed.mode = NORMAL;
}


void handle_insert_mode(c_editor &ed)
{
    s_file *file = get_active_file(ed);
    if (!file) return;

    int c = GetCharPressed();
    while (c > 0)
    {
        if (c >= 32 && c <= 126)
            ed.cursor = insert_char(*file, ed.cursor, (char)c);
        c = GetCharPressed();
    }
    if (IsKeyPressed(KEY_ENTER))
        ed.cursor = split_line(*file, ed.cursor);
    if (IsKeyPressed(KEY_BACKSPACE))
        ed.cursor = remove_last_char(*file, ed.cursor);
    if (IsKeyPressed(KEY_TAB))
        for (int i = 0; i < TAB_SIZE; i++)
            ed.cursor = insert_char(*file, ed.cursor, ' ');
    if (IsKeyPressed(KEY_UP) && ed.cursor.row > 0)
    {
        ed.cursor.row--;
        ed.cursor.col = std::min(ed.cursor.col, (int)file->lines[ed.cursor.row].size());
    }
    if (IsKeyPressed(KEY_DOWN) && ed.cursor.row < (int)file->lines.size() - 1)
    {
        ed.cursor.row++;
        ed.cursor.col = std::min(ed.cursor.col, (int)file->lines[ed.cursor.row].size());
    }
    if (IsKeyPressed(KEY_LEFT) && ed.cursor.col > 0)
        ed.cursor.col--;
    if (IsKeyPressed(KEY_RIGHT) && ed.cursor.col < (int)file->lines[ed.cursor.row].size())
        ed.cursor.col++;
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

int handle_command_mode(c_editor &ed)
{
    int c = GetCharPressed();
    while (c > 0)
    {
        if (c >= 32 && c <= 126)
            ed.command_input += (char)c;
        c = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !ed.command_input.empty())
        ed.command_input.pop_back();

    if (IsKeyPressed(KEY_ESCAPE) && ed.mode == COMMAND)
        ed.mode = TREE_DIRECTORY;

    if (IsKeyPressed(KEY_ENTER))
    {
        std::cout << "Command entered: " << ed.command_input << std::endl;

        if (strcmp(ed.command_input.c_str(), ":w") == 0)
        {
            if (!ed.windows.empty())
                save_to_file(ed, ed.windows[ed.focused_window].otvoren_file.filename);
        }
        if (strcmp(ed.command_input.c_str(), ":q") == 0)
        {
            ed.mode = TREE_DIRECTORY;
            return (1);
        }
        else
            ed.mode = NORMAL;
        ed.command_input.clear();
        ed.command_input = ":";
    }
    return 0;
}

int keyboard_input(c_editor &ed)
{
    if (ed.mode == INSERT)
        handle_insert_mode(ed);
    else if (ed.mode == NORMAL)
        handle_normal_mode(ed);
    else if (ed.mode == COMMAND)
    {
        if (handle_command_mode(ed) == 1)
            return 1;
    }
    else if (ed.mode == TREE_DIRECTORY)
        tree_input(ed);
    s_file *file = get_active_file(ed);
    if (file)
        file->scroll = update_scroll(*file, file->scroll, GetFrameTime());
    return 0;
}
