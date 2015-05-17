//
//  SpineFactory.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 03/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "SpriteMeshSystem.hpp"
#include "SpriteTextureSystem.hpp"
#include "SpineModel.hpp"
#include "Atlas.hpp"
#include "Material.hpp"
#include "TextureComponent.hpp"
#include "SpineAnimation.hpp"
#include "SpineAnimator.hpp"
#include "SpineSkeleton.hpp"

namespace minijson {
    class value;
}

namespace Pocket {
    class SpineFactory : public GameFactory {
    public:
        void Initialize();
        
        GameObject* CreateModel(GameObject* atlas, GameObject* skeleton);
        GameObject* LoadSkeleton(std::string filename);
        bool LoadAnimations(std::string filename, SpineAnimationCollection& animations);
        
        static float Scaler;
        
    private:

        void ParseJson(minijson::value& value, SpineSkeleton& skeleton);

        void ParseBones(minijson::value& bonesArray, SpineSkeleton& skeleton);
        void ParseBone(minijson::value& value, SpineSkeleton& skeleton);

        void ParseSlots(minijson::value& slotsArray, SpineSkeleton& skeleton);
        void ParseSlot(minijson::value& value, SpineSkeleton& skeleton);
        
        void ParseSkins(minijson::value& bonesArray, SpineSkeleton& skeleton);
        void ParseSkin(std::string name, minijson::value& value, SpineSkeleton& skeleton);
        void ParseImage(std::string name, SpineSkeleton::Skin& skin, minijson::value& value, SpineSkeleton& skeleton);
        
        void ParseAnimations(minijson::value& value, SpineAnimationCollection& animations);
        void ParseAnimation(minijson::value& value, std::string name, SpineAnimationCollection& animations);
        void ParseAnimationSlots(minijson::value& value, SpineAnimation& animation);
        void ParseAnimationBones(minijson::value& value, SpineAnimation& animation);
    };
}

