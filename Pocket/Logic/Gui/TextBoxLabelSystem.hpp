//
//  TextBoxLabelSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Sizeable.hpp"
#include "TextBox.hpp"
#include "Label.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Sizeable.hpp"
#include <cmath>

namespace Pocket {

    template<typename T>
    class TextBoxLabelSystem : public GameSystem<T, TextBox, Label, Mesh, Sizeable> {
    public:
        using GameObject = GameObject<T>;
        
        static float cursorWidth;
    
        void Initialize() {
        
            cursorWidth = 1.05f;
            cursor = 0;
            activeTextbox = 0;
            timer = 0;
            activeTextboxAdded = 0;
            activeTextboxAddedGO = 0;
        }

        void ObjectAdded(GameObject *object) {
            TextBox* textbox = object->GetComponent<TextBox>();
            textbox->Text.Changed += event_handler(this, &TextBoxTextChanged, object);
            textbox->Active.Changed += event_handler(this, &TextBoxActiveChanged, object);
            if (textbox->Active) {
                activeTextboxAdded = textbox;
                activeTextboxAddedGO = object;
            }
            TextBoxTextChanged(textbox, object);
        }

        void ObjectRemoved(GameObject *object) {
            object->GetComponent<TextBox>()->Text.Changed -= event_handler(this, &TextBoxLabelSystem::TextBoxTextChanged, object);
            object->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &TextBoxLabelSystem::TextBoxActiveChanged, object);
            if (cursor && activeTextbox == object->GetComponent<TextBox>()) {
                cursor->Remove();
                cursor = 0;
            }
        }

        void TextBoxTextChanged(TextBox *textBox, GameObject* object) {
            object->GetComponent<Label>()->Text = textBox->Text;
            
            if (activeTextbox == textBox) {
                MoveCursor(textBox, object);
            }
        }

        void TextBoxActiveChanged(TextBox *textBox, Pocket::GameObject *object) {
         
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
                cursor->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, {cursorWidth,object->GetComponent<Label>()->FontSize * 1.1f}, Colour::Black());
                cursor->AddComponent<Material>();
                MoveCursor(textBox, object);
                
                timer = 0;
            }
        }

        void MoveCursor(Pocket::TextBox *textBox, Pocket::GameObject *object) {
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

        void Update(float dt) {
            
            if (activeTextboxAdded && activeTextboxAdded->Active) {
                TextBoxActiveChanged(activeTextboxAdded, activeTextboxAddedGO);
                activeTextboxAdded = 0;
            }
            
            if (!cursor) return;
            timer += dt;
            
            cursor->EnableComponent<Material>(fmodf(timer, 0.7f)<0.35f);
        }


        GameObject* cursor;
        TextBox* activeTextbox;
        TextBox* activeTextboxAdded;
        GameObject* activeTextboxAddedGO;
        float timer;
        
    };
}