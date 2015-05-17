//
//  XmlSerializer.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "tinyxml.h"
#include <string>

namespace Pocket {
    
    class Vector2;
    class Vector3;
    class Matrix4x4;
    class Quaternion;
    class Box;
    class XmlSerializer {
    public:
        XmlSerializer();
        ~XmlSerializer();
        
        void Serialize(TiXmlDocument* xmlFile);
        
    protected:
        
        //virtual void OnSerialize();
        //void Write(std::string name, Vector2& value);
        
        
    };
}