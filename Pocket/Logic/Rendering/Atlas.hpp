//
//  Atlas.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/8/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Box.hpp"
#include "TypeInfo.hpp"
#include <map>

namespace Pocket {
    struct Atlas {
    
        Atlas();
    
    struct Node {
        Box outer;
        Box inner;
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(outer)
        TYPE_FIELD(inner)
        TYPE_FIELDS_END
    };
    
    bool Load(std::string textureFile, const Vector2& textureSize);
    
    bool LoadSpineAtlas(std::string filename);
    
    const Node& GetNode(std::string name);
    
    const Vector2& TextureSize() const;
    
public:
        typedef std::map<std::string, Node> Nodes;
        Nodes nodes;
    
        Vector2 String2Vector(std::string s);
    
        Node defaultNode;
        Vector2 textureSize;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(nodes)
        TYPE_FIELD(defaultNode)
        TYPE_FIELD(textureSize)
        TYPE_FIELDS_END
    };
}