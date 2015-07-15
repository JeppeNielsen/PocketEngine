//
//  SpineFactory.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 03/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "SpineFactory.hpp"
#include "minijson.h"
#include "FileReader.hpp"
#include "StringHelper.hpp"
#include "TransformHierarchy.hpp"
#include "MathHelper.hpp"
#include "SpineAnimatorSystem.hpp"
#include "File.hpp"

using namespace Pocket;

float SpineFactory::Scaler = 0.01f;

void SpineFactory::Initialize() {
    CreateSystem<SpriteMeshSystem>();
    CreateSystem<SpriteTextureSystem>();
    CreateSystem<TransformHierarchy>();
    CreateSystem<SpineAnimatorSystem>();
}

GameObject* SpineFactory::CreateModel(Pocket::GameObject *atlas, GameObject* skeletonObject) {
    GameObject* object = World()->CreateObject();
    object->AddComponent<Atlas>(atlas);
    object->AddComponent<TextureComponent>(atlas);
    SpineModel* model = object->AddComponent<SpineModel>();
    SpineSkeleton* skeleton = object->AddComponent<SpineSkeleton>(skeletonObject);
    object->AddComponent<Transform>();
    object->AddComponent<SpineAnimator>();
    
    for (int i=0; i<skeleton->bones.size(); ++i) {
        SpineSkeleton::Bone& skeletonBone = skeleton->bones[i];
        SpineModel::Bone& modelBone = model->bones[skeletonBone.name];
        modelBone.position = skeletonBone.position;
        modelBone.rotation = skeletonBone.rotation;
        modelBone.scale = skeletonBone.scale;
        modelBone.object = World()->CreateObject();
        modelBone.transform = modelBone.object->AddComponent<Transform>();
        modelBone.transform->Position = skeletonBone.position;
        modelBone.transform->Rotation = skeletonBone.rotation;
    }
    //update hierarchy
    for (int i=0; i<skeleton->bones.size(); ++i) {
        SpineSkeleton::Bone& skeletonBone = skeleton->bones[i];
        SpineModel::Bone& modelBone = model->bones[skeletonBone.name];
        SpineModel::Bones::iterator parentBone = model->bones.find(skeletonBone.parent);
        modelBone.object->Parent = parentBone==model->bones.end() ? object : parentBone->second.object;
    }
    
    for (int i=0; i<skeleton->slots.size(); ++i) {
        SpineSkeleton::Slot& skeletonSlot = skeleton->slots[i];
        SpineModel::Bones::iterator slotBone = model->bones.find(skeletonSlot.bone);
        if (slotBone == model->bones.end()) continue;
        SpineModel::Slot& modelSlot = model->slots[skeletonSlot.name];
        modelSlot.object = World()->CreateObject();
        modelSlot.transform = modelSlot.object->AddComponent<Transform>();
        modelSlot.sprite = modelSlot.object->AddComponent<Sprite>();
        modelSlot.sizeable = modelSlot.object->AddComponent<Sizeable>();
        modelSlot.object->AddComponent<Mesh>();
        modelSlot.object->AddComponent<Atlas>(object);
        modelSlot.object->AddComponent<TextureComponent>(object);
        modelSlot.object->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
        modelSlot.object->Parent = slotBone->second.object;
        
        //assign default image
        model->SetImage(skeletonSlot.name, skeletonSlot.attachment, *skeleton);
    }
    
    return object;
}

GameObject* SpineFactory::LoadSkeleton(std::string filename) {

    File file;
    if (!file.Load(filename)) return 0;

    std::string json = file.GetDataAsText();
    
    //std::cout<<json<<std::endl;
    
    const char* jsonCString = json.c_str();
    
    minijson::value root;
    
    minijson::error e = minijson::parse(jsonCString, root);
    if (e == minijson::no_error) {
    
        GameObject* object = World()->CreateObject();
        SpineSkeleton* skeleton = object->AddComponent<SpineSkeleton>();
        ParseJson(root, *skeleton);
        return object;
    } else {
        std::cout<<"Error parsing json:"<<std::endl<<minijson::errstr(e)<<std::endl<<std::endl;
        return 0;
    }
}

void SpineFactory::ParseJson(minijson::value& value, SpineSkeleton& skeleton) {
    switch (value.getType()) {
        case minijson::type::object_type: {
            minijson::object& o = value.get<minijson::object>();
            
            for (auto it = o.begin(); it!=o.end(); ++it) {
                if (it->first == "bones") {
                    ParseBones(it->second, skeleton);
                } else if (it->first == "slots") {
                    ParseSlots(it->second, skeleton);
                } else if (it->first == "skins") {
                     ParseSkins(it->second, skeleton);
                }
            }
            break;
        }
        case minijson::type::array_type: {
            minijson::array array = value.get<minijson::array>();
            for (minijson::array::iterator it = array.begin(); it!=array.end(); ++it) {
                ParseJson(*it, skeleton);
            }
            break;
        }
        default:
            break;
    }
}

void SpineFactory::ParseBones(minijson::value& bonesArray, SpineSkeleton& skeleton) {
    //std::cout<<bonesArray.str() <<std::endl;
    minijson::array array = bonesArray.get<minijson::array>();
    for (minijson::array::iterator it = array.begin(); it!=array.end(); ++it) {
        ParseBone(*it, skeleton);
    }
}

void SpineFactory::ParseBone(minijson::value &value, SpineSkeleton& skeleton) {
    minijson::object& boneData = value.get<minijson::object>();

    std::string boneName = boneData["name"].get<std::string>();
    
    skeleton.bones.push_back(SpineSkeleton::Bone());
    SpineSkeleton::Bone& bone = skeleton.bones.back();
    bone.name = boneName;
    
    bone.parent = boneData["parent"].get<std::string>();
    
    float x = boneData["x"].get<minijson::number>();
    float y = boneData["y"].get<minijson::number>();
    float rotation = (float)boneData["rotation"].get<minijson::number>();
    
    bone.position = Vector2(x*Scaler, y*Scaler);
    bone.rotation = ToRadians(rotation);
}


void SpineFactory::ParseSlots(minijson::value& slotsArray, SpineSkeleton& skeleton) {
    
    minijson::array array = slotsArray.get<minijson::array>();
    for (minijson::array::iterator it = array.begin(); it!=array.end(); ++it) {
        ParseSlot(*it, skeleton);
    }
}

void SpineFactory::ParseSlot(minijson::value &value, SpineSkeleton& skeleton) {
    minijson::object& slotData = value.get<minijson::object>();
    
    std::string name = slotData["name"].get<std::string>();
    skeleton.slots.push_back(SpineSkeleton::Slot());
    
    SpineSkeleton::Slot& slot = skeleton.slots.back();
    
    slot.name = name;
    slot.bone = slotData["bone"].get<std::string>();
    slot.attachment = slotData["attachment"].get<std::string>();
}

void SpineFactory::ParseSkins(minijson::value& skins, SpineSkeleton& skeleton) {
    
    minijson::object o = skins.get<minijson::object>();
    for (minijson::object::iterator it = o.begin(); it!=o.end(); ++it) {
        ParseSkin(it->first, it->second, skeleton);
    }
    
}

void SpineFactory::ParseSkin(std::string name, minijson::value &value, SpineSkeleton& skeleton) {
    SpineSkeleton::Skin& skin = skeleton.skins[name];
    
    minijson::object o = value.get<minijson::object>();
    for (minijson::object::iterator it = o.begin(); it!=o.end(); ++it) {
        ParseImage(it->first, skin, it->second, skeleton);
    }
}

void SpineFactory::ParseImage(std::string name, SpineSkeleton::Skin& skin, minijson::value &value, SpineSkeleton& skeleton) {

    SpineSkeleton::Images& images = skin.images[name];
    //minijson::object imageData = value.get<minijson::object>();
    
    minijson::object o = value.get<minijson::object>();
    for (minijson::object::iterator it = o.begin(); it!=o.end(); ++it) {
    
        minijson::object imageData = it->second.get<minijson::object>();
    
        SpineSkeleton::Image& image = images[it->first];
        
        image.name = it->first;
        image.position = Vector2(imageData["x"].get<minijson::number>()*Scaler, imageData["y"].get<minijson::number>()*Scaler);
        image.rotation = ToRadians(imageData["rotation"].get<minijson::number>());
        image.scale = Vector2(1,1);
        image.size = Vector2(imageData["width"].get<minijson::number>()*Scaler, imageData["height"].get<minijson::number>()*Scaler);
    }
}

bool SpineFactory::LoadAnimations(std::string filename, SpineAnimationCollection& animations) {
    
    File file;
    if (!file.Load(filename)) return 0;

    std::string json = file.GetDataAsText();
    const char* jsonCString = json.c_str();
    
    minijson::value root;
    
    minijson::error e = minijson::parse(jsonCString, root);
    if (e == minijson::no_error) {
        ParseAnimations(root, animations);
        return true;
    } else {
        std::cout<<"Error parsing json:"<<std::endl<<minijson::errstr(e)<<std::endl<<std::endl;
        return false;
    }
}

void SpineFactory::ParseAnimations(minijson::value &value, SpineAnimationCollection& animations) {
    minijson::object o = value.get<minijson::object>();

    for (auto it = o.begin(); it!=o.end(); ++it) {
        if (it->first == "animations") {
            minijson::object anims = it->second.get<minijson::object>();
            for (auto a = anims.begin(); a!=anims.end(); ++a) {
                ParseAnimation(a->second, a->first, animations);
            }
        }
    }
}

void SpineFactory::ParseAnimation(minijson::value &value, std::string name, SpineAnimationCollection& animations) {
    SpineAnimation& animation = animations[name];
    
    minijson::object o = value.get<minijson::object>();
    
    for (auto it = o.begin(); it!=o.end(); ++it) {
        if (it->first=="slots") {
            ParseAnimationSlots(it->second, animation);
        } else if (it->first == "bones") {
            ParseAnimationBones(it->second, animation);
        }
   }
   
   animation.CalcStartEndTime();
}

void SpineFactory::ParseAnimationSlots(minijson::value &value, SpineAnimation &animation) {
   
    minijson::object slots = value.get<minijson::object>();
    
    for (auto it = slots.begin(); it!=slots.end(); ++it) {
        
        SpineAnimation::SlotTimeline& slotTimeline = animation.slots[it->first];
        minijson::object slot = it->second.get<minijson::object>();
        if (slot.find("attachment") == slot.end()) continue;
        minijson::array slotFrames = slot["attachment"].get<minijson::array>();
        for (minijson::array::iterator slotFrame = slotFrames.begin(); slotFrame!=slotFrames.end(); ++slotFrame) {
            minijson::object slotFrameData = (*slotFrame).get<minijson::object>();
            SpineAnimation::SlotFrame frame;
            frame.time = slotFrameData["time"].get<minijson::number>();
            frame.attachment = slotFrameData["name"].get<minijson::string>();
            slotTimeline.Add(frame.time, frame);
        }
    }
    
}

void SpineFactory::ParseAnimationBones(minijson::value &value, SpineAnimation &animation) {

    minijson::object bones = value.get<minijson::object>();
    
    for (auto it = bones.begin(); it!=bones.end(); ++it) {
        minijson::object boneData = it->second.get<minijson::object>();
        SpineAnimation::Bone& bone = animation.bones[it->first];
        if (boneData.find("rotate") != boneData.end()) {
            minijson::array rotateFrames = boneData["rotate"].get<minijson::array>();
            for (minijson::array::iterator rotateFrame = rotateFrames.begin(); rotateFrame!=rotateFrames.end(); ++rotateFrame) {
                minijson::object rotateFrameData = (*rotateFrame).get<minijson::object>();
                SpineAnimation::RotateFrame frame;
                frame.time = rotateFrameData["time"].get<minijson::number>();
                frame.rotation = ToRadians(rotateFrameData["angle"].get<minijson::number>());
                //TODO: curves
                bone.rotation.Add(frame.time, frame);
            }
        }
        
        if (boneData.find("translate")!=boneData.end()) {
            minijson::array positionFrames = boneData["translate"].get<minijson::array>();
            for (minijson::array::iterator positionFrame = positionFrames.begin(); positionFrame!=positionFrames.end(); ++positionFrame) {
                minijson::object positionFrameData = (*positionFrame).get<minijson::object>();
                SpineAnimation::Vector2Frame frame;
                frame.time = positionFrameData["time"].get<minijson::number>();
                frame.value = Vector2(positionFrameData["x"].get<minijson::number>()*Scaler,
                                      positionFrameData["y"].get<minijson::number>()*Scaler);
                //TODO: curves
                bone.position.Add(frame.time, frame);
            }
        }
        
        if (boneData.find("scale")!=boneData.end()) {
            minijson::array scaleFrames = boneData["scale"].get<minijson::array>();
            for (minijson::array::iterator scaleFrame = scaleFrames.begin(); scaleFrame!=scaleFrames.end(); ++scaleFrame) {
                minijson::object scaleFrameData = (*scaleFrame).get<minijson::object>();
                SpineAnimation::Vector2Frame frame;
                frame.time = scaleFrameData["time"].get<minijson::number>();
                frame.value = Vector2(scaleFrameData["x"].get<minijson::number>(),
                                      scaleFrameData["y"].get<minijson::number>());
                //TODO: curves
                bone.scale.Add(frame.time,frame);
            }
        }
    }
}
