#include <iostream>
#include <string>
#include <vector>

#include "FileScanner.h"
#include "FileClassifier.h"
#include "FolderManager.h"
#include "Logger.h"

using namespace std;

int main(){
    FileScanner scanner;
    FileClassifier classifier;
    FolderManager manager;
    Logger logger;

    int choice;

    while (true){
        cout << "\n====================================\n";
        cout << "     AI Folder Management System\n";
        cout << "====================================\n";

        cout << "1. Scan Folder\n";
        cout << "2. Preview Organization\n";
        cout << "3. Organize Files\n";
        cout << "4. View Organization Log\n";
        cout << "5. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        // OPTION 1: Scan Folder
        if (choice == 1){
            string folderPath;

            cout << "\nEnter folder path: ";
            cin.ignore();
            getline(cin, folderPath);

            vector<string> files = scanner.scanFolder(folderPath);

            if (files.empty()){
                cout << "\nNothing to scan.\n";
                continue;
            }

            cout << "\nFiles found:\n\n";

            for (int i = 0; i < files.size(); i++){
                string category = classifier.classifyFile(files[i]);

                cout << i + 1 << ". "
                     << files[i]
                     << " -> "
                     << category
                     << endl;
            }
        }

        // OPTION 2: Preview Organization
        else if (choice == 2){
            string folderPath;

            cout << "\nEnter folder path: ";
            cin.ignore();
            getline(cin, folderPath);

            vector<string> files = scanner.scanFolder(folderPath);

            if (files.empty()){
                cout << "\nNothing to preview.\n";
                continue;
            }

            cout << "\nOrganization Preview:\n\n";

            for (const string &file : files){
                string category = classifier.classifyFile(file);

                cout << file
                     << " -> "
                     << category
                     << endl;
            }
        }

        // OPTION 3: Organize Files
        else if (choice == 3){
            string folderPath;

            cout << "\nEnter folder path: ";
            cin.ignore();
            getline(cin, folderPath);

            vector<string> files = scanner.scanFolder(folderPath);

            if (files.empty()){
                cout << "\nNothing to organize.\n";
                continue;
            }

            cout << "\nOrganization Preview:\n\n";

            for (const string &file : files){
                string category = classifier.classifyFile(file);

                cout << file
                     << " -> "
                     << category
                     << endl;
            }

            char confirm;

            cout << "\nDo you want to organize these files? (y/n): ";
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y'){
                cout << "\nOrganizing files...\n\n";

                for (const string &file : files)
                {
                    string category = classifier.classifyFile(file);

                    manager.organizeFile(file, category);
                }

                cout << "\nFile organization completed.\n";
            }
            else{
                cout << "\nOrganization cancelled.\n";
            }
        }

        // OPTION 4: View Log
        else if (choice == 4){
            logger.viewLog();
        }

        // OPTION 5: Exit
        else if (choice == 5){
            cout << "\nExiting AI Folder Management System...\n";
            break;
        }

        // INVALID OPTION
        else{
            cout << "\nInvalid choice. Please try again.\n";
        }
    }

    return 0;
}
