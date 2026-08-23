#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <string>//for text
#include <vector>//the scanner finds multple files so we use vector

using namespace std;

class FileScanner {
public:
    vector<string> scanFolder(const string& folderPath);
    /*the const means the function shouldn't modify the original path.
    the & means we pass the string by reference*/
};

#endif