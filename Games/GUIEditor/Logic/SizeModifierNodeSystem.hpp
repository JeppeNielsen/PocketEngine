//
//  SizeModifierNodeSystem.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/22/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "SizeModifierNode.hpp"
#include "Draggable.hpp"
#include "PropertyListener.hpp"

using namespace Pocket;

SYSTEM(SizeModifierNodeSystem, SizeModifierNode, Draggable, Transform)
public:
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
    void Update(float dt);
    
private:
    PropertyListener<GameObject*> draggingObjects;
};
