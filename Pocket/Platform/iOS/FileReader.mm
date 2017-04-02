//
//  FileReader.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/13/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "FileReader.hpp"
#import <Foundation/Foundation.h>
#include <map>
#include "StringHelper.hpp"
#include <istream>

using namespace Pocket;

std::string FileReader::GetFile(std::string localFile) {
    NSString* file = [NSString stringWithUTF8String:localFile.c_str()];
    
    NSString* path = [[NSBundle mainBundle] pathForResource:file
                                                     ofType:nil];
    if (!path) return "";
    return [path UTF8String];
}
std::string FileReader::GetWritablePath() {
    NSArray *docs = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documents = [docs objectAtIndex:0];
    return [documents UTF8String];
}

std::string FileReader::GetExecutablePath() {
    NSString * exePath = [[NSBundle mainBundle] executablePath];
    return [exePath UTF8String];
}

std::string FileReader::GetBundlePath() {
    NSString * exePath = [[NSBundle mainBundle] bundlePath];
    return [exePath UTF8String];
}

std::string FileReader::GetBundleDir() {
    std::string bundlePath = GetBundlePath();
    std::vector<std::string> dirs;
    StringHelper::split(bundlePath, '/', dirs);
    std::string path = "";
    for (unsigned i=0; i<dirs.size()-1; ++i) {
        path += dirs[i] + "/";
    }
    return path;
}

void FileReader::ParseFile(const std::string& file, const std::function<void(const std::string& line)>& lineRead) {
    std::ifstream file;
    std::string line;
    if (file.is_open())
    {
        while ( std::getline (file,line) ) {
            lineRead(line);
        }
        file.close();
    }
}

