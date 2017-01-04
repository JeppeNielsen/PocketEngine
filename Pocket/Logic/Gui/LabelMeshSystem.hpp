//
//  SpriteMeshSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameSystem.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "Label.hpp"
#include "Font.hpp"
#include "Atlas.hpp"

namespace Pocket {
    class LabelMeshSystem : public GameSystem<Label, Font, Mesh, Sizeable, Atlas> {
    public:
        void ObjectAdded(GameObject *object);
        void ObjectRemoved(GameObject *object);
        void SomethingChanged(GameObject* object);
        void AddText(Mesh& mesh, const Pocket::Font &font, std::string text, const Pocket::Vector2& size, float fontSize, Font::HAlignment hAlign, Font::VAlignment vAlign, bool wordWrap, const Box& t, const Colour& color);
    };
}