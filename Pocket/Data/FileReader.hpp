//
//  FileReader.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/13/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#include <string>
#include <vector>

namespace Pocket {
    class FileReader {
      
    public:
        static std::string GetFile(std::string localFile);
        static std::string GetWritablePath();
        static std::string GetExecutablePath();
        static std::string GetBundlePath();
        static std::string GetBundleDir();
        
        static std::vector<std::string> ShowOpenFileRequester(const std::string& path, bool allowFiles, bool allowDirectories);
        static std::string ShowSaveFileRequester(const std::string& path);
        
    };
}