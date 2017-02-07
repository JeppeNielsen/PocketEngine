//
//  FileHelper.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <functional>

namespace Pocket {
    class FileHelper {
    public:
        static bool FileExists(const std::string& path);
        static bool DeleteFile(const std::string& path);
        static std::string GetFileNameFromPath(const std::string& path);
        static void RecurseFolder(const std::string& path, const std::function<void(const std::string&)>& onFile, const std::string& extension = "");
    private:
        static void RecurseFolderInternal(const std::string& path, const std::function<void(const std::string&)>& onFile, const std::string& extension);
    };
}
