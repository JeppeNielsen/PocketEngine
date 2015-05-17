//
//  SpineSkeleton.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 08/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include <vector>
#include "Vector2.hpp"

using namespace Pocket;

namespace Pocket {
    Component(SpineSkeleton)
    public:
        void Reset();
        
        struct Bone {
            std::string name;
            std::string parent;
            Vector2 position;
            float rotation;
            Vector2 scale;
        };
        typedef std::vector<Bone> Bones;
        Bones bones;

        struct Slot {
            std::string name;
            std::string bone;
            std::string attachment;
        };

        typedef std::vector<Slot> Slots;
        Slots slots;

        struct Image {
            std::string name;
            Vector2 position;
            float rotation;
            Vector2 scale;
            Vector2 size;
        };

        typedef std::map<std::string, Image> Images;
        typedef std::map<std::string, Images> SkinImages;

        struct Skin {
            SkinImages images;
        };

        typedef std::map<std::string, Skin> Skins;
        Skins skins;
    };
}