//
//  Gui.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "GameFactory.hpp"
#include "RenderSystem.hpp"
#include "TransformHierarchy.hpp"
#include "TouchSystem.hpp"
#include "LabelMeshSystem.hpp"
#include "SpriteMeshSystem.hpp"
#include "SpriteTextureSystem.hpp"
#include "Atlas.hpp"
#include "Orderable.hpp"
#include "DraggableSystem.hpp"
#include "LayoutSystem.hpp"
#include "TextBoxSystem.hpp"
#include "MenuButtonSystem.hpp"
#include "MenuSystem.hpp"
#include "ColorSystem.hpp"

namespace Pocket {
    class Gui : public GameFactory {
    public:
        Gui();
        ~Gui();
        
        void Setup(const std::string& atlasTexture, const std::string& atlasXml, const Box& viewport, InputManager& inputManager);
        void Setup(GameObject* atlas, const Box& viewport, InputManager& inputManager);
        
        GameObject* Atlas();
        GameObject* GetCamera();
        
        GameObject* CreatePivot(GameObject* parent = 0, const Vector2& position = Vector2(0,0));
        GameObject* CreateControl(GameObject* parent = 0);
        GameObject* CreateControl(GameObject* parent, const std::string& spriteName, const Vector2& size = Vector2(1,1));
        GameObject* CreateControl(GameObject* parent, const std::string& spriteName, const Vector2& position, const Vector2& size);
        GameObject* CreateLabelControl(GameObject* parent, const std::string& spriteName, const Vector2& position, const Vector2& size, GameObject* font, std::string text, float fontSize);
        GameObject* CreateTextBox(GameObject* parent, const std::string& spriteName, const Vector2& position, const Vector2& size, GameObject* font, std::string text, float fontSize);
        
        
        GameObject* CreateFont(const std::string& fontFile, const std::string& fontAtlasName);
        GameObject* CreateLabel(GameObject* parent, const Vector2& position, const Vector2& size, GameObject* font, const std::string& text, float fontSize);
        
        GameObject* CreateClipper(GameObject* parent, bool push);
        
        GameObject* CreateMenu(GameObject* parent, const Vector2& position);
        void AddMenuAnimator(GameObject* control, GameObject* menu, std::string activeMenu, GameObject* animations, const std::string& fadeInAnimation, const std::string& fadeOutAnimation);
        
        GameObject* CreateListbox(GameObject* parent, const std::string& spriteName, const Vector2& position, const Vector2& size, GameObject** pivot);
        
    public:
        void Initialize();
        
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