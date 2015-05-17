//
//  TextureSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/21/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "TextureSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "TextureComponent.hpp"

using namespace Pocket;

void TextureSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Mesh>();
    AddComponent<Material>();
    AddComponent<TextureComponent>();
    manipulator = 0;
}

void TextureSystem::ObjectAdded(Pocket::GameObject *object) {
    manipulator->UpdateFromObject(object);
}

void TextureSystem::ObjectRemoved(Pocket::GameObject *object) {
    manipulator->UpdateFromObject(object);
}

