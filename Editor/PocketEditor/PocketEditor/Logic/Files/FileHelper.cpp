//
//  FileHelper.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 20/07/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "FileHelper.hpp"
#include <dirent.h>

void FileHelper::FindFiles(std::vector<std::string> &list, const std::string &path, const std::string &extension) {
    FindFilesAtPath(list, path, extension);
}

void FileHelper::FindFilesAtPath(std::vector<std::string> &list, const std::string &path, const std::string &extension) {
    
    struct dirent *entry;
    DIR *dp;

    dp = opendir(path.c_str());
    if (dp == NULL) {
        return;
    }

    while ((entry = readdir(dp))) {
        //puts(entry->d_name);
        
        std::string filename(entry->d_name);
        
        if (entry->d_type == DT_DIR ) {
            if (filename!="." && filename!="..") {
                FindFilesAtPath(list, path + filename, extension);
            }
        } else {
            if (filename.find(extension)!=std::string::npos) {
                list.push_back(path+ "/"+ filename);
            }
        }
    }
    closedir(dp);
}

