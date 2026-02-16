#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <string>
#include "raylib.h"

#define NEWLINE_OFFSET 5

enum e_mode
{
    NORMAL,
    INSERT,
    COMMAND,
    MENU
};

struct s_file
{
	int							x_start;
	int							y_start;
    std::string					filename;
    std::vector<std::string>	lines;
};

class c_editor
{
public:
    std::vector<s_file> files;
    int cursor_row;
    int cursor_col;
    float window_scroll;
    float scroll_velocity;
    e_mode mode;
    bool just_enter_input_mode;
};

// input
void handle_insert_mode(c_editor &ed);
void handle_normal_mode(c_editor &ed);
void keyboard_input(c_editor &ed);
void remove_last_char(c_editor &ed);

// drawing
void draw_text(c_editor &ed);
void draw_cursor(c_editor &ed);

// file handling
s_file open_file(const std::string &filename);
void save_to_file(const c_editor &ed, const std::string &filename);

#endif