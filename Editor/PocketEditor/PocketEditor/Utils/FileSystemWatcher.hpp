//
//  FileSystemWatcher.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Event.hpp"
#include <string>

namespace Pocket {
    class FileSystemWatcher {

    public:
        void Start(const std::string& pathToWatch);
        void Stop();

        Event<> Changed;
        Event<std::string> FileModified;
        const std::string& Path();
    private:
        std::string path;
        void* streamPtr;
    };
}
