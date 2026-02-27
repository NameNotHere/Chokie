#include    "./input.h"

namespace fs = std::filesystem;

s_window	create_window(int xpos, int ypos, std::string file, bool is_special)
{
	s_window	win;
	win.otvoren_file.filename = file;
	win.window_x = xpos;
	win.window_y = ypos;
	win.is_special = is_special;
	win.otvoren_file = open_file(file);

	return (win);
}