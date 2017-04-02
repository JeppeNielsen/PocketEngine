//
//  FileReader.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/13/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#include <string>
#include <vector>
#include <functional>

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
        static void ShowMessageBox(const std::string& message);
        static std::string ShowMessageTextBox(const std::string& message, const std::string& text);
        static void ParseFile(const std::string& file, const std::function<void(const std::string& line)>& lineRead);
    };
}
