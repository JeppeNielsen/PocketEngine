//
//  RenderSystem.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Transform.hpp"
#include "MeshComponent.h"
#include "Material.h"
#include <iostream>
#include "CameraSystem.hpp"
#include "Camera.hpp"

using namespace Pocket;

struct VisibleObject {
    GameObject* object;
    Transform* transform;
    Material* material;
    IShader* shader;
    MeshComponent* mesh;
    int vertexType;
    float distanceToCamera;
};

struct RenderInfo {
    int drawCalls;
    int verticesRendered;
    int objectsRendered;
};

class IObjectRenderer {
public:
    virtual ~IObjectRenderer() {}
    virtual void Begin() = 0;
    virtual void End(RenderInfo& renderInfo) = 0;
    virtual void RenderObject(const VisibleObject& visibleObject) = 0;
    virtual void RenderTransparentObject(const VisibleObject& visibleObject) = 0;
    const float* viewProjection;
};

template<class Vertex>
class ObjectRenderer : public IObjectRenderer {
private:
    VertexRenderer<Vertex> renderer;
    Shader<Vertex>* currentShader;
    BlendMode currentBlendMode;
    int objectsRendered;
  
public:
    
    void Begin() override {
        currentShader = 0;
        renderer.BeginLoop();
        currentBlendMode = BlendMode::Opaque;
        objectsRendered = 0;
    }
    
    void End(RenderInfo& renderInfo) override {
        renderer.EndLoop();
        renderInfo.drawCalls += renderer.drawCalls;
        renderInfo.verticesRendered += renderer.verticesRendered;
        renderInfo.objectsRendered += objectsRendered;
    }
    
    void RenderObject(const VisibleObject& visibleObject) override {
        if (visibleObject.shader != currentShader) {
            renderer.Render();
            currentShader = static_cast<Shader<Vertex>*>(visibleObject.shader);
            currentShader->Use();
            currentShader->SetViewProjection(viewProjection);
        }
        
        const VertexMesh<Vertex>& mesh = visibleObject.mesh->ConstMesh<Vertex>();
        currentShader->RenderObject(renderer,
            mesh.vertices,
            mesh.triangles,
            visibleObject.object,
            *visibleObject.transform->World.GetValue()
            );
        objectsRendered++;
    }
    
    void RenderTransparentObject(const VisibleObject& visibleObject) override {
        if (visibleObject.material->BlendMode.GetValue()!=currentBlendMode) {
            currentBlendMode = visibleObject.material->BlendMode.GetValue();
            renderer.Render();
             if (currentBlendMode == BlendMode::Alpha ) {
                glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            } else {
                glBlendFunc (GL_SRC_ALPHA, GL_ONE);
            }
        }
        RenderObject(visibleObject);
    }
};

template<class Vertex>
IObjectRenderer* VertexType<Vertex>::CreateRenderer() {
    return new ObjectRenderer<Vertex>();
}

class RenderSystem : public GameSystem {
private:
    
    typedef std::vector<IObjectRenderer*> ObjectRenderers;
    ObjectRenderers objectRenderers;
    
    typedef std::vector<VisibleObject> VisibleObjects;
    VisibleObjects opaqueObjects;
    VisibleObjects transparentObjects;

    RenderInfo renderInfo;
    
    CameraSystem* cameras;
    
public:

    ~RenderSystem() {
        for (auto renderer : objectRenderers) {
            delete renderer;
        }
        objectRenderers.clear();
    }
    
    void Initialize() {
        for (auto& vertexType : IVertexType::typelist) {
            objectRenderers.push_back(vertexType->CreateRenderer());
        }
        AddComponent<Transform>();
        AddComponent<MeshComponent>();
        AddComponent<Material>();
    }
    
    void AddedToWorld(GameWorld& world) {
        cameras = world.CreateSystem<CameraSystem>();
    }
    
    void ObjectAdded(GameObject* object) {

    }
    
    void RenderCamera(GameObject* cameraObject) {
        Transform* cameraTransform = cameraObject->GetComponent<Transform>();
        Camera* camera = cameraObject->GetComponent<Camera>();
        
        Matrix4x4 viewProjection = camera->Projection.GetValue()->Multiply(*cameraTransform->WorldInverse.GetValue());
        const float* viewProjectionGL = viewProjection.GetGlMatrix();
        
        for (auto renderer : objectRenderers) {
            renderer->viewProjection = viewProjectionGL;
        }
        
        Vector3 distanceToCameraPosition;
        for(auto object : Objects()) {
            
            Material* material = object->GetComponent<Material>();
            if (!material->Shader()) continue; // must have shader
            MeshComponent* mesh = object->GetComponent<MeshComponent>();
            if (!mesh->vertexMesh) continue; // must have mesh
            
            Transform* transform = object->GetComponent<Transform>();
            
            const Matrix4x4& world = *transform->World.GetValue();
            distanceToCameraPosition.x = world[0][3];
            distanceToCameraPosition.y = world[1][3];
            distanceToCameraPosition.z = world[2][3];
            float fInvW = 1.0f / ( viewProjection[3][0] * distanceToCameraPosition.x + viewProjection[3][1] * distanceToCameraPosition.y + viewProjection[3][2] * distanceToCameraPosition.z + viewProjection[3][3] );
            float distanceToCamera = ( viewProjection[2][0] * distanceToCameraPosition.x + viewProjection[2][1] * distanceToCameraPosition.y + viewProjection[2][2] * distanceToCameraPosition.z + viewProjection[2][3] ) * fInvW;
            
            VisibleObjects& visibleObjects = material->BlendMode.GetValue() == BlendMode::Opaque ? opaqueObjects : transparentObjects;
            
            visibleObjects.push_back({
                object,
                transform,
                material,
                material->Shader(),
                mesh,
                mesh->VertexType(),
                distanceToCamera
            });
            
            //MeshComponent* mesh = object->GetComponent<MeshComponent>();
            //objectRenderers[mesh->VertexType()]->RenderObject(object, mesh);
        }
        
        
        glEnable(GL_DEPTH_TEST);
        
        if (!opaqueObjects.empty()) {
            std::sort(opaqueObjects.begin(), opaqueObjects.end(), SortOpaqueObjects);
            glDisable(GL_BLEND);
            glDepthMask(true);
            RenderVisibleObjects(opaqueObjects);
            opaqueObjects.clear();
        }
        
        if (!transparentObjects.empty()) {
            
            std::sort(transparentObjects.begin(), transparentObjects.end(), SortTransparentObjects);
            
            glEnable(GL_BLEND);
            glDepthMask(false);
            //clipper.UseDepth = false;
            
            RenderTransparentVisibleObjects(transparentObjects);
            glDepthMask(true);
            transparentObjects.clear();
        }
        
        /*
        if (!opaqueObjects.empty()) {
            
            std::sort(opaqueObjects.begin(), opaqueObjects.end(), SortOpaqueObjects);
            
            glDisable(GL_BLEND);
            glDepthMask(true);
            clipper.UseDepth = true;
        
            RenderVisibleObjects(frustum, viewProjectionGL, opaqueObjects, false);
            
            opaqueObjects.clear();
        }
        */
        
        
    }
    
    void RenderVisibleObjects(const VisibleObjects& visibleObjects) {
        int currentVertexType = visibleObjects[0].vertexType;
        objectRenderers[currentVertexType]->Begin();
        objectRenderers[currentVertexType]->RenderObject(visibleObjects[0]);
        for (size_t i=1; i<visibleObjects.size(); ++i) {
            const VisibleObject& visibleObject = visibleObjects[i];
            if (currentVertexType!=visibleObject.vertexType) {
                objectRenderers[currentVertexType]->End(renderInfo);
                currentVertexType = visibleObject.vertexType;
                objectRenderers[currentVertexType]->Begin();
            }
            objectRenderers[currentVertexType]->RenderObject(visibleObject);
        }
        objectRenderers[currentVertexType]->End(renderInfo);
    }
    
    void RenderTransparentVisibleObjects(const VisibleObjects& visibleObjects) {
        int currentVertexType = visibleObjects[0].vertexType;
        objectRenderers[currentVertexType]->Begin();
        objectRenderers[currentVertexType]->RenderTransparentObject(visibleObjects[0]);
        for (size_t i=1; i<visibleObjects.size(); ++i) {
            const VisibleObject& visibleObject = visibleObjects[i];
            if (currentVertexType!=visibleObject.vertexType) {
                objectRenderers[currentVertexType]->End(renderInfo);
                currentVertexType = visibleObject.vertexType;
                objectRenderers[currentVertexType]->Begin();
            }
            objectRenderers[currentVertexType]->RenderTransparentObject(visibleObject);
        }
        objectRenderers[currentVertexType]->End(renderInfo);
    }
    
    void SetBlendMode(BlendMode blendMode) {
       
    }
    
    void Render() {
        renderInfo.drawCalls = 0;
        renderInfo.verticesRendered = 0;
        renderInfo.objectsRendered = 0;
        for(auto camera : cameras->Objects()) {
            RenderCamera(camera);
        }
        
        static int counter = 0;
        if (counter++%20==0) {
            std::cout<<"Draw calls : " << renderInfo.drawCalls<< ", Vertices Rendered : " << renderInfo.verticesRendered<<", Objects rendered : "<< renderInfo.objectsRendered<<std::endl;
        }
    }
    
private:
    static bool SortOpaqueObjects(const VisibleObject& a, const VisibleObject& b) {
        if (a.vertexType != b.vertexType) {
            return a.vertexType<b.vertexType;
        }
    
        if (a.shader == b.shader) {
            return a.distanceToCamera<b.distanceToCamera;
        } else {
            return ((size_t)a.shader)<((size_t)b.shader);
        }
    }
    
    static bool SortTransparentObjects(const VisibleObject& a, const VisibleObject& b) {
        return a.distanceToCamera>b.distanceToCamera;
    }
    
};