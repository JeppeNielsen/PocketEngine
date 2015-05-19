//
//  PieceControllerSystem.cpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PieceControllerSystem.hpp"
#include "PointGravity.hpp"

void PieceControllerSystem::Initialize() {
    AddComponent<PieceCollider>();
    AddComponent<PieceController>();
}

void PieceControllerSystem::ObjectAdded(Pocket::GameObject *object) {
    
}

void PieceControllerSystem::ObjectRemoved(Pocket::GameObject *object) {
    
}

void PieceControllerSystem::SetInputManager(Pocket::InputManager &input) {
    input.ButtonDown += event_handler(this, &PieceControllerSystem::ButtonDown);
}

void PieceControllerSystem::ButtonDown(std::string button) {
    buttonDowns.insert(button);
}

void PieceControllerSystem::Update(float dt) {
    if (buttonDowns.empty()) return;
    for (Buttons::iterator buttonIt = buttonDowns.begin(); buttonIt!=buttonDowns.end(); ++buttonIt) {
        for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
            ModifyTransform(*it, (*buttonIt));
        }
    }
    buttonDowns.clear();
}

void PieceControllerSystem::ModifyTransform(GameObject* colliderObject, std::string button) {
    PieceCollider* pieceCollider = colliderObject->GetComponent<PieceCollider>();
    if (button == "w") {
        pieceCollider->AddMovement(0, 1);
    } else if (button == "s") {
        colliderObject->GetComponent<PointGravity>()->Gravity = 50.0f;
        colliderObject->RemoveComponent<PieceController>();
    } else if (button == "a") {
        pieceCollider->AddMovement(Point(-1,0),0);
    } else if (button == "d") {
        pieceCollider->AddMovement(Point(1,0),0);
    }
}