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
#include "Property.hpp"

namespace Pocket {
    struct FileSystemListener {
        Property<std::string> Path;
        Property<std::string> Extension;
        
        float latency;
        FileSystemWatcher watcher;
    };
}
