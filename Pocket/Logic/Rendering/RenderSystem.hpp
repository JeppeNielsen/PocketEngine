//
//  RenderSystem.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Camera.hpp"
#include "OctreeSystem.hpp"
#include "ObjectRenderer.hpp"
#include "ShaderCollection.hpp"

namespace Pocket {

class RenderSystem : public GameSystem<Transform, Mesh, Material> {
public:
    using OctreeSystem = OctreeSystem<Material>;
    
    struct CameraSystem : GameSystem<Transform, Camera> { };
    
    struct TextureSystem : GameSystem<TextureComponent, Orderable> {};
    
    using Systems = meta::list<CameraSystem, OctreeSystem, TextureSystem>;
    
    using ObjectRenderers = std::vector<IObjectRenderer*>;
    using VisibleObjects = std::vector<VisibleObject>;

    virtual ~RenderSystem();
    void Initialize(IGameWorld* world);
    void ObjectAdded(GameObject *object);
    OctreeSystem& Octree();
    void RenderCamera(GameObject* cameraObject);
    void RenderVisibleObjects(const VisibleObjects& visibleObjects);
    void RenderTransparentVisibleObjects(const VisibleObjects& visibleObjects);
    void Render();
    static bool SortOpaqueObjects(const VisibleObject& a, const VisibleObject& b);
    static bool SortTransparentObjects(const VisibleObject& a, const VisibleObject& b);
    
    ShaderCollection Shaders;
    IShader* DefaultShader;
    IShader* DefaultTexturedShader;
    
private:
    
    ObjectRenderers objectRenderers;
    
    VisibleObjects opaqueObjects;
    VisibleObjects transparentObjects;

    RenderInfo renderInfo;
    typename RenderSystem::ObjectCollection objectsInFrustum;

    CameraSystem* cameras;
    OctreeSystem* meshOctreeSystem;
};

}