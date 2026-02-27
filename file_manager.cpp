#include    "./input.h"
#include    "iostream"
#include	<bits/stdc++.h>
namespace fs = std::filesystem;

s_window	create_window(int xpos, int ypos, std::string file, bool is_special)
{
	s_window	win;
	win.otvoren_file.FILEPATH = file;
	win.window_x = xpos;
	win.window_y = ypos;
	win.is_special = is_special;
	win.otvoren_file = open_file(file);

	int j = 0;
	for (j = file.size() - 1; j >= 0; j--)
		if (file[j] == '/' || file[j] == '\\')
			break;
	while (file[j] > 0)
	{
		win.otvoren_file.filename = file[j] + win.otvoren_file.filename;
		j++;
	}
	std::cout << "filename: " << win.otvoren_file.filename << std::endl;
	return (win);
}