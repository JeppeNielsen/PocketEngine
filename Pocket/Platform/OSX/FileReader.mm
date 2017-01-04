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
#include <AppKit/AppKit.h>
#include <stdio.h>
#include <vector>
#include <dirent.h>
#include <fstream>

using namespace Pocket;
using namespace std;

vector<string> FileReader::RunCommmand(const string &cmd) {
    FILE*           fp;
    const int       SIZEBUF = 1234;
    char            buf [SIZEBUF];
    vector<string> out;
    
    if ((fp = popen(cmd.c_str (), "r")) == NULL) {
        return out;
    }
    string cur_string = "";
    while (fgets(buf, sizeof (buf), fp)) {
        cur_string += buf;
    }
    out.push_back(cur_string.substr (0, cur_string.size () - 1));
    pclose(fp);
    return out;
}

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

std::vector<std::string> FileReader::ShowOpenFileRequester(const std::string &path, bool allowFiles, bool allowDirectories) {

    std::vector<std::string> files;

    NSOpenPanel* openDlg = [NSOpenPanel openPanel];

    // Enable the selection of files in the dialog.
    [openDlg setCanChooseFiles:allowFiles ? YES : NO];

    // Enable the selection of directories in the dialog.
    [openDlg setCanChooseDirectories: allowDirectories ? YES : NO];
    
    NSString* pathString = [[NSString alloc]initWithUTF8String:path.c_str()];

    // Display the dialog.  If the OK button was pressed,
    // process the files.
    if ( [openDlg runModalForDirectory:pathString file:nil] == NSModalResponseOK )
    {
        // Get an array containing the full filenames of all
        // files and directories selected.
        NSArray* filesSelected = [openDlg filenames];

        // Loop through all the files and process them.
        for( int i = 0; i < [filesSelected count]; i++ )
        {
            NSString* fileName = [filesSelected objectAtIndex:i];
            files.push_back([fileName UTF8String]);
        }
    }

    return files;
}

std::string FileReader::ShowSaveFileRequester(const std::string &path) {
    
    NSSavePanel* openDlg = [NSSavePanel savePanel];

    NSString* pathString = [[NSString alloc]initWithUTF8String:path.c_str()];

    // Display the dialog.  If the OK button was pressed,
    // process the files.
    if ( [openDlg runModalForDirectory:pathString file:nil] == NSModalResponseOK )
    {
        // Get an array containing the full filenames of all
        // files and directories selected.
        NSArray* filesSelected = [openDlg filenames];

        // Loop through all the files and process them.
        for( int i = 0; i < [filesSelected count]; i++ )
        {
            NSString* fileName = [filesSelected objectAtIndex:i];
            return std::string([fileName UTF8String]);
        }
    }

    return "";
}

void FileReader::OpenPathInFileExplorer(const std::string &path) {
    RunCommmand("open " + path);
}

void FileReader::FindFiles(std::vector<std::string> &list, const std::string &path, const std::string &extension) {
    FindFilesAtPath(list, path, extension);
}

void FileReader::FindFilesAtPath(std::vector<std::string> &list, const std::string &path, const std::string &extension) {
    
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
                FindFilesAtPath(list, path +"/"+ filename, extension);
            }
        } else {
            if (filename.find(extension)!=std::string::npos) {
                list.push_back(path+ "/"+ filename);
            }
        }
    }
    closedir(dp);
}

bool FileReader::EndsWith(const std::string& str, const std::string& endsWith) {
    if (str.length() >= endsWith.length()) {
        return (0 == str.compare (str.length() - endsWith.length(), endsWith.length(), endsWith));
    } else {
        return false;
    }
}

bool FileReader::FileExists(const std::string &path) {
    std::ifstream f(path.c_str());
    return f.good();
}

bool FileReader::DeleteFile(const std::string &path) {
    return remove( path.c_str() ) == 0;
}

std::string FileReader::GetFileNameFromPath(const std::string &path) {
    auto pos = path.rfind("/");
    if (pos == std::string::npos) {
        return path;
    } else {
        return path.substr(pos+1, path.size()-pos-1);
    }
}
