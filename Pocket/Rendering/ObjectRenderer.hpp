//
//  ObjectRenderer.h
//  Shaders
//
//  Created by Jeppe Nielsen on 03/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "TextureComponent.hpp"
#include "Orderable.hpp"
#include "Clipper.hpp"

namespace Pocket {

struct VisibleObject {
    Transform* transform;
    Material* material;
    IShader* shader;
    Mesh* mesh;
    int vertexType;
    TextureComponent* texture;
    Orderable* orderable;
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
    virtual void Begin(bool isTransparent) = 0;
    virtual void End(RenderInfo& renderInfo) = 0;
    virtual void RenderObject(const VisibleObject& visibleObject) = 0;
    virtual void RenderTransparentObject(const VisibleObject& visibleObject) = 0;
    const float* viewProjection;
};

template<class V>
class ObjectRenderer : public IObjectRenderer {
private:
    VertexRenderer<V> renderer;
    Shader<V>* currentShader;
    BlendModeType currentBlendMode;
    TextureComponent* currentTexture;
    Clipper clipper;
    int objectsRendered;
    
public:
    
    void Begin(bool isTransparent) override {
        currentShader = 0;
        renderer.BeginLoop();
        currentBlendMode = BlendModeType::Opaque;
        objectsRendered = 0;
        clipper.UseDepth = !isTransparent;
        currentTexture = (TextureComponent*)-1;
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
            currentShader = static_cast<Shader<V>*>(visibleObject.shader);
            currentShader->Use();
            currentShader->SetViewProjection(viewProjection);
        }
        
        if (visibleObject.texture!=currentTexture) {
            renderer.Render();
            currentTexture = visibleObject.texture;
            glBindTexture(GL_TEXTURE_2D, currentTexture ? currentTexture->Texture().GetHandle() : 0);
        }
        
        int clip = visibleObject.material->Clip;
        bool isClipping = clip != 0;
        
        if (isClipping) {
            renderer.Render();
            if (clip==1) {
                clipper.PushBegin();
            } else {
                clipper.PopBegin();
            }
        }
        
        const VertexMesh<V>& mesh = visibleObject.mesh->ConstMesh<V>();
        currentShader->RenderObject(renderer,
            mesh.vertices,
            mesh.triangles,
            visibleObject.transform->World
            );
        objectsRendered++;
        
        if (isClipping) {
            renderer.Render();
            if (clip==1) {
                clipper.PushEnd();
            } else {
                clipper.PopEnd();
            }
        }
    }
    
    void RenderTransparentObject(const VisibleObject& visibleObject) override {
        if (visibleObject.material->BlendMode()!=currentBlendMode) {
            currentBlendMode = visibleObject.material->BlendMode();
            renderer.Render();
             if (currentBlendMode == BlendModeType::Alpha ) {
                glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            } else {
                glBlendFunc (GL_SRC_ALPHA, GL_ONE);
            }
        }
        RenderObject(visibleObject);
    }
};

}