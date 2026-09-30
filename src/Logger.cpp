#include "Logger.h"
#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

void Logger::logAction(const string& fileName, const string& category){
    ofstream logFile("organization_log.txt", ios::app);

    if (!logFile){
        cout << "Error: Could not open log file." << endl;
        return;
    }

    time_t currentTime = time(nullptr);

    logFile << ctime(&currentTime)
            << "File: " << fileName
            << " -> Category: " << category
            << "\n-----------------------------\n";

    logFile.close();
}


// View organization log
void Logger::viewLog(){
    ifstream logFile("organization_log.txt");

    if (!logFile){
        cout << "\nNo organization log found.\n";
        return;
    }

    string line;

    cout << "\n====================================\n";
    cout << "       Organization Log\n";
    cout << "====================================\n\n";

    while (getline(logFile, line)){
        cout << line << endl;
    }

    logFile.close();
}