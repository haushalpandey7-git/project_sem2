#ifndef LOGGER_H
#define LOGGER_H

#include <string>

using namespace std;

class Logger{
public:
    void logAction(const string& fileName, const string& category);
    void viewLog();
};

#endif