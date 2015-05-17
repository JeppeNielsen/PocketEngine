//
//  Atlas.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameComponent.hpp"
#include "Box.hpp"
#include <map>

namespace Pocket {
    
    Component(Atlas)
    
public:
    
    struct Node {
        Box outer;
        Box inner;
    };
    
    Atlas();
    
    bool Load(std::string textureFile, const Vector2& textureSize);
    
    bool LoadSpineAtlas(std::string filename);
    
    //void Set(std::string name, const Box& textureCoords, const Box& offset);
    
    const Node& GetNode(std::string name);
    
    void Reset();
    void Clone(const Atlas& other);
    
    const Vector2& TextureSize() const;
    
private:
        typedef std::map<std::string, Node> Nodes;
        Nodes nodes;
    
        Vector2 String2Vector(std::string s);
    
        Node defaultNode;
        Vector2 textureSize;
        
    };
}