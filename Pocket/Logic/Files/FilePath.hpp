//
//  FilePath.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>

namespace Pocket {
    struct FilePath {
        std::string path;
        std::string filename;
        bool isFolder;
        std::string GetFilePath();
        std::string GetFolderPath();
    };
}

