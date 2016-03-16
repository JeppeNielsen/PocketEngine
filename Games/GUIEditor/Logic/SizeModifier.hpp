//
//  SizeModifier.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/22/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"

using namespace Pocket;

struct SizeModifier {
    SizeModifier();
    GameObject* Nodes[8];
    GameObject* Lines[4];
    void DeleteNodes();
};
