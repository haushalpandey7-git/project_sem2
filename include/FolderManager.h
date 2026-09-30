#ifndef FOLDER_MANAGER_H
#define FOLDER_MANAGER_H

#include <string>

using namespace std;

class FolderManager{
public:
    bool organizeFile(const string& filePath, const string& category);
};

#endif