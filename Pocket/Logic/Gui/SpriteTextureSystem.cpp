//
//  SpriteTextureSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "SpriteTextureSystem.hpp"
#include "Atlas.hpp"

using namespace Pocket;

void SpriteTextureSystem::Initialize() {
    AddComponent<Sprite>();
    AddComponent<Atlas>();
    AddComponent<Mesh>();
}

void SpriteTextureSystem::ObjectAdded(Pocket::GameObject *object) {
    Sprite* sprite = object->GetComponent<Sprite>();
    sprite->SpriteName.Changed += event_handler(this, &SpriteTextureSystem::SpriteTextureChanged, object);
    sprite->CornerSize.Changed += event_handler(this, &SpriteTextureSystem::SpriteTextureChanged, object);
    
    SpriteTextureChanged(sprite, object);
}

void SpriteTextureSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Sprite>()->SpriteName.Changed -= event_handler(this, &SpriteTextureSystem::SpriteTextureChanged,object);
    object->GetComponent<Sprite>()->CornerSize.Changed -= event_handler(this, &SpriteTextureSystem::SpriteTextureChanged,object);
}

void SpriteTextureSystem::SpriteTextureChanged(Sprite* sprite, GameObject* object) {
    
    Atlas* atlas = object->GetComponent<Atlas>();
    Mesh* mesh = object->GetComponent<Mesh>();
    
    Mesh::VerticesList& vertices = mesh->Vertices();
    Vector2 cornerSize = sprite->CornerSize.GetValue();
    
    const Atlas::Node& atlasNode = atlas->GetNode(sprite->SpriteName);
    
    bool normal = cornerSize.x<=0 && cornerSize.y<=0;
    
    if (normal) {
        if (atlasNode.inner != atlasNode.outer) {
            cornerSize = atlasNode.outer.Size() - atlasNode.inner.Size();
            cornerSize.y = -cornerSize.y;
            cornerSize *= atlas->TextureSize() * 0.5f;
            sprite->CornerSize = cornerSize;
            normal = false;
        }
    }
    
    int vertCount = normal ? 4 : 16;
    
    if (vertices.size()!=vertCount) {
        vertices.resize(vertCount, Vertex(0));
    }
    
    if (normal) {
            
        vertices[0].TextureCoords = Vector2(atlasNode.outer.left,atlasNode.outer.bottom);
        vertices[1].TextureCoords = Vector2(atlasNode.outer.right,atlasNode.outer.bottom);
        vertices[2].TextureCoords = Vector2(atlasNode.outer.right,atlasNode.outer.top);
        vertices[3].TextureCoords = Vector2(atlasNode.outer.left,atlasNode.outer.top);
    } else {
        
        vertices[0].TextureCoords = Vector2(atlasNode.outer.left, atlasNode.outer.bottom);
        vertices[1].TextureCoords = Vector2(atlasNode.inner.left, atlasNode.outer.bottom);
        vertices[2].TextureCoords = Vector2(atlasNode.inner.right, atlasNode.outer.bottom);
        vertices[3].TextureCoords = Vector2(atlasNode.outer.right, atlasNode.outer.bottom);
        
        vertices[4].TextureCoords = Vector2(atlasNode.outer.left, atlasNode.inner.bottom);
        vertices[5].TextureCoords = Vector2(atlasNode.inner.left, atlasNode.inner.bottom);
        vertices[6].TextureCoords = Vector2(atlasNode.inner.right, atlasNode.inner.bottom);
        vertices[7].TextureCoords = Vector2(atlasNode.outer.right, atlasNode.inner.bottom);
        
        vertices[ 8].TextureCoords = Vector2(atlasNode.outer.left, atlasNode.inner.top);
        vertices[ 9].TextureCoords = Vector2(atlasNode.inner.left, atlasNode.inner.top);
        vertices[10].TextureCoords = Vector2(atlasNode.inner.right, atlasNode.inner.top);
        vertices[11].TextureCoords = Vector2(atlasNode.outer.right, atlasNode.inner.top);
        
        vertices[12].TextureCoords = Vector2(atlasNode.outer.left, atlasNode.outer.top);
        vertices[13].TextureCoords = Vector2(atlasNode.inner.left, atlasNode.outer.top);
        vertices[14].TextureCoords = Vector2(atlasNode.inner.right, atlasNode.outer.top);
        vertices[15].TextureCoords = Vector2(atlasNode.outer.right, atlasNode.outer.top);
    }
    
}


