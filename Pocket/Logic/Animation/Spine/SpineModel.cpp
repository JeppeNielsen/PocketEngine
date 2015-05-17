//
//  SpineModel.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 01/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SpineModel.hpp"
#include "minijson.h"
#include "FileReader.hpp"
#include <sstream>
#include <istream>
#include "StringHelper.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "MathHelper.hpp"

using namespace Pocket;

void SpineModel::SetImage(std::string slotName, std::string imageName, SpineSkeleton& skeleton) {
    Slots::iterator slotIterator = slots.find(slotName);
    if (slotIterator==slots.end()) return;
    
    Slot& slot = slotIterator->second;
    SpineSkeleton::Skin& defaultSkin = skeleton.skins.begin()->second;
    SpineSkeleton::Image& image = defaultSkin.images[slotName][imageName];
    //if (slot.sprite->SpriteName() == image.name) return;
    
    slot.transform->Position = image.position;
    slot.transform->Rotation = image.rotation;
    slot.transform->Scale = Vector3(image.scale.x, image.scale.y, 1.0f);
    slot.sizeable->Size = image.size;
    slot.transform->Anchor = image.size * 0.5f;
    
    slot.sprite->SpriteName = image.name;
}

