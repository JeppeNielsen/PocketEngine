//
//  FilePath.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "FilePath.hpp"

std::string FilePath::GetFilePath() {
    return path +"/"+ filename;
}