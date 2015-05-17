//
//  SpineModel.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 01/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"
#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include "DirtyProperty.hpp"
#include <map>
#include <string>
#include "Transform.hpp"
#include "Sprite.hpp"
#include "Atlas.hpp"
#include "Sizeable.hpp"
#include "SpineSkeleton.hpp"
#include "Pointer.hpp"

namespace Pocket {
    Component(SpineModel)

        struct Bone {
            Pointer<GameObject> object;
            Pointer<Transform> transform;
            Vector2 position;
            float rotation;
            Vector2 scale;
        };

        typedef std::map<std::string, Bone> Bones;
        Bones bones;

        struct Slot {
            Pointer<GameObject> object;
            Pointer<Transform> transform;
            Pointer<Sprite> sprite;
            Pointer<Sizeable> sizeable;
            std::string bone;
            std::string attachment;
        };

        typedef std::map<std::string, Slot> Slots;
        Slots slots;

        void SetImage(std::string slotName, std::string imageName, SpineSkeleton& skeleton);
    };
}