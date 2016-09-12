//
//  FileSystemListener.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include "FileSystemWatcher.hpp"

struct FileSystemListener {
    std::string path;
    float latency;
    FileSystemWatcher watcher;
};