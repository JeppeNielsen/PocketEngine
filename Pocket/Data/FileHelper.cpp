//
//  FileHelper.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "FileHelper.hpp"

#include <stdio.h>
#include <vector>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>

using namespace Pocket;

void FileHelper::RecurseFolder(const std::string& path, const std::function<void(const std::string&)>& onFile, const std::string& extension) {
    RecurseFolderInternal(path, onFile, extension);
}

void FileHelper::RecurseFolderInternal(const std::string& path, const std::function<void(const std::string&)>& onFile, const std::string& extension) {
    
    struct dirent *entry;
    DIR *dp;

    dp = opendir(path.c_str());
    if (dp == NULL) {
        return;
    }

    while ((entry = readdir(dp))) {
        std::string filename(entry->d_name);
        if (entry->d_type == DT_DIR ) {
            if (filename!="." && filename!="..") {
                RecurseFolderInternal(path +"/"+ filename, onFile, extension);
            }
        } else {
            if (extension == "" || filename.find(extension)!=std::string::npos) {
                onFile(path+ "/"+ filename);
            }
        }
    }
    closedir(dp);
}


bool FileHelper::FileExists(const std::string &path) {
    std::ifstream f(path.c_str());
    return f.good();
}

bool FileHelper::DeleteFile(const std::string &path) {
    return remove( path.c_str() ) == 0;
}

bool FileHelper::CreateFolder(const std::string &path) {
    return mkdir(path.c_str(), 0755) == 0;
}

std::string FileHelper::GetFileNameFromPath(const std::string &path) {
    auto pos = path.rfind("/");
    if (pos == std::string::npos) {
        return path;
    } else {
        return path.substr(pos+1, path.size()-pos-1);
    }
}

std::string FileHelper::GetFolderPath(const std::string &path) {
    auto pos = path.rfind("/");
    if (pos == std::string::npos) {
        return path;
    } else {
        return path.substr(0, pos);
    }
}
