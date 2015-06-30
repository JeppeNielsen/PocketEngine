//
//  RenderSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/21/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "OctreeSystem.hpp"
#include "Material.hpp"
#include "ShaderUnlitUncolored.hpp"
#include "ShaderUnlitUncoloredTextured.hpp"
#include "ParticleEmitter.hpp"
#include "CameraSystem.hpp"
#include "TextureSystem.hpp"
#include "ColorSystem.hpp"

using namespace Pocket;

RenderSystem::RenderSystem() { }

RenderSystem::~RenderSystem() {
    delete shaders[VisibleObject::Unlit];
    delete shaders[VisibleObject::UnlitTextured];
    shaders.clear();
}

OctreeSystem& RenderSystem::Octree() {
    return *octree;
}

void RenderSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Mesh>();
    AddComponent<Material>();
}

void RenderSystem::AddedToWorld(Pocket::GameWorld &world) {
    octree = world.CreateSystem<OctreeSystem>();
    octree->AddComponent<Material>();
    
    
    cameraSystem = world.CreateOrGetSystem<CameraSystem>();
    
    textureSystem = world.CreateSystem<TextureSystem>();
    textureSystem->manipulator = this;
    
    colorSystem = world.CreateOrGetSystem<ColorSystem>();
    
    shaders[VisibleObject::Unlit] = new ShaderUnlitUncolored();
    shaders[VisibleObject::UnlitTextured] = new ShaderUnlitUncoloredTextured();
    shaders[VisibleObject::Lit] = new ShaderTexturedLighting();
    shaders[VisibleObject::LitTextured] = new ShaderTexturedLighting();
    
    for (Shaders::iterator it=shaders.begin(); it!=shaders.end(); ++it) {
        it->second->Initialize();
    }
    
    particlesShader.Initialize();
    terrainShader.Initialize();
}

void RenderSystem::ObjectAdded(GameObject* object) {
    VisibleObject* visibleObject = new VisibleObject(object);
    SetMetaData(object, visibleObject);
    ApplyVisibleObject(visibleObject);
}

void RenderSystem::ObjectRemoved(GameObject* object) {
    VisibleObject* visibleObject = (VisibleObject*)GetMetaData(object);
    SetMetaData(object, 0);
    delete visibleObject;
}

void RenderSystem::Render() {
    const ObjectCollection& cameras = cameraSystem->Objects();
    for (ObjectCollection::const_iterator it = cameras.begin(); it!=cameras.end(); ++it) {
        RenderCamera(*it);
    }
}

bool RenderSystem::SortOpaqueObjects(const VisibleObject* a, const VisibleObject* b) {
    if (a->texture==b->texture) {
        if (!a->orderable || !b->orderable) {
            return a->distanceToCamera<b->distanceToCamera;
        } else {
            int orderA = a->orderable->Order.Get();
            int orderB = b->orderable->Order.Get();
            if (orderA==orderB) {
                return a->distanceToCamera<b->distanceToCamera;
            } else {
                return orderA<orderB;
            }
        }
   } else {
        if (!a->orderable || !b->orderable) {
            return a->texture<b->texture;
        } else {
            return a->orderable->Order.Get()<b->orderable->Order.Get();
        }
    }
}

bool RenderSystem::SortTransparentObjects(const VisibleObject* a, const VisibleObject* b) {
    if (a->texture==b->texture) {
        if (!a->orderable || !b->orderable) {
            return a->distanceToCamera>b->distanceToCamera;
        } else {
            int orderA = a->orderable->Order.Get();
            int orderB = b->orderable->Order.Get();
            if (orderA==orderB) {
                return a->distanceToCamera>b->distanceToCamera;
            } else {
                return orderA<orderB;
            }
        }
    } else {
        if (!a->orderable || !b->orderable) {
            return a->texture<b->texture;
        } else {
            return a->orderable->Order.Get()<b->orderable->Order.Get();
        }
    }
}

void RenderSystem::RenderCamera(GameObject* cameraObject) {
    Transform* transform = cameraObject->GetComponent<Transform>();
    currentCameraObject = cameraObject;
    currentCamera = cameraObject->GetComponent<Camera>();
    if (!currentCamera) return;
    
    RenderMask cameraMask = currentCamera->Mask;
    
    particlesShader.Aspect = currentCamera->Viewport.GetValue().Aspect();
    
    Matrix4x4 viewProjection = currentCamera->Projection.GetValue()->Multiply(*transform->WorldInverse.GetValue());
	const float* viewProjectionGL = viewProjection.GetGlMatrix();
    
	BoundingFrustum frustum;
	frustum.SetFromViewProjection(viewProjection);
	octree->GetObjectsInFrustum(frustum, renderList);
    
    if (renderList.empty()) return;
    
    renderedObjects = 0;
   	BoundingFrustum::Count = 0;
    renderer.Init();
    
    Vector3 position;
    for(unsigned i=0; i<renderList.size(); i++) {
        GameObject* object = renderList[i];
        VisibleObject* visibleObject = (VisibleObject*)GetMetaData(object);
        RenderMask mask = visibleObject->material->Mask;
        if (!((cameraMask & mask) == mask)) {
            continue;
        }
        
        if (visibleObject->material->BlendMode.GetValue() == BlendMode::None) {
            opaqueObjects.push_back(visibleObject);
        } else {
            
            const Matrix4x4& world = *visibleObject->transform->World.GetValue();
            position.x = world[0][3];
            position.y = world[1][3];
            position.z = world[2][3];
            float fInvW = 1.0f / ( viewProjection[3][0] * position.x + viewProjection[3][1] * position.y + viewProjection[3][2] * position.z + viewProjection[3][3] );
            visibleObject->distanceToCamera = ( viewProjection[2][0] * position.x + viewProjection[2][1] * position.y + viewProjection[2][2] * position.z + viewProjection[2][3] ) * fInvW;
            
            transparentObjects.push_back(visibleObject);
        }
    }
    renderList.clear();
    
    if (!currentCamera->Orthographic()) {
        Box viewport = currentCamera->Viewport;
        glViewport(viewport.left, viewport.bottom, viewport.Size().x, viewport.Size().y);
    }
    glEnable(GL_DEPTH_TEST);
    
    if (!opaqueObjects.empty()) {
        
        std::sort(opaqueObjects.begin(), opaqueObjects.end(), SortOpaqueObjects);
        
        glDisable(GL_BLEND);
        glDepthMask(true);
        clipper.UseDepth = true;
    
        RenderVisibleObjects(frustum, viewProjectionGL, opaqueObjects, false);
        
        opaqueObjects.clear();
    }
    
    if (!transparentObjects.empty()) {
        
        std::sort(transparentObjects.begin(), transparentObjects.end(), SortTransparentObjects);
        
        glEnable(GL_BLEND);
        glDepthMask(false);
        clipper.UseDepth = false;
        
        RenderVisibleObjects(frustum, viewProjectionGL, transparentObjects, true);
        
        glDepthMask(true);
        
        transparentObjects.clear();
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    /*
    static int test = 0;
    if (++test % 120 == 0) {
        std::cout
		<<"Rendered objects :   "<< renderedObjects
		<<" : "<<",   draw calls : " <<renderer.drawCalls
		<<" : "<<",   vertices : " << renderer.verticesRendered
        <<" : "<<",   frustum checks : " <<BoundingFrustum::Count
        <<std::endl;
    }
    */
}

void RenderSystem::RenderVisibleObjects(const BoundingFrustum& frustum, const float* viewProjection, const VisibleObjects &visibleObjects, bool useTransparency) {

    renderer.shader = 0;
    Shader* shader;
    
    Texture* oldTexture = 0;
    Texture* texture;
    
    BlendMode::Mode blendMode;
    
    if (useTransparency) {
        blendMode = visibleObjects[0]->material->BlendMode.GetValue();
        if (blendMode == BlendMode::Alpha ) {
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        } else {
            glBlendFunc (GL_SRC_ALPHA, GL_ONE);
        }
    }
    
    for(unsigned i=0; i<visibleObjects.size(); i++) {
        VisibleObject* visibleObject = visibleObjects[i];
        
		shader = visibleObject->shader;
        texture = visibleObject->texture ? &visibleObject->texture->Texture() : 0;
        
        int clip = visibleObject->material->Clip;
        
        bool isClipping = clip != 0;
        
        if (isClipping) {
            if (clip==1) {
                renderer.Render();
                clipper.PushBegin();
            } else {
                renderer.Render();
                clipper.PopBegin();
            }
        }
        
        if (shader!=renderer.shader) {
            renderer.Render();
            renderer.shader = shader;
            shader->cameraObject = currentCameraObject;
            shader->camera = currentCamera;
            shader->frustum = &frustum;
            shader->viewProjection = viewProjection;
        }
                      
        if (useTransparency) {
            if (blendMode != visibleObject->material->BlendMode.GetValue()) {
                blendMode = visibleObject->material->BlendMode.GetValue();
                
                renderer.Render();
                
                if (blendMode == BlendMode::Alpha ) {
                    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                } else {
                    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
                }
            }
        }
                
        if (texture!=oldTexture || i==0) {
            renderer.Render();
            shader->BindTexture(texture);
            oldTexture = texture;
        }
        
        Mesh* mesh = visibleObject->mesh;
        
        renderer.visibleObject = visibleObject;
        shader->RenderVertices(visibleObject, mesh, renderer);
        
        if (isClipping) {
            if (clip==1) {
                renderer.Render();
                clipper.PushEnd();
            } else {
                renderer.Render();
                clipper.PopEnd();
            }
        }
        
        renderedObjects++;
    }
    renderer.Render();

#ifndef DISABLE_MAP_BUFFER
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
#endif
}

void RenderSystem::ApplyVisibleObject(VisibleObject* visibleObject) {
    if (!visibleObject) return;
    visibleObject->Refresh();
    visibleObject->shader = shaders[visibleObject->type];
    if (visibleObject->object->GetComponent<ParticleEmitter>()) {
        visibleObject->shader = &particlesShader;
    } else if (visibleObject->object->GetComponent<Terrain>()) {
        visibleObject->shader = &terrainShader;
    }
}

void RenderSystem::UpdateFromObject(Pocket::GameObject *object) {
    VisibleObject* visibleObject = (VisibleObject*)GetMetaData(object);
    ApplyVisibleObject(visibleObject);
}
