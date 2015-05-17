//
//  RenderSystem.hpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/21/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "TextureComponent.hpp"
#include "ShaderTexturedLighting.hpp"
#include "Colorable.hpp"
#include "VisibleObject.hpp"
#include <map>
#include "ShaderParticleSystem.hpp"
#include "ShaderTerrain.hpp"
#include "VertexRenderer.hpp"
#include "Clipper.hpp"

#define BATCH_RENDERING_ENABLED

namespace Pocket {
    class OctreeSystem;
    class CameraSystem;
    class TextureSystem;
    class ColorSystem;
    
    class RenderSystem : public GameSystem, IVisibleObjectManipulator {
    public:
        RenderSystem();
        ~RenderSystem();
        
        void Initialize();
        void Render();
        void AddedToWorld(GameWorld& world);
        
        OctreeSystem& Octree();
        void UpdateFromObject(GameObject *object);
        
    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
    private:
                
        void ApplyVisibleObject(VisibleObject* visibleObject);
        
        CameraSystem* cameraSystem;
        TextureSystem* textureSystem;
        ColorSystem* colorSystem;
        
        OctreeSystem* octree;
        
        void RenderCamera(GameObject* cameraObject);
        ObjectCollection renderList;
        typedef std::vector<VisibleObject*> VisibleObjects;
        VisibleObjects opaqueObjects;
        VisibleObjects transparentObjects;
        
        typedef std::map<VisibleObject::Type, Shader*> Shaders;
        
        Shaders shaders;
        
        ShaderParticleSystem particlesShader;
        ShaderTerrain terrainShader;
        
        static bool SortOpaqueObjects(const VisibleObject* a, const VisibleObject* b);
        static bool SortTransparentObjects(const VisibleObject* a, const VisibleObject* b);
        
        void RenderVisibleObjects(const BoundingFrustum& frustum, float* viewProjection, const VisibleObjects& visibleObjects, bool useTransparency);
        
        int renderedObjects;
        
        GameObject* currentCameraObject;
        Camera* currentCamera;
        
        VertexRenderer renderer;
        
        Clipper clipper;
        
    };
}
