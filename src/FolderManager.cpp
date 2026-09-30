#include "FolderManager.h"
#include "Logger.h"

#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = filesystem;

bool FolderManager::organizeFile(const string& filePath, const string& category){
    try{
        fs::path file = filePath;

        // Create category folder
        fs::path destinationFolder = file.parent_path() / category;

        if (!fs::exists(destinationFolder)){
            fs::create_directory(destinationFolder);
        }

        // Initial destination
        fs::path destination = destinationFolder / file.filename();

        // Handle duplicate filenames
        if (fs::exists(destination)){
            string stem = file.stem().string();
            string extension = file.extension().string();

            int counter = 1;

            while (fs::exists(destination)){
                string newFileName =
                    stem + " (" + to_string(counter) + ")" + extension;

                destination = destinationFolder / newFileName;

                counter++;
            }
        }

        // Move the file
        fs::rename(file, destination);

        cout << file.filename().string()
             << " -> "
             << category;

        if (destination.filename() != file.filename()){
            cout << " as "
                 << destination.filename().string();
        }

        cout << endl;

        // Log successful organization
        Logger logger;

        logger.logAction(
            file.filename().string(),
            category
        );

        return true;
    }
    catch (const fs::filesystem_error& e){
        cout << "Error moving file: "
             << e.what()
             << endl;

        return false;
    }
}