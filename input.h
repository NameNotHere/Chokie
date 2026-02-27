#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <string>
#include "raylib.h"
#include <iostream>
#include    <filesystem>

namespace fs = std::filesystem;
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1000
#define NEWLINE_OFFSET 5

enum e_mode
{
    NORMAL,
    INSERT,
    COMMAND,
    MENU,
    TREE_DIRECTORY
};

struct Cursor
{
    int row = 0;
    int col = 0;
};

struct ScrollState
{
    float offset = 0.f;
    float velocity = 0.f;
};

struct s_file
{
    std::string               filename;
    std::vector<std::string>  lines;
};

struct s_window
{
    s_file      otvoren_file;
    long long   window_x = 0;
    long long   window_y = 0;
    bool        is_special = false;
};

class c_editor
{
public:
    int                         focused_window = 0;
    std::vector<s_window>       windows;

    Cursor                      cursor;
    ScrollState                 scroll;

    e_mode                      mode = NORMAL;
    std::string                 command_input;
    bool                        just_enter_input_mode = false;

    fs::path                    current_dir = fs::current_path();
    int                         tree_active_file = 0;
};

// helpers that operate on the minimal state they need
Cursor      insert_char(s_file &file, Cursor cur, char c);
Cursor      remove_last_char(s_file &file, Cursor cur);
Cursor      split_line(s_file &file, Cursor cur);
ScrollState update_scroll(const s_file &file,
                          ScrollState current,
                          float dt);

void        handle_insert_mode(c_editor &ed);
void        handle_normal_mode(c_editor &ed);
void        keyboard_input(c_editor &ed);
void        remove_last_char(c_editor &ed);

// drawing
void draw_text(s_window &win, c_editor &ed);
void draw_cursor(c_editor &ed);
void draw_line_text(int x_start, int y, const std::string &line);

// file handling
s_file      open_file(const std::string &filename);
void        save_to_file(const c_editor &ed, const std::string &filename);
bool        is_file_valid(const std::string& filename);

// directory handling 
void    open_tree_view(c_editor &ed);
s_window	create_window(int xpos, int ypos, std::string file, bool is_special);

// command mode
void    handle_command_mode(c_editor &ed);

void    tree_input(c_editor &ed);

#endif // INPUT_H
