//
//  TextBoxLabelSystem.cpp
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "TextBoxLabelSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Sizeable.hpp"
#include <cmath>

using namespace Pocket;

static float cursorWidth = 1.05f;

void TextBoxLabelSystem::Initialize() {
    AddComponent<TextBox>();
    AddComponent<Label>();
    AddComponent<Mesh>();
    AddComponent<Sizeable>();
    cursor = 0;
    activeTextbox = 0;
    timer = 0;
    activeTextboxAdded = 0;
    activeTextboxAddedGO = 0;
}

void TextBoxLabelSystem::ObjectAdded(Pocket::GameObject *object) {
    TextBox* textbox = object->GetComponent<TextBox>();
    textbox->Text.Changed += event_handler(this, &TextBoxLabelSystem::TextBoxTextChanged, object);
    textbox->Active.Changed += event_handler(this, &TextBoxLabelSystem::TextBoxActiveChanged, object);
    if (textbox->Active) {
        activeTextboxAdded = textbox;
        activeTextboxAddedGO = object;
    }
    TextBoxTextChanged(textbox, object);
}

void TextBoxLabelSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<TextBox>()->Text.Changed -= event_handler(this, &TextBoxLabelSystem::TextBoxTextChanged, object);
    object->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &TextBoxLabelSystem::TextBoxActiveChanged, object);
    if (cursor && activeTextbox == object->GetComponent<TextBox>()) {
        cursor->Remove();
        cursor = 0;
    }
}

void TextBoxLabelSystem::TextBoxTextChanged(TextBox *textBox, GameObject* object) {
    object->GetComponent<Label>()->Text = textBox->Text;
    
    if (activeTextbox == textBox) {
        MoveCursor(textBox, object);
    }
}

void TextBoxLabelSystem::TextBoxActiveChanged(TextBox *textBox, Pocket::GameObject *object) {
 
    activeTextbox = 0;
    if (cursor) {
        cursor->Remove();
        cursor = 0;
    }
    
    if (textBox->Active) {
        activeTextbox = textBox;
        
        cursor = World()->CreateObject();
        cursor->AddComponent<Transform>();
        cursor->Parent = object;
        cursor->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, {cursorWidth,object->GetComponent<Label>()->FontSize * 1.1f}, Colour::White());
        cursor->AddComponent<Material>();
        MoveCursor(textBox, object);
        
        timer = 0;
    }
}

void TextBoxLabelSystem::MoveCursor(Pocket::TextBox *textBox, Pocket::GameObject *object) {
    if (!cursor) return;
    Mesh* mesh = object->GetComponent<Mesh>();
    if (mesh->Vertices().empty()) {
        Sizeable* sizeable = object->GetComponent<Sizeable>();
        cursor->GetComponent<Transform>()->Position = sizeable->Size * 0.5f;
    } else {
        const BoundingBox& bounds = mesh->LocalBoundingBox;
        Vector3 local = Vector3(bounds.center.x + bounds.extends.x * 0.5f + cursorWidth, bounds.center.y,0);
        cursor->GetComponent<Transform>()->Position = local;
    }
}

void TextBoxLabelSystem::Update(float dt) {
    
    if (activeTextboxAdded && activeTextboxAdded->Active) {
        TextBoxActiveChanged(activeTextboxAdded, activeTextboxAddedGO);
        activeTextboxAdded = 0;
    }
    
    if (!cursor) return;
    timer += dt;
    
    cursor->EnableComponent<Material>(fmodf(timer, 0.7f)<0.35f);
}
