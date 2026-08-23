#include "FileScanner.h"
#include <filesystem>

using namespace std;
namespace fs = filesystem;

vector<string> FileScanner::scanFolder(const string& folderPath)
{
    vector<string> files;

    for (auto entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            files.push_back(entry.path().string());
        }
    }

    return files;
}