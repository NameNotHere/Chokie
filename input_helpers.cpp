#include "input.h"
#include <algorithm>
#include <cstring>

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
