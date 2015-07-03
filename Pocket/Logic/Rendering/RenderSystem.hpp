//
//  RenderSystem.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include <iostream>
#include "CameraSystem.hpp"
#include "Camera.hpp"
#include "OctreeSystem.hpp"
#include "ObjectRenderer.hpp"

namespace Pocket {

class RenderSystem : public GameSystem {
public:
    typedef std::vector<IObjectRenderer*> ObjectRenderers;
    typedef std::vector<VisibleObject> VisibleObjects;

    ~RenderSystem();
    void Initialize();
    void AddedToWorld(GameWorld& world);
    OctreeSystem& Octree();
    void RenderCamera(GameObject* cameraObject);
    void RenderVisibleObjects(const VisibleObjects& visibleObjects);
    void RenderTransparentVisibleObjects(const VisibleObjects& visibleObjects);
    void Render();
private:
    static bool SortOpaqueObjects(const VisibleObject& a, const VisibleObject& b);
    static bool SortTransparentObjects(const VisibleObject& a, const VisibleObject& b);
    
    ObjectRenderers objectRenderers;
    
    VisibleObjects opaqueObjects;
    VisibleObjects transparentObjects;

    RenderInfo renderInfo;
    
    CameraSystem* cameras;
    OctreeSystem* meshOctreeSystem;
    
    ObjectCollection objectsInFrustum;
};

}