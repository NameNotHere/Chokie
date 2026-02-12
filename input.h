#ifndef INPUT_H
#define INPUT_H

#	include	<vector>
#	include	<string>
#	include	"raylib.h"

enum e_mode
{
	NORMAL,
	INSERT,
	COMMAND,
	MENU
};

struct  s_editor
{
    std::vector<std::string>	lines;
    int							cursor_row;
    int							cursor_col;
	int							window_scroll;
    bool						insert_mode;
	
};

// Input functions
void	handle_insert_mode(s_editor &ed);
void	handle_normal_mode(s_editor &ed);
void	keyboard_input(s_editor &ed);
void	draw_text(s_editor &ed);
void	remove_last_char(s_editor &ed);

// drawing functions
void	draw_text(s_editor &ed);
void	draw_cursor(s_editor &ed);

// file handling
std::vector<std::string> read_file(const std::string &filename);
#endif
