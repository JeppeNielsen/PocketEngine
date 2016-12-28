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
        static std::vector<std::string> RunCommmand(const std::string& cmd);
        static std::string GetFile(std::string localFile);
        static std::string GetWritablePath();
        static std::string GetExecutablePath();
        static std::string GetBundlePath();
        static std::string GetBundleDir();
        
        static std::vector<std::string> ShowOpenFileRequester(const std::string& path, bool allowFiles, bool allowDirectories);
        static std::string ShowSaveFileRequester(const std::string& path);
        
        static void OpenPathInFileExplorer(const std::string& path);
        static void FindFiles(std::vector<std::string>& list, const std::string& path, const std::string& extension);
        static bool EndsWith(const std::string& str, const std::string& endsWith);
        static bool FileExists(const std::string& path);
        static bool DeleteFile(const std::string& path);
    private:
        static void FindFilesAtPath(std::vector<std::string>& list, const std::string& path, const std::string& extension);
    };
}