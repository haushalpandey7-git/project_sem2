#ifndef FILECLASSIFIER_H
#define FILECLASSIFIER_H

#include <string>

using namespace std;

class FileClassifier {
public:
    string classifyFile(const string& filePath);
};

#endif