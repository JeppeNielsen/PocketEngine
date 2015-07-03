//
//  RenderSystem.cpp
//  Shaders
//
//  Created by Jeppe Nielsen on 03/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "RenderSystem.hpp"

using namespace Pocket;

RenderSystem::~RenderSystem() {
    for (auto renderer : objectRenderers) {
        delete renderer;
    }
    objectRenderers.clear();
}

void RenderSystem::Initialize() {
    for (auto& vertexType : IVertexType::typelist) {
        objectRenderers.push_back(vertexType->CreateRenderer());
    }
    AddComponent<Transform>();
    AddComponent<Mesh>();
    AddComponent<Material>();
}

void RenderSystem::AddedToWorld(GameWorld& world) {
    cameras = world.CreateSystem<CameraSystem>();
    meshOctreeSystem = world.CreateSystem<OctreeSystem>();
    meshOctreeSystem->AddComponent<Material>();
}

OctreeSystem& RenderSystem::Octree() {
    return *meshOctreeSystem;
}

void RenderSystem::RenderCamera(GameObject* cameraObject) {
    Transform* cameraTransform = cameraObject->GetComponent<Transform>();
    Camera* camera = cameraObject->GetComponent<Camera>();
    RenderMask cameraMask = camera->Mask;
    
    Matrix4x4 viewProjection = camera->Projection.GetValue()->Multiply(*cameraTransform->WorldInverse.GetValue());
    const float* viewProjectionGL = viewProjection.GetGlMatrix();
    
    BoundingFrustum frustum;
    frustum.SetFromViewProjection(viewProjection);
    meshOctreeSystem->GetObjectsInFrustum(frustum, objectsInFrustum);

    if (objectsInFrustum.empty()) return;
    
    if (!camera->Orthographic()) {
        Box viewport = camera->Viewport;
        glViewport(viewport.left, viewport.bottom, viewport.Size().x, viewport.Size().y);
    }
    
    for (auto renderer : objectRenderers) {
        renderer->viewProjection = viewProjectionGL;
    }
    
    Vector3 distanceToCameraPosition;
    for(auto object : objectsInFrustum) {
        
        Material* material = object->GetComponent<Material>();
        if (!material->Shader()) continue; // must have shader
        RenderMask mask = material->Mask;
        if (!((cameraMask & mask) == mask)) { // must be matching camera mask
            continue;
        }
        
        Mesh* mesh = object->GetComponent<Mesh>();
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
            object->GetComponent<TextureComponent>(),
            object->GetComponent<Orderable>(),
            distanceToCamera
        });
    }
    objectsInFrustum.clear();
    
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
}

void RenderSystem::RenderVisibleObjects(const VisibleObjects& visibleObjects) {
    int currentVertexType = visibleObjects[0].vertexType;
    objectRenderers[currentVertexType]->Begin(false);
    objectRenderers[currentVertexType]->RenderObject(visibleObjects[0]);
    for (size_t i=1; i<visibleObjects.size(); ++i) {
        const VisibleObject& visibleObject = visibleObjects[i];
        if (currentVertexType!=visibleObject.vertexType) {
            objectRenderers[currentVertexType]->End(renderInfo);
            currentVertexType = visibleObject.vertexType;
            objectRenderers[currentVertexType]->Begin(false);
        }
        objectRenderers[currentVertexType]->RenderObject(visibleObject);
    }
    objectRenderers[currentVertexType]->End(renderInfo);
}

void RenderSystem::RenderTransparentVisibleObjects(const VisibleObjects& visibleObjects) {
    int currentVertexType = visibleObjects[0].vertexType;
    objectRenderers[currentVertexType]->Begin(true);
    objectRenderers[currentVertexType]->RenderTransparentObject(visibleObjects[0]);
    for (size_t i=1; i<visibleObjects.size(); ++i) {
        const VisibleObject& visibleObject = visibleObjects[i];
        if (currentVertexType!=visibleObject.vertexType) {
            objectRenderers[currentVertexType]->End(renderInfo);
            currentVertexType = visibleObject.vertexType;
            objectRenderers[currentVertexType]->Begin(true);
        }
        objectRenderers[currentVertexType]->RenderTransparentObject(visibleObject);
    }
    objectRenderers[currentVertexType]->End(renderInfo);
}

void RenderSystem::Render() {
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

bool RenderSystem::SortOpaqueObjects(const VisibleObject& a, const VisibleObject& b) {

    if (a.orderable && b.orderable) {
        int orderA = a.orderable->Order.Get();
        int orderB = b.orderable->Order.Get();
        if (orderA!=orderB) {
            return orderA<orderB;
        }
    }

    if (a.vertexType != b.vertexType) {
        return a.vertexType<b.vertexType;
    }

    if (a.shader == b.shader) {
        if (a.texture==b.texture) {
            return a.distanceToCamera<b.distanceToCamera;
        } else {
            return a.texture<b.texture;
        }
    } else {
        return ((size_t)a.shader)<((size_t)b.shader);
    }
}

bool RenderSystem::SortTransparentObjects(const VisibleObject& a, const VisibleObject& b) {
    if (a.orderable && b.orderable) {
        int orderA = a.orderable->Order.Get();
        int orderB = b.orderable->Order.Get();
        if (orderA!=orderB) {
            return orderA<orderB;
        }
    }

    return a.distanceToCamera>b.distanceToCamera;
}

