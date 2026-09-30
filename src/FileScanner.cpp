#include "FileScanner.h"
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = filesystem;

vector<string> FileScanner::scanFolder(const string& folderPath){
    vector<string> files;

    try{
        if (!fs::exists(folderPath)){
            cout << "Error: Folder does not exist." << endl;
            return files;
        }

        if (!fs::is_directory(folderPath)){
            cout << "Error: The given path is not a folder." << endl;
            return files;
        }

        for (auto entry : fs::directory_iterator(folderPath)){
            if (entry.is_regular_file())
            {
                files.push_back(entry.path().string());
            }
        }

        if (files.empty()){
            cout << "No files found in this folder." << endl;
        }
    }
    catch (const fs::filesystem_error& e){
        cout << "Error scanning folder: "
             << e.what()
             << endl;
    }

    return files;
}