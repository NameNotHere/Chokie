#include    <filesystem>
#include    "./input.h"

namespace fs = std::filesystem;

void    open_tree_view(c_editor &ed)
{
    // Open the file tree view
    // select which file with tab and enter
    // enter can also enter a directory and display that
    (void)ed;
    fs::path current_dir = fs::current_path();    
    for (const auto &entry : fs::directory_iterator(current_dir)) {
        if (fs::is_directory(entry)) {
            std::cout << "[DIR]  " << entry.path().filename() << "\n";
        } else if (fs::is_regular_file(entry)) {
            std::cout << "[FILE] " << entry.path().filename() << "\n";
        } else {
            std::cout << "[OTHER] " << entry.path().filename() << "\n";
        }
    }
}
