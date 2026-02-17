

void    handle_command_mode(c_editor &ed)
{
    std::string command;
    char key = GetKeyPressed();

    while (!IsKeyPressed(KEY_ENTER))
    {
        while (key > 0)
        {
            command += (char)key;
            key = GetKeyPressed();
        }
    }
    // Finalize command mode
}
