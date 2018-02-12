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
#include "ColorSystem.hpp"
#include "DraggableMotionSystem.hpp"
#include "VelocitySystem.hpp"
#include "LimitableSystem.hpp"
#include "SelectedColorerSystem.hpp"
#include "LayoutSystem.hpp"
#include "PanelSystem.hpp"
#include "PanelDropSystem.hpp"
#include "PanelAreaSystem.hpp"
#include "TouchCancelSystem.hpp"
#include "HoverSystem.hpp"
#include "ScrollWheelMoverSystem.hpp"

namespace Pocket {
    class Gui : public GameConcept {
    public:
        
        using Fonts = std::vector<GameObject*>;
        
        void Initialize();
        static void CreateSubSystems(GameStorage& storage);
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
        GameObject* CreateFont(const std::string& fontFile);
        GameObject* CreateLabel(GameObject *parent, const Vector2 &position, const Vector2 &size, GameObject *font, const std::string &text, float fontSize);
        GameObject* CreateLabelControl(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize);
        GameObject* CreateTextBox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize);
        GameObject* CreateListbox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject** pivot);
        
        GameObject* CreateLayoutControl(GameObject* parent, const std::string& spriteName, const Vector2& minSize, const Vector2& desiredSize, const Vector2& maxSize, Layouter::LayoutMode layoutMode = Layouter::LayoutMode::Vertical);
        GameObject* CreateLayout(GameObject* parent, const Vector2& minSize, const Vector2& desiredSize, const Vector2& maxSize, Layouter::LayoutMode layoutMode = Layouter::LayoutMode::Vertical);
        
        void AddLayouter(GameObject* object, const Vector2& minSize, const Vector2& desiredSize, const Vector2& maxSize, Layouter::LayoutMode layoutMode = Layouter::LayoutMode::Vertical);
        
        const Fonts& GetFonts() const;
    private:
        RenderSystem* renderer;
        GameObject* camera;
        GameObject* atlas;
        TouchSystem* touchSystem;
        HoverSystem* hoverSystem;
        TextBoxSystem* textboxSystem;
        Fonts fonts;
    };
}
