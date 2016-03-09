//
//  MenuSystem.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "MenuSystem.hpp"

using namespace Pocket;

void MenuSystem::ObjectAdded(Pocket::GameObject *object) {
    Menu* menu = object->GetComponent<Menu>();
    menu->ActiveMenu.Changed.Bind(this, &MenuSystem::ActiveMenuChanged, object);
    
    MenuAnimator* menuAnimator = object->GetComponent<MenuAnimator>();
    TransformAnimator* animator = object->GetComponent<TransformAnimator>();
    TransformAnimationDatabase* database = object->GetComponent<TransformAnimationDatabase>();
    TransformAnimation* anim =
    menu->ActiveMenu() == menuAnimator->Menu ? database->GetAnimation(menuAnimator->FadeInAnimation) : database->GetAnimation(menuAnimator->FadeOutAnimation);
    animator->SetFromLastFrame(anim);
}

void MenuSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Menu>()->ActiveMenu.Changed.Unbind(this, &MenuSystem::ActiveMenuChanged, object);
}

void MenuSystem::ActiveMenuChanged(Pocket::GameObject *object) {
    Menu* menu = object->GetComponent<Menu>();
    MenuAnimator* menuAnimator = object->GetComponent<MenuAnimator>();
    TransformAnimator* animator = object->GetComponent<TransformAnimator>();
    TransformAnimationDatabase* database = object->GetComponent<TransformAnimationDatabase>();
    
    if (menu->ActiveMenu.PreviousValue() == menuAnimator->Menu) {
        animator->Play(database->GetAnimation(menuAnimator->FadeOutAnimation));
    }
    
    if (menu->ActiveMenu() == menuAnimator->Menu) {
        animator->Play(database->GetAnimation(menuAnimator->FadeInAnimation));
    }
}
