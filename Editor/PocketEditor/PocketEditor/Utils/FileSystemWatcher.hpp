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

class FileSystemWatcher {

public:
    void Start(const std::string& pathToWatch);
    void Stop();

    Pocket::Event<> Changed;
private:
    void* streamPtr;
};