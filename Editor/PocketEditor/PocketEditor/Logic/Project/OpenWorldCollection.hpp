//
//  OpenWorldCollection.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <string>
#include "OpenWorld.hpp"
#include "Property.hpp"

using namespace Pocket;

class OpenWorldCollection {
public:
    OpenWorld* LoadWorld(const std::string& path, const std::string& filename);
    void CloseWorld(OpenWorld* world);
    bool TryFindOpenWorld(const std::string& path, OpenWorld** world);
    
    OpenWorldCollection();
    ~OpenWorldCollection();
    
    Property<OpenWorld*> ActiveWorld;
    
    void Clear();
    
    void Initialize(InputManager& input);
    
    Event<OpenWorld*> WorldLoaded;
    Event<OpenWorld*> WorldClosed;
    
private:
    using Worlds = std::vector<OpenWorld*>;
    Worlds worlds;
    InputManager* input;
};