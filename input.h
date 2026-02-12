#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <string>
#include "raylib.h"

// Editor struct
struct Editor {
    std::vector<std::string> lines;
    int cursor_row;
    int cursor_col;
    bool insert_mode;
};

// Input functions
void handle_insert_mode(Editor &ed);
void handle_normal_mode(Editor &ed);
void keyboard_input(Editor &ed);
void draw_text(Editor &ed);
void remove_last_char(Editor &ed);

#endif // INPUT_H
