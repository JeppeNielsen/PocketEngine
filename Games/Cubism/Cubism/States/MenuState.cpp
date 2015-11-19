//
//  MenuState.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 9/7/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "MenuState.hpp"
#include "TransformAnimatorSystem.hpp"
#include "TouchAnimatorSystem.hpp"
#include "MathHelper.hpp"
#include "DraggableMotionSystem.hpp"
#include "VelocitySystem.hpp"
#include "LimitableSystem.hpp"
#include "TouchCancelSystem.hpp"

#include <sstream>

void MenuState::Initialize() {
    
    Gui* gui = Parent().gui;
    Menu* menu = Parent().menu;
    GameObject* menuObject = Parent().menuObject;
    GameObject* animations = Parent().animations;
   
    {
        
        TransformAnimationDatabase* animationsDatabase = animations->AddComponent<TransformAnimationDatabase>();
        
        TransformAnimation& wobble = animationsDatabase->AddAnimation("Wobble");
        wobble.scale.Add(0, 1, 1, 1);
        wobble.scale.Add(0.3f, 1.2f, 1.2f, 1.2f);
        wobble.scalePlayMode = TransformAnimationPlayMode::PingPong;
        
        wobble.rotation.Add(0, {0,0,-0.5f}, {0,0,-0.5f}, {0,0,-0.5f});
        wobble.rotation.Add(0.4f, {0,0,0.5f}, {0,0,0.5f}, {0,0,0.5f});
        wobble.rotationPlayMode = TransformAnimationPlayMode::PingPong;
        
        //wobble.position.Add(0, {-10,0,0});
        //wobble.position.Add(0.4f, {10,0,0});
        //wobble.positionPlayMode = TransformAnimationPlayMode::PingPong;
        
        TransformAnimation& rotation = animationsDatabase->AddAnimation("Rotation");
        rotation.rotation.Add(0, {0,0,0});
        rotation.rotation.Add(1, {0,0,0.4f});
        rotation.rotationPlayMode = TransformAnimationPlayMode::PingPong;
        
        
        TransformAnimation& down = animationsDatabase->AddAnimation("Down");
        down.scale.Add(0, 1);
        down.scale.Add(0.1f, 0.8f);
        
        TransformAnimation& up = animationsDatabase->AddAnimation("Up");
        up.scale.Add(0, 0.8f);
        up.scale.Add(0.1f, 1.0f);
        
        {
            TransformAnimation& anim = animationsDatabase->AddAnimation("FadeIn");
            anim.position.Add(0, {-Manager().ScreenSize().x,0});
            anim.position.Add(0.3f, 0);
            anim.rotation.Add(0, {0,0,-1.5f});
            anim.rotation.Add(0.3f, 0);
        }
        
        {
            TransformAnimation& anim = animationsDatabase->AddAnimation("FadeOut");
            anim.position.Add(0.0f, 0);
            anim.position.Add(0.3f, {Manager().ScreenSize().x,0});
            anim.rotation.Add(0, 0);
            anim.rotation.Add(0.3f, {0,0,1.5f});
        }
            
        {
            TransformAnimation& anim = animationsDatabase->AddAnimation("FadeOutTop");
            anim.position.Add(0.0f, 0);
            anim.position.Add(0.3f, {0,Manager().ScreenSize().y});
        }
        
        {
            TransformAnimation& anim = animationsDatabase->AddAnimation("FadeInTop");
            anim.position.Add(0.0f, {0,Manager().ScreenSize().y});
            anim.position.Add(0.3f, 0);
        }
        
        {
            TransformAnimation& anim = animationsDatabase->AddAnimation("FadeOutBottom");
            anim.position.Add(0.0f, 0);
            anim.position.Add(0.3f, {Manager().ScreenSize().x,0});
            //anim.rotation.Add(0.0f, {0,0,ToRadians(0)});
            //anim.rotation.Add(0.3f, {0,0,ToRadians(360)});
        }
        
        {
            TransformAnimation& anim = animationsDatabase->AddAnimation("FadeInBottom");
            anim.position.Add(0.0f, {Manager().ScreenSize().x,0});
            anim.position.Add(0.3f, 0);
            //anim.rotation.Add(0.0f, {0,0,ToRadians(-360)});
            //anim.rotation.Add(0.6f, {0,0,ToRadians(0)});
        }
        
    }
    
   
    {
        GameObject* frontMenu =  gui->CreatePivot(0, Manager().ScreenSize() * 0.5f); //gui->CreateSubMenu(0, menu, Manager().ScreenSize() * 0.5f, "Front", animations, "FadeIn", "FadeOut");
        
        GameObject* top = gui->CreatePivot(frontMenu, {0, Manager().ScreenSize().y * 0.25f});
        gui->AddMenuAnimator(top, menuObject, "Front", animations, "FadeInTop", "FadeOutTop");
        
            Vector2 size = {897,200};
            GameObject* logo = gui->CreateControl(top, "Logo", {0,0}, size);
            logo->GetComponent<Transform>()->Anchor = size * 0.5f;
       
        GameObject* bottom = gui->CreatePivot(frontMenu, {0, -Manager().ScreenSize().y * 0.25f});
        gui->AddMenuAnimator(bottom, menuObject, "Front", animations, "FadeInBottom", "FadeOutBottom");
        
    
        GameObject* playButton = Parent().CreateButton(bottom, "Button", 0, {300,150}, "PLAY", 30);
            playButton->AddComponent<MenuButton>()->menu = menu;
            playButton->GetComponent<MenuButton>()->menuToActivate = "Levels";
    }
    
    {
        GameObject* levelsMenu = gui->CreatePivot(0, Manager().ScreenSize() * 0.5f);
        
        GameObject* top = gui->CreatePivot(levelsMenu, {0, Manager().ScreenSize().y * 0.25f});
        gui->AddMenuAnimator(top, menuObject, "Levels", animations, "FadeInTop", "FadeOutTop");
        
        GameObject* backButton = Parent().CreateButton(top, "Button", {Manager().ScreenSize().x*0.5f-50,140}, {100,100}, "X", 20);
        backButton->AddComponent<MenuButton>()->menu = menu;
        backButton->GetComponent<MenuButton>()->menuToActivate = "Front";
        
        GameObject* bottom = gui->CreatePivot(levelsMenu, {0, 0});
        gui->AddMenuAnimator(bottom, menuObject, "Levels", animations, "FadeInBottom", "FadeOutBottom");
        
        size_t items = Parent().puzzleDatabase.Puzzles().size();
        float itemsHeight = 200;
        float itemsMargin = 30;
        
        float totalHeight = items * (itemsHeight + itemsMargin) - itemsMargin;
        
        GameObject* panel = CreateScrollPanel(bottom, totalHeight);
        
        for (int i =0; i<items; i++) {
            
            GameObject* button = Parent().CreateButton(panel, "Button", {250, totalHeight - (itemsHeight+itemsMargin) * 0.5f - i*(itemsHeight+itemsMargin)}, {420,itemsHeight}, Parent().puzzleDatabase.Puzzles()[i].Name, 40);
            button->GetComponent<Touchable>()->ClickThrough = true;
            button->AddComponent<TouchableCanceller>()->MovementToCancel = 5;
            button->GetComponent<Touchable>()->Click += event_handler(this, &MenuState::LevelClicked, i);
            
            levelObjects.push_back(button);
        }
    }
    
    UpdateLevelList();
}

void MenuState::UpdateLevelList() {
    for (int i=0; i<levelObjects.size(); i++) {
        bool unlocked = Parent().puzzleProgress.IsLevelUnlocked(i);
        levelObjects[i]->GetComponent<Colorable>()->Color = unlocked ? Colour::White() : Colour(0.5f, 0.5f, 0.5f, 1.0f);
        levelObjects[i]->EnableComponent<Touchable>(unlocked);
        if (i==Parent().puzzleProgress.CurrentLevelIndex()) {
            Vector3 worldPosition = levelObjects[i]->GetComponent<Transform>()->World.GetValue()->TransformPosition(levelObjects[i]->GetComponent<Sizeable>()->Size * 0.5f);
            Vector3 localPosition = levelObjects[i]->Parent()->GetComponent<Transform>()->WorldInverse.GetValue()->TransformPosition(worldPosition);
            //localPosition = levelObjects[i]->GetComponent<Hierarchy>()->Parent()->Owner()->GetComponent<Transform>()->Local.GetValue()->TransformPosition(localPosition);
            levelObjects[i]->Parent()->GetComponent<Transform>()->Position = {0.0f, levelObjects.size() * (200.0f+30.0f) * -0.5f + i*(200.0f+30.0f)+100.0f,0.0f};
            
        }
    }
}

void MenuState::Enabled() {
    UpdateLevelList();
}


GameObject* MenuState::CreateScrollPanel(Pocket::GameObject *parent, float height) {
    Vector2 size(500,height);
    GameObject* panel = Parent().gui->CreatePivot(parent); //gui->CreateControl(parent, "White", 0, {500, height});
    panel->GetComponent<Transform>()->Anchor = size * 0.5f;
    panel->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(size*0.5f, size, Colour::White());
    panel->AddComponent<Touchable>();
    panel->AddComponent<Draggable>();
    panel->AddComponent<Velocity>()->Friction = 4;
    panel->AddComponent<Limitable>()->max = Vector2(0,height*0.5f-100);
    panel->GetComponent<Limitable>()->min = Vector2(0,-height*0.5f+100);
    panel->GetComponent<Velocity>()->MinimumSpeedBeforeStop = 50.0f;
    
    
    return panel;
}

void MenuState::LevelClicked(Pocket::TouchData touch, int levelIndex) {
    Parent().puzzleProgress.SetLevelIndex(levelIndex);
    std::cout<<"Level #"<<(levelIndex+1)<<" clicked"<< std::endl;
    Parent().CurrentState = "Playing";
}

void MenuState::Update(float dt) {

}

void MenuState::Render() {
    Parent().world.Render();
}
