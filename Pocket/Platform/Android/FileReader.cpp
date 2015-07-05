//
//  FileReader.cpp
//  Android
//
//  Created by Jeppe Nielsen on 05/07/15.
//
//

#include "FileReader.hpp"
using namespace Pocket;

std::string FileReader::GetFile(std::string localFile) {
    return "Assets/" + localFile;
}

unsigned char* FileReader::GetData(std::string localFile, size_t* length) {
    return 0;
}

std::string FileReader::GetWritablePath() {
    return "";
}
std::string FileReader::GetExecutablePath() {
    return "";
}
std::string FileReader::GetBundlePath() {
    return "";
}
std::string FileReader::GetBundleDir() {
    return "";
}

std::string FileReader::ReadAllText(std::string filename) {
    return "";
}