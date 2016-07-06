//
//  MapMeshSystem.h
//  Homeland
//
//  Created by Jeppe Nielsen on 15/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Map.h"
#include "MapRenderer.h"
#include "PropertyListener.hpp"

using namespace Pocket;

struct MapMeshSystem : public GameSystem<Map, MapRenderer, Mesh, Transform> {
	void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
private:
	PropertyListener<GameObject*> changedTransforms;
};