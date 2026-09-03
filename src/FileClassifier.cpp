#include "FileClassifier.h"
#include <filesystem>

using namespace std;
namespace fs = filesystem;

string FileClassifier::classifyFile(const string& filePath) {

    string extension = fs::path(filePath).extension().string();

    if (extension == ".jpg" || extension == ".jpeg" ||
        extension == ".png" || extension == ".gif") {
        return "Images";
    }

    if (extension == ".pdf" || extension == ".doc" ||
    extension == ".docx" || extension == ".txt" ||
    extension == ".ppt" || extension == ".pptx" ||
    extension == ".xls" || extension == ".xlsx") {
    return "Documents";
}

    if (extension == ".cpp" || extension == ".h" ||
        extension == ".c" || extension == ".py" ||
        extension == ".java") {
        return "Code";
    }

    if (extension == ".mp4" || extension == ".mkv" ||
        extension == ".avi") {
        return "Videos";
    }

    if (extension == ".mp3" || extension == ".wav") {
        return "Audio";
    }

    return "Others";
}