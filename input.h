#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <string>
#include "raylib.h"
#include <iostream>

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
	int							x_start = 0;
	int							y_start = 0;
    std::string					filename;
    std::vector<std::string>	lines;
};

struct  s_window
{
    s_file		otvoren_file;
    long long	window_x = 0;
	long long	window_y = 0;
	bool		is_special = false;
};

class c_editor
{
public:
	int						focused_window = 0;
    std::vector<s_window>	windows;
    int 					cursor_row = 0;
    int						cursor_col = 0;
    float					window_scroll = 0;
    float					scroll_velocity = 0;
    e_mode					mode;
    bool					just_enter_input_mode = 0;
};

// input
void handle_insert_mode(c_editor &ed);
void handle_normal_mode(c_editor &ed);
void keyboard_input(c_editor &ed);
void remove_last_char(c_editor &ed);

// drawing
void draw_text(s_window &win, c_editor &ed);
void draw_cursor(c_editor &ed);

// file handling
s_file open_file(const std::string &filename);
void save_to_file(const c_editor &ed, const std::string &filename);

// directory handling 
void    open_tree_view(c_editor &ed);

#endif