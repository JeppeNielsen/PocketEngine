//
//  FilePath.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "FilePath.hpp"

using namespace Pocket;

std::string FilePath::GetFilePath() {
    return path +"/"+ filename;
}

std::string FilePath::GetFolderPath() {
    return isFolder ? GetFilePath() : path;
}
