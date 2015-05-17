//
//  MenuSystem.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "MenuSystem.hpp"

void MenuSystem::Initialize() {
    AddComponent<Menu>();
    AddComponent<MenuAnimator>();
    AddComponent<TransformAnimator>();
    AddComponent<TransformAnimationDatabase>();
}

void MenuSystem::ObjectAdded(Pocket::GameObject *object) {
    Menu* menu = object->GetComponent<Menu>();
    menu->ActiveMenu.ChangedWithOld += event_handler(this, &MenuSystem::ActiveMenuChanged, object);
    
    MenuAnimator* menuAnimator = object->GetComponent<MenuAnimator>();
    TransformAnimator* animator = object->GetComponent<TransformAnimator>();
    TransformAnimationDatabase* database = object->GetComponent<TransformAnimationDatabase>();
    TransformAnimation* anim =
    menu->ActiveMenu() == menuAnimator->Menu ? database->GetAnimation(menuAnimator->FadeInAnimation) : database->GetAnimation(menuAnimator->FadeOutAnimation);
    animator->SetFromLastFrame(anim);
}

void MenuSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Menu>()->ActiveMenu.ChangedWithOld -= event_handler(this, &MenuSystem::ActiveMenuChanged, object);
}

void MenuSystem::ActiveMenuChanged(Property<Menu*, std::string>::EventData d, Pocket::GameObject *object) {
    MenuAnimator* menuAnimator = object->GetComponent<MenuAnimator>();
    TransformAnimator* animator = object->GetComponent<TransformAnimator>();
    TransformAnimationDatabase* database = object->GetComponent<TransformAnimationDatabase>();
    
    if (d.Old == menuAnimator->Menu) {
        animator->Play(database->GetAnimation(menuAnimator->FadeOutAnimation));
    }
    
    if (d.Current == menuAnimator->Menu) {
        animator->Play(database->GetAnimation(menuAnimator->FadeInAnimation));
    }
    
}
