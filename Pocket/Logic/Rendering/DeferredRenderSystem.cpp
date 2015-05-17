//
//  DeferredRenderSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/21/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"
#include "DeferredRenderSystem.hpp"
#include "OctreeSystem.hpp"
#include "Material.hpp"
#include "ShaderUnlitUncolored.hpp"
#include "ShaderUnlitUncoloredTextured.hpp"
#include "ParticleEmitter.hpp"
#include "ColorSystem.hpp"
#include "CameraSystem.hpp"
#include "TextureSystem.hpp"

using namespace Pocket;

DeferredRenderSystem::DeferredRenderSystem() { }

DeferredRenderSystem::~DeferredRenderSystem() {
}

OctreeSystem& DeferredRenderSystem::Octree() {
    return *octree;
}

void DeferredRenderSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Mesh>();
    AddComponent<Material>();
}

void DeferredRenderSystem::AddedToWorld(Pocket::GameWorld &world) {
    octree = world.CreateSystem<OctreeSystem>();
    octree->AddComponent<Material>();
    
    lightOctree = world.CreateSystem<OctreeSystem>();
    lightOctree->AddComponent<Light>();
    
    cameraSystem = world.CreateOrGetSystem<CameraSystem>();
    
    textureSystem = world.CreateSystem<TextureSystem>();
    textureSystem->manipulator = this;
    
    colorSystem = world.CreateOrGetSystem<ColorSystem>();
    
    lightMeshSystem = world.CreateSystem<LightMeshSystem>();
        
    deferredShader.Initialize();
    defferedLight.Initialize();
    particlesShader.Initialize();
    terrainShader.Initialize();
    deferredBuffers.Initialize();
}

void DeferredRenderSystem::ObjectAdded(GameObject* object) {
    VisibleObject* visibleObject = new VisibleObject(object);
    SetMetaData(object, visibleObject);
    ApplyVisibleObject(visibleObject);
}

void DeferredRenderSystem::ObjectRemoved(GameObject* object) {
    VisibleObject* visibleObject = (VisibleObject*)GetMetaData(object);
    SetMetaData(object, 0);
    delete visibleObject;
}

void DeferredRenderSystem::Render() {
    const ObjectCollection& cameras = cameraSystem->Objects();
    for (ObjectCollection::const_iterator it = cameras.begin(); it!=cameras.end(); ++it) {
        RenderCamera(*it);
    }
}

bool DeferredRenderSystem::SortOpaqueObjects(const VisibleObject* a, const VisibleObject* b) {
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

bool DeferredRenderSystem::SortTransparentObjects(const VisibleObject* a, const VisibleObject* b) {
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

void DeferredRenderSystem::RenderCamera(GameObject* cameraObject) {
    Transform* transform = cameraObject->GetComponent<Transform>();
    currentCameraObject = cameraObject;
    currentCamera = cameraObject->GetComponent<Camera>();
    if (!currentCamera) return;
    
    RenderMask cameraMask = currentCamera->Mask;
    
    particlesShader.Aspect = currentCamera->Viewport.GetValue().Aspect();
    
    Matrix4x4 inverseView = *transform->WorldInverse.GetValue();
    Matrix4x4 viewProjection = currentCamera->Projection.GetValue()->Multiply(inverseView);
    Matrix4x4 inverseProjection = currentCamera->Projection.GetValue()->Invert();
    Matrix4x4 worldView =  *transform->World.GetValue();
    
	float* viewProjectionGL = viewProjection.GetGlMatrix();
    float* inverseProjectionGL = inverseProjection.GetGlMatrix();
    
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
        if (!visibleObject) continue; // TODO: check why this is needed
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
        ASSERT_GL(glViewport(viewport.left, viewport.bottom, viewport.Size().x, viewport.Size().y));
    }
    ASSERT_GL(glEnable(GL_DEPTH_TEST));
    
    if (!opaqueObjects.empty()) {
        
        std::sort(opaqueObjects.begin(), opaqueObjects.end(), SortOpaqueObjects);
        
        ASSERT_GL(glDisable(GL_BLEND));
        ASSERT_GL(glDepthMask(true));
        clipper.UseDepth = true;
        
        Vector2 viewportSize = currentCamera->Viewport.GetValue().Size();
        deferredBuffers.Resize((int)viewportSize.x, (int)viewportSize.y);
        deferredBuffers.BeginGeometry();
        deferredShader.viewMatrix = inverseView.GetGlMatrix();
        
        RenderVisibleObjects(frustum, viewProjectionGL, opaqueObjects, false);
        
        //deferredBuffers.SaveBuffer("Test.png");
        
        deferredBuffers.BeginLights();
        
        
        
        
        {
            
            renderer.Init();
            //renderer.shader = &defferedLight;
            
            defferedLight.cameraObject = cameraObject;
            defferedLight.camera = currentCamera;
            defferedLight.frustum = &frustum;
            defferedLight.viewProjection = viewProjectionGL;
            defferedLight.InitBatching(0);
            defferedLight.SetInverseProjection(inverseProjectionGL, worldView.GetGlMatrix());
            defferedLight.SetViewPort(currentCamera->Viewport.GetValue().Size());
            
            lightOctree->GetObjectsInFrustum(frustum, renderList);
            
            
            for (size_t i=0; i<renderList.size(); i++) {
                GameObject* object = renderList[i];
                VisibleObject visibleObject(object);
                renderer.visibleObject = &visibleObject;
                
                Light* light = object->GetComponent<Light>();
                Vector3 lightPosition = visibleObject.transform->World.GetValue()->TransformPosition(0);
                //lightPosition = inverseView.TransformPosition(lightPosition);
                
                defferedLight.SetLightProperties(lightPosition, Colour::White(), light->Radius, light->Brightness);
                defferedLight.RenderVertices(&visibleObject, visibleObject.mesh, renderer);
                renderer.Render();
            }
        }
        
        deferredBuffers.End();
        opaqueObjects.clear();
    }
    
    if (!transparentObjects.empty()) {
        
        std::sort(transparentObjects.begin(), transparentObjects.end(), SortTransparentObjects);
        
        ASSERT_GL(glEnable(GL_BLEND));
        ASSERT_GL(glDepthMask(false));
        clipper.UseDepth = false;
        
        RenderVisibleObjects(frustum, viewProjectionGL, transparentObjects, true);
        
        ASSERT_GL(glDepthMask(true));
        
        transparentObjects.clear();
    }
    
    ASSERT_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    ASSERT_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    
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

void DeferredRenderSystem::RenderVisibleObjects(const BoundingFrustum& frustum, float* viewProjection, const VisibleObjects &visibleObjects, bool useTransparency) {

    renderer.shader = 0;
    Shader* shader;
    
    Texture* oldTexture = 0;
    Texture* texture;
    
    BlendMode::Mode blendMode;
    
    if (useTransparency) {
        blendMode = visibleObjects[0]->material->BlendMode.GetValue();
        if (blendMode == BlendMode::Alpha ) {
            ASSERT_GL(glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        } else {
           ASSERT_GL( glBlendFunc (GL_SRC_ALPHA, GL_ONE));
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
                    ASSERT_GL(glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
                } else {
                   ASSERT_GL( glBlendFunc (GL_SRC_ALPHA, GL_ONE));
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
    ASSERT_GL(glUnmapBuffer(GL_ARRAY_BUFFER));
    ASSERT_GL(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
#endif
}

void DeferredRenderSystem::ApplyVisibleObject(VisibleObject* visibleObject) {
    if (!visibleObject) return;
    visibleObject->Refresh();
    visibleObject->shader = &deferredShader;
    if (visibleObject->object->GetComponent<ParticleEmitter>()) {
        visibleObject->shader = &particlesShader;
    } else if (visibleObject->object->GetComponent<Terrain>()) {
        visibleObject->shader = &terrainShader;
    }
}

void DeferredRenderSystem::UpdateFromObject(Pocket::GameObject *object) {
    VisibleObject* visibleObject = (VisibleObject*)GetMetaData(object);
    ApplyVisibleObject(visibleObject);
}

void DeferredRenderSystem::LightMeshSystem::Initialize() {
    AddComponent<Mesh>();
    AddComponent<Light>();
}

void DeferredRenderSystem::LightMeshSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Light>()->Type.Changed += event_handler(this, &DeferredRenderSystem::LightMeshSystem::LightChanged, object);
    object->GetComponent<Light>()->Radius.Changed += event_handler(this, &DeferredRenderSystem::LightMeshSystem::LightChanged, object);
    LightChanged(object->GetComponent<Light>(), object);
}

void DeferredRenderSystem::LightMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Light>()->Type.Changed -= event_handler(this, &DeferredRenderSystem::LightMeshSystem::LightChanged, object);
    object->GetComponent<Light>()->Radius.Changed -= event_handler(this, &DeferredRenderSystem::LightMeshSystem::LightChanged, object);
}

void DeferredRenderSystem::LightMeshSystem::LightChanged(Pocket::Light *light, Pocket::GameObject *object) {
    changedLights.insert(object);
}

void DeferredRenderSystem::LightMeshSystem::Update(float dt) {
    if (changedLights.empty()) return;
    for (ChangedLights::iterator it = changedLights.begin(); it!=changedLights.end(); ++it) {
        UpdateMesh(*it);
    }
}

void DeferredRenderSystem::LightMeshSystem::UpdateMesh(Pocket::GameObject *object) {
    Mesh* mesh = object->GetComponent<Mesh>();
    Light* light = object->GetComponent<Light>();

    mesh->Clear();
    
    if (light->Type == Light::LightType::Point) {
        mesh->AddGeoSphere(0, light->Radius()+1, 2);
        mesh->Flip();
    }
}

