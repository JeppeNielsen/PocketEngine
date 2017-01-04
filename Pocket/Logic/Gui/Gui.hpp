//
//  Gui.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "GameSystem.hpp"
#include "RenderSystem.hpp"
#include "TransformHierarchy.hpp"
#include "TouchSystem.hpp"
#include "LabelMeshSystem.hpp"
#include "SpriteMeshSystem.hpp"
#include "SpriteTextureSystem.hpp"
#include "DraggableSystem.hpp"
#include "HierarchyOrder.hpp"
#include "DroppableSystem.hpp"
#include "LayoutSystem.hpp"
#include "TextBoxSystem.hpp"
#include "TextBoxLabelSystem.hpp"
#include "MenuButtonSystem.hpp"
#include "MenuSystem.hpp"
#include "ColorSystem.hpp"
#include "DraggableMotionSystem.hpp"
#include "VelocitySystem.hpp"
#include "LimitableSystem.hpp"
#include "DroppableSystem.hpp"
#include "SelectedColorerSystem.hpp"
#include "LayoutSystem.hpp"

namespace Pocket {
    class Gui : public GameConcept {
    public:
        
        void Initialize();
        void Setup(const std::string &atlasTexture, const std::string &atlasXml, const Rect& viewport);
        void Setup(GameObject *atlas, const Rect &viewport);
        GameObject* GetCamera();
        GameObject* GetAtlas();
        
        GameObject* CreatePivot();
        GameObject* CreatePivot(GameObject* parent);
        GameObject* CreatePivot(GameObject* parent, const Vector2& position);
        GameObject* CreateControl(GameObject* parent);
        GameObject* CreateControl(GameObject* parent, const std::string& spriteName);
        GameObject* CreateControl(GameObject *parent, const std::string& spriteName, const Vector2 &position, const Vector2 &size);
        GameObject* CreateControl(GameObject *parent, const std::string &spriteName, const Vector2& size);
        GameObject* CreateClipper(GameObject *parent, bool push);
        GameObject* CreateFont(const std::string& fontFile, const std::string& fontAtlasName);
        GameObject* CreateLabel(GameObject *parent, const Vector2 &position, const Vector2 &size, GameObject *font, const std::string &text, float fontSize);
        GameObject* CreateLabelControl(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize);
        GameObject* CreateTextBox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize);
        
        GameObject* CreateMenu(GameObject *parent, const Vector2 &position);

        void AddMenuAnimator(GameObject *control, GameObject *menu, std::string activeMenu, GameObject *animations, const std::string &fadeInAnimation, const std::string &fadeOutAnimation);
        GameObject* CreateListbox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject** pivot);
    private:
        RenderSystem* renderer;
        GameObject* camera;
        GameObject* atlas;
        TouchSystem* touchSystem;
        TextBoxSystem* textboxSystem;
        typedef std::vector<GameObject*> Fonts;
        Fonts fonts;
    };
}