#ifndef FILE_CLASSIFIER_H
#define FILE_CLASSIFIER_H

#include <string>

#include "AIClassifier.h"

using namespace std;

class FileClassifier{
private:
    AIClassifier ai;

public:
    string classifyFile(const string& filePath);
};

#endif