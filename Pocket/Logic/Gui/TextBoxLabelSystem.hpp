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
        
        const float cursorWidth = 1.05f;
    
        void Initialize() {
            cursor = 0;
            activeTextbox = 0;
            timer = 0;
            activeTextboxAdded = 0;
            activeTextboxAddedGO = 0;
        }

        void ObjectAdded(GameObject *object) {
            TextBox* textbox = object->template GetComponent<TextBox>();
            textbox->Text.Changed.Bind(this, &TextBoxLabelSystem::TextBoxChanged, object);
            textbox->Active.Changed.Bind(this, &TextBoxLabelSystem::TextBoxChanged, object);
            if (textbox->Active) {
                activeTextboxAdded = textbox;
                activeTextboxAddedGO = object;
            }
            TextBoxChanged(object);
        }

        void ObjectRemoved(GameObject *object) {
            TextBox* textBox = object->template GetComponent<TextBox>();
            textBox->Text.Changed.Unbind(this, &TextBoxLabelSystem::TextBoxChanged, object);
            textBox->Active.Changed.Unbind(this, &TextBoxLabelSystem::TextBoxChanged, object);
            if (cursor && activeTextbox == textBox) {
                cursor->Remove();
                cursor = 0;
            }
        }

        void TextBoxChanged(GameObject* object) {
            TextBox* textBox = object->template GetComponent<TextBox>();
            object->template GetComponent<Label>()->Text = textBox->Text;
            
            if (activeTextbox == textBox) {
                MoveCursor(textBox, object);
            }
        }

        void TextBoxActiveChanged(bool& active, GameObject *object) {
            
            TextBox* textBox = object->template GetComponent<TextBox>();
            
            activeTextbox = 0;
            if (cursor) {
                cursor->Remove();
                cursor = 0;
            }
            
            if (textBox->Active) {
                activeTextbox = textBox;
                
                cursor = this->World().CreateObject();
                cursor->template AddComponent<Transform>();
                cursor->Parent = object;
                cursor->template AddComponent<Mesh>()->template GetMesh<Vertex>().AddQuad(0, {cursorWidth,object->template GetComponent<Label>()->FontSize * 1.1f}, Colour::Black());
                cursor->template AddComponent<Material>();
                MoveCursor(textBox, object);
                
                timer = 0;
            }
        }

        void MoveCursor(Pocket::TextBox *textBox, GameObject *object) {
            if (!cursor) return;
            Mesh* mesh = object->template GetComponent<Mesh>();
            if (mesh->Vertices().empty()) {
                Sizeable* sizeable = object->template GetComponent<Sizeable>();
                cursor->template GetComponent<Transform>()->Position = sizeable->Size * 0.5f;
            } else {
                const BoundingBox& bounds = mesh->LocalBoundingBox;
                Vector3 local = Vector3(bounds.center.x + bounds.extends.x * 0.5f + cursorWidth, bounds.center.y,0);
                cursor->template GetComponent<Transform>()->Position = local;
            }
        }

        void Update(float dt) {
            
            if (activeTextboxAdded && activeTextboxAdded->Active) {
                TextBoxChanged(activeTextboxAddedGO);
                activeTextboxAdded = 0;
            }
            
            if (!cursor) return;
            timer += dt;
            
            //cursor->template EnableComponent<Material>(fmodf(timer, 0.7f)<0.35f);
        }


        GameObject* cursor;
        TextBox* activeTextbox;
        TextBox* activeTextboxAdded;
        GameObject* activeTextboxAddedGO;
        float timer;
        
    };
}