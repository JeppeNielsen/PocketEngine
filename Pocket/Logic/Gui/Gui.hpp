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
//#include "LayoutSystem.hpp"
#include "TextBoxSystem.hpp"
#include "TextBoxLabelSystem.hpp"
//#include "MenuButtonSystem.hpp"
//#include "MenuSystem.hpp"
//#include "ColorSystem.hpp"

namespace Pocket {
    class Gui : public GameConcept {
    public:
        
        void Initialize(GameWorld* world) ;
        void Setup(const std::string &atlasTexture, const std::string &atlasXml, const Box& viewport, InputManager& inputManager);
        void Setup(GameObject *atlas, const Box &viewport, InputManager &inputManager);
        GameObject* GetCamera();
        GameObject* GetAtlas();
        
        GameObject* CreatePivot(GameObject* parent, const Vector2& position);
        GameObject* CreateControl(GameObject* parent);
        GameObject* CreateControl(GameObject *parent, const std::string& spriteName, const Vector2 &position, const Vector2 &size);
        GameObject* CreateControl(GameObject *parent, const std::string &spriteName, const Vector2& size);
        GameObject* CreateClipper(GameObject *parent, bool push);
        GameObject* CreateFont(const std::string& fontFile, const std::string& fontAtlasName);
        GameObject* CreateLabel(GameObject *parent, const Vector2 &position, const Vector2 &size, GameObject *font, const std::string &text, float fontSize);
        GameObject* CreateLabelControl(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize);
        GameObject* CreateTextBox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize);
/*
        GameObject* CreateMenu(GameObject *parent, const Vector2 &position) {
            GameObject* pivot = CreatePivot(parent);
            pivot->template GetComponent<Transform>()->Position = position;
            pivot->template AddComponent<Menu>();
            return pivot;
        }


        void AddMenuAnimator(GameObject *control, GameObject *menu, std::string activeMenu, GameObject *animations, const std::string &fadeInAnimation, const std::string &fadeOutAnimation) {
            control->template AddComponent<Menu>(menu);
            control->template AddComponent<TransformAnimator>();
            MenuAnimator* menuAnimator = control->template AddComponent<MenuAnimator>();
            menuAnimator->Menu = activeMenu;
            menuAnimator->FadeInAnimation = fadeInAnimation;
            menuAnimator->FadeOutAnimation = fadeOutAnimation;
            control->template AddComponent<TransformAnimationDatabase>(animations);
        }

        GameObject* CreateListbox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject** pivot) {
            GameObject* listbox = CreateControl(parent, spriteName, position, size);
            listbox->template AddComponent<Layoutable>();
            listbox->template GetComponent<Touchable>()->ClickThrough = false;
            CreateClipper(listbox, true);
            GameObject* p = CreatePivot(listbox);
            p->template AddComponent<Sizeable>();
            p->template AddComponent<Layoutable>()->ChildLayouting = Layoutable::ChildLayouting::VerticalStackedFit;
            p->template GetComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
            p->template AddComponent<Touchable>(listbox);
            p->template AddComponent<Draggable>()->Movement = Draggable::MovementMode::YAxis;
            p->template AddComponent<DraggableMotion>();
            p->template AddComponent<Velocity>()->MinimumSpeedBeforeStop = 5;
            p->template GetComponent<Velocity>()->Friction = 5;
            Limitable* limitable = p->template AddComponent<Limitable>();
            limitable->Size = p->template GetComponent<Sizeable>();
            limitable->View = listbox->template GetComponent<Sizeable>();
            CreateClipper(listbox, false);
            (*pivot)=p;
            return listbox;
        }
*/

    private:
        GameWorld* world;
        RenderSystem* renderer;
        GameObject* camera;
        GameObject* atlas;
        TouchSystem* touchSystem;
        TextBoxSystem* textboxSystem;
        typedef std::vector<GameObject*> Fonts;
        Fonts fonts;
    };
}