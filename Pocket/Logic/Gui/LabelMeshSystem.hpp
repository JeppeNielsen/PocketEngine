//
//  SpriteMeshSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameWorld.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "Label.hpp"
#include "Font.hpp"

namespace Pocket {
    class Sizeable;
    class LabelMeshSystem : public GameSystem {
    public:
        void Initialize();
    protected:    
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    
    private:
        void SizeChanged(Sizeable* sizeable, GameObject* context);
        void LabelChanged(Label* label,  GameObject* context);
        
        void SomethingChanged(GameObject* object);
    
        void AddText(Mesh& mesh, const Pocket::Font &font, std::string text, const Pocket::Vector2& size, float fontSize, Font::HAlignment hAlign, Font::VAlignment vAlign, bool wordWrap, const Box& t, const Colour& color);
        
    };
}