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
#include "DeferredBuffers.hpp"
#include "DeferredShader.hpp"
#include "Light.hpp"
#include "DeferredLight.hpp"

#define BATCH_RENDERING_ENABLED

namespace Pocket {
    class OctreeSystem;
    class LightingSystem;
    class ColorSystem;
    class CameraSystem;
    class TextureSystem;
    
    class DeferredRenderSystem : public GameSystem, IVisibleObjectManipulator {
    public:
        DeferredRenderSystem();
        ~DeferredRenderSystem();
        
        void Initialize();
        void Render();
        void AddedToWorld(GameWorld& world);
        
        OctreeSystem& Octree();
        
        void UpdateFromObject(GameObject* object);
        
    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        
    private:
                
        class LightMeshSystem : public GameSystem {
            public:
                void Initialize();
                void ObjectAdded(GameObject* object);
                void ObjectRemoved(GameObject* object);
                void Update(float dt);
            private:
                void LightChanged(Light* light, GameObject* object);
                void UpdateMesh(GameObject* object);
                typedef std::set<GameObject*> ChangedLights;
                ChangedLights changedLights;
        };
                
        void ApplyVisibleObject(VisibleObject* visibleObject);
        
        CameraSystem* cameraSystem;
        TextureSystem* textureSystem;
        ColorSystem* colorSystem;
        LightMeshSystem* lightMeshSystem;
        
        OctreeSystem* octree;
        OctreeSystem* lightOctree;
        
        void RenderCamera(GameObject* cameraObject);
        ObjectCollection renderList;
        typedef std::vector<VisibleObject*> VisibleObjects;
        VisibleObjects opaqueObjects;
        VisibleObjects transparentObjects;
        
        DeferredShader deferredShader;
        DeferredLight defferedLight;
        
        ShaderParticleSystem particlesShader;
        ShaderTerrain terrainShader;
        
        static bool SortOpaqueObjects(const VisibleObject* a, const VisibleObject* b);
        static bool SortTransparentObjects(const VisibleObject* a, const VisibleObject* b);
        
        void RenderVisibleObjects(const BoundingFrustum& frustum, const float* viewProjection, const VisibleObjects& visibleObjects, bool useTransparency);
        
        int renderedObjects;
        
        GameObject* currentCameraObject;
        Camera* currentCamera;
        
        VertexRenderer renderer;
        
        Clipper clipper;
        DeferredBuffers deferredBuffers;
        
    };
}
