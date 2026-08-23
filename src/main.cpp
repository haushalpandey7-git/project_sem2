/*   cmake -S . -B build -G "MinGW Makefiles"
     cmake --build build
     .\build\AIFolderManager.exe
*/
#include <iostream>
#include <string>
#include <vector>
#include "FileScanner.h"

using namespace std;

int main()
{
    cout << "AI Folder Management System\n";
    cout << "1. Scan Folder\n";
    cout << "2. Organize Files\n";
    cout << "3. Exit\n";

    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1){
        string folderPath;

        cout << "Enter folder path: ";
        cin.ignore();
        getline(cin, folderPath);

        FileScanner scanner;

        vector<string> files = scanner.scanFolder(folderPath);

        cout << "\nFiles found:\n";

        for (int i = 0; i < files.size(); i++){
            cout << i + 1 << ". " << files[i] << endl;
        }
    }
    else if (choice == 2){
        cout << "File organization selected.\n";
    }
    else if (choice == 3){
        cout << "Exiting...\n";
    }
    else{
        cout << "Invalid choice.\n";
    }

    return 0;
}