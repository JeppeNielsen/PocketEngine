//
//  FieldInfoEditors.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GuiFieldEditors.hpp"
#include <fstream>
#include "FileHelper.hpp"
#include "GameObjectHandle.hpp"
#include "Cloner.hpp"
#include "EditorDropTarget.hpp" // <- TODO: not part of engine

using namespace Pocket;

template<typename T, int Size>
struct FieldInfoEditorTextboxes : public GuiFieldEditor {

    void SetField(void* field) override {
        this->field = static_cast<T*>(field);
    }

    void Initialize(Gui* gui, GameObject* parent) override {
        GameObject* control = gui->CreateControl(parent);
        control->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Horizontal;
        
        for(int i=0; i<Size; ++i) {
            GameObject* textBox = gui->CreateTextBox(control, "TextBox", 0, 0, 0, "", 15.0f);
            textBox->GetComponent<Touchable>()->ClickThrough = true;
            textBox->Children()[0]->GetComponent<TextBox>()->Active.Changed.Bind(this, &FieldInfoEditorTextboxes<T, Size>::TextChanged, textBox);
            textBox->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
            textBox->AddComponent<Layouter>()->Min = {20, 20};
            textBox->GetComponent<Layouter>()->Desired = {100, 20};
            textBox->GetComponent<Layouter>()->Max = {5000, 20};
            
            textBoxes[i] = textBox;
        }
        prev = (*this->field);
        forceUpdate = true;
    }
    
    void Destroy() override {
        for(int i=0; i<Size; ++i) {
             textBoxes[i]->Children()[0]->template GetComponent<TextBox>()->Active.Changed.Unbind(this, &FieldInfoEditorTextboxes<T, Size>::TextChanged, textBoxes[i]);
             textBoxes[i]->Remove();
        }
    }
    
    void TextChanged(GameObject* textBoxGO) {
        TextBox* textBox = textBoxGO->Children()[0]->GetComponent<TextBox>();
        if (textBox->Active) return;
        
        for(int i=0; i<Size; ++i) {
             if (textBoxes[i] == textBoxGO) {
                TextboxChanged(i, textBox->Text);
             }
        }
    }
    
    void Update(float dt) override {
        bool changed = prev!=(*this->field) || forceUpdate;
        if (changed) {
            forceUpdate = false;
            for(int i=0; i<Size; ++i) {
                if (!textBoxes[i]->Children()[0]->template GetComponent<TextBox>()->Active()) {
                    std::stringstream s;
                    UpdateTextbox(i,s);
                    textBoxes[i]->Children()[0]->template GetComponent<TextBox>()->Text = s.str();
                }
            }
        }
        prev = (*this->field);
    }

    virtual void TextboxChanged(int index, std::string text) = 0;
    virtual void UpdateTextbox(int index, std::stringstream& stream) = 0;
    GameObject* textBoxes[Size];
    
    T* field;
    T prev;
    bool forceUpdate;
};

struct FieldInfoEditorFloat : public FieldInfoEditorTextboxes<float, 1> {
    void TextboxChanged(int index, std::string text) override {
        float value = (float)atof(text.c_str());
        (*field) = value;
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        stream<<(*field);
    }
};

struct FieldInfoEditorInt : public FieldInfoEditorTextboxes<int, 1> {
    void TextboxChanged(int index, std::string text) override {
        int value = (int)atoi(text.c_str());
        (*field) = value;
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        stream<<(*field);
    }
};

struct FieldInfoEditorVector2 : public FieldInfoEditorTextboxes<Vector2, 2> {
    void TextboxChanged(int index, std::string text) override {
        float value = (float)atof(text.c_str());
        (*field).Set(index, value);
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        stream<<(*field)[index];
    }
};

struct FieldInfoEditorVector3 : public FieldInfoEditorTextboxes<Vector3, 3> {
    void TextboxChanged(int index, std::string text) override {
        float value = (float)atof(text.c_str());
        (*field).Set(index, value);
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        stream<<(*field)[index];
    }
};



struct FieldInfoEditorQuaternion : public FieldInfoEditorTextboxes<Quaternion, 3> {
    void TextboxChanged(int index, std::string text) override {
        float value = (float)atof(text.c_str());
    
        Vector3 euler = (*field).ToEuler();
        euler *= MathHelper::RadToDeg;
        euler.Set(index, value);
        euler *= MathHelper::DegToRad;
        (*field) = Quaternion(euler);
    }
    
    void UpdateTextbox(int index, std::stringstream& stream) override {
        Vector3 euler = (*field).ToEuler();
        euler *= MathHelper::RadToDeg;
        stream<<euler[index];
    }
};

struct FieldInfoEditorString : public FieldInfoEditorTextboxes<std::string, 1> {
    void TextboxChanged(int index, std::string text) override {
        (*field) = text;
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        stream<<(*field);
    }
};

struct FieldInfoEditorRect : public FieldInfoEditorTextboxes<Rect, 4> {
    void TextboxChanged(int index, std::string text) override {
        float value = (float)atof(text.c_str());
        
        if (index == 0) {
            (*field).x = value;
        } else if (index == 1) {
            (*field).y = value;
        } else if (index == 2) {
            (*field).width = value;
        } else {
            (*field).height = value;
        }
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        if (index == 0) {
            stream<<(*field).x;
        } else if (index == 1) {
            stream<<(*field).y;
        } else if (index == 2) {
            stream<<(*field).width;
        } else {
            stream<<(*field).height;
        }
    }
};

struct FieldInfoEditorBool : public GuiFieldEditor {

    void SetField(void* field) override {
        this->field = static_cast<bool*>(field);
    }

    void Initialize(Gui* gui, GameObject* parent) override {
        control = gui->CreateControl(parent, "TextBox");
        control->AddComponent<Layouter>()->Min = {20, 20};
        control->GetComponent<Layouter>()->Desired = {100, 20};
        control->GetComponent<Layouter>()->Max = {5000, 20};
        control->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Horizontal;
        control->GetComponent<Touchable>()->Click.Bind(this, &FieldInfoEditorBool::Clicked);

        checkBox = gui->CreateControl(control, "TextBox");

        checkBox->AddComponent<Layouter>()->Min = {20, 20};
        checkBox->GetComponent<Layouter>()->Desired = {100, 20};
        checkBox->GetComponent<Layouter>()->Max = {5000, 20};

        checkBox->RemoveComponent<Touchable>();
        checkBox->GetComponent<Colorable>()->Color = Colour::Black();
        
        UpdateVisuals();
    }
    
    void Destroy() override {
        control->GetComponent<Touchable>()->Click.Unbind(this, &FieldInfoEditorBool::Clicked);
        control->Remove();
    }
    
    void Clicked(TouchData d) {
        *field = !(*field);
        UpdateVisuals();
    }
    
    void UpdateVisuals() {
        checkBox->Enabled = *field;
    }
    
    void Update(float dt) override {
    
    }
    
    bool* field;
    GameObject* control;
    GameObject* checkBox;
};

struct FieldInfoEditorBox : public FieldInfoEditorTextboxes<Box, 4> {
    void TextboxChanged(int index, std::string text) override {
        float value = (float)atof(text.c_str());
        
        if (index == 0) {
            (*field).left = value;
        } else if (index == 1) {
            (*field).top = value;
        } else if (index == 2) {
            (*field).right = value;
        } else {
            (*field).bottom = value;
        }
    }
    void UpdateTextbox(int index, std::stringstream& stream) override {
        if (index == 0) {
            stream<<(*field).left;
        } else if (index == 1) {
            stream<<(*field).top;
        } else if (index == 2) {
            stream<<(*field).right;
        } else {
            stream<<(*field).bottom;
        }
    }
};

template<> IFieldEditor* FieldEditorCreator<int>::Create(int* ptr) {
    FieldInfoEditorInt* editor = new FieldInfoEditorInt();
    editor->SetField(ptr);
    return editor;
}

template<> IFieldEditor* FieldEditorCreator<float>::Create(float* ptr) {
    FieldInfoEditorFloat* editor = new FieldInfoEditorFloat();
    editor->SetField(ptr);
    return editor;
}

template<> IFieldEditor* FieldEditorCreator<Vector2>::Create(Vector2* ptr) {
    FieldInfoEditorVector2* editor = new FieldInfoEditorVector2();
    editor->SetField(ptr);
    return editor;
}

template<> IFieldEditor* FieldEditorCreator<Vector3>::Create(Vector3* ptr) {
    FieldInfoEditorVector3* editor = new FieldInfoEditorVector3();
    editor->SetField(ptr);
    return editor;
}

template<> IFieldEditor* FieldEditorCreator<Quaternion>::Create(Quaternion* ptr) {
    FieldInfoEditorQuaternion* editor = new FieldInfoEditorQuaternion();
    editor->SetField(ptr);
    return editor;
}

template<> IFieldEditor* FieldEditorCreator<std::string>::Create(std::string* ptr) {
    FieldInfoEditorString* editor = new FieldInfoEditorString();
    editor->SetField(ptr);
    return editor;
}

template<> IFieldEditor* FieldEditorCreator<Rect>::Create(Rect* ptr) {
    FieldInfoEditorRect* editor = new FieldInfoEditorRect();
    editor->SetField(ptr);
    return editor;
}

template<> IFieldEditor* FieldEditorCreator<bool>::Create(bool* ptr) {
    FieldInfoEditorBool* editor = new FieldInfoEditorBool();
    editor->SetField(ptr);
    return editor;
}

template<> IFieldEditor* FieldEditorCreator<Box>::Create(Box* ptr) {
    FieldInfoEditorBox* editor = new FieldInfoEditorBox();
    editor->SetField(ptr);
    return editor;
}


TypeEditorTitle::Callback TypeEditorTitle::Title = [] (void* guiPtr, void* parentPtr, const std::string& title) -> void* {
    Gui* gui = static_cast<Gui*>(guiPtr);
    GameObject* parent = static_cast<GameObject*>(parentPtr);
    
    GameObject* labelGo = gui->CreateLabel(parent, 0, 0, 0, title, 15);
    labelGo->AddComponent<Layouter>()->Min = {20,20};
    labelGo->AddComponent<Layouter>()->Desired = {100,20};
    labelGo->AddComponent<Layouter>()->Max = {5000,20};
    Label* label = labelGo->GetComponent<Label>();
    labelGo->GetComponent<Colorable>()->Color = Colour::Black();
    label->HAlignment = Font::HAlignment::Center;
    label->VAlignment = Font::VAlignment::Middle;
    return 0;
};

struct ReferenceComponentEditor : public GuiFieldEditor {

    GameObject::ReferenceComponent component;
    GameObject* control;
    GameObject* parent;
    GameObject* label;

    void SetField(void* field) override {
        component = *static_cast<GameObject::ReferenceComponent*>(field);
        menu = 0;
    }

    void Initialize(Gui* gui, GameObject* parent) override {
    
        this->parent = parent;
    
        control = gui->CreateControl(parent);
        control->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Horizontal;
        
        GameObject* textBox = gui->CreateControl(control, "TextBox");
        textBox->AddComponent<Layouter>()->Min = {20, 20};
        textBox->GetComponent<Layouter>()->Desired = {100, 20};
        textBox->GetComponent<Layouter>()->Max = {5000, 20};
        textBox->GetComponent<Touchable>()->Click.Bind([this] (TouchData d){
            MenuClicked();
        });
        textBox->AddComponent<EditorDropTarget>()->OnDropped = [this] (EditorObject* editorObject) {
            if (!editorObject->gameObject->HasComponent(component.componentId)) return;
            component.object->ReplaceComponent(component.componentId, editorObject->gameObject);
            UpdateLabel();
        };
        
        //gui->CreateLabel(Pocket::GameObject *parent, const Pocket::Vector2 &position, const Pocket::Vector2 &size, Pocket::GameObject *font, const std::string &text, float fontSize)
        label = gui->CreateLabel(textBox, 0, 20, 0, "", 20);
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        
        //label->AddComponent<Layouter>(textBox);
        label->AddComponent<Colorable>()->Color = Colour::Black();
        
        TypeEditorTitle::Title(gui, parent, component.name);
        
        this->gui = gui;
        
        this->parent->World()->Input().TouchDown.Bind(this, &ReferenceComponentEditor::TouchUp);
        
        UpdateLabel();
    }
    
    void TouchUp(TouchEvent e) {
        if (menu) {
            menu->Remove();
        }
        menu = 0;
    }
    
    void Destroy() override {
        this->parent->World()->Input().TouchDown.Unbind(this, &ReferenceComponentEditor::TouchUp);
    }
    
    void Update(float dt) override {
        
    }
    
    void UpdateLabel() {
        GameObject* owner = component.object->GetComponentOwner(component.componentId);
        std::string text;
        if (owner) {
            std::string path = owner->TryGetRootPath();
            text = FileHelper::GetFileNameFromPath(path);
            
            std::stringstream ss;
            ss<<owner->RootId();
            text += " : " + ss.str();
        } else {
            text = "Removed";
        }
        label->GetComponent<Label>()->Text = text;
    }
    
    GameObject* menu;
    Gui* gui;
    
    struct ClickedData {
        std::string guid;
        int objectId;
    };
    
    void MenuClicked() {
        if (!component.object->World()->GetPaths) {
            return;
        }
        
        if (menu) {
            menu->Remove();
        }
        
        GameWorld* world =component.object->World();
        
        std::vector<std::string> guids;
        std::vector<std::string> paths;
        world->GetPaths(guids, paths);
        
        menu = gui->CreateControl(0, "TextBox", 0, {200,200});
        menu->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Vertical;
        menu->GetComponent<Transform>()->Position = control->GetComponent<Transform>()->World().TransformPosition(0);
        
        for (int i=0; i<guids.size(); ++i) {
        
            std::ifstream file;
            file.open(paths[i]);
            
            
            //std::cout << "Start parse file: "<<p.second<<std::endl;
            
            
            //context.World().TryParseJson(file, GameIdHelper::GetComponentID<Transform>(), [] (int parent, int object) {
            //    std::cout << " parent: " << parent << "  object: " << object << std::endl;
            //});

        
            world->TryParseJson(file, component.componentId, [&, this](int parentId, int id) {
                GameObject* button = gui->CreateControl(menu, "Box");
                button->AddComponent<Layouter>()->Desired = { 200, 30 };
                button->GetComponent<Layouter>()->Min = {50,30};
                button->GetComponent<Layouter>()->Max = {500,30};
                
                std::string text = FileHelper::GetFileNameFromPath(paths[i]);
                
                std::stringstream ss;
                ss<<id;
                text += " : " + ss.str();
                
                GameObject* label = gui->CreateLabel(button, 0, 10, 0, text, 20);
                //label->AddComponent<Layouter>(button);
                label->AddComponent<Colorable>()->Color = Colour::Black();
                
                button->GetComponent<Touchable>()->Down.Bind(this, &ReferenceComponentEditor::Clicked, { guids[i], id } );

            });
            std::cout << "guid: " << guids[i] << "  path:" << paths[i] << std::endl;
        }
    }
    
    void Clicked(TouchData touch, ClickedData d) {
        
        if (menu) {
            menu->Remove();
            menu = 0;
        }
        
        std::cout << "Guid : " << d.guid << "  object id :"<< d.objectId<<std::endl;
        
        GameObject* root = component.object->World()->TryFindRoot(d.guid);
        if (!root) return;
        GameObject* object = root->FindObject(d.objectId);
        component.object->ReplaceComponent(component.componentId, object);
        
        UpdateLabel();
    }
    
};

template<> IFieldEditor* FieldEditorCreator<GameObject::ReferenceComponent>::Create(Pocket::GameObject::ReferenceComponent *ptr) {
    ReferenceComponentEditor* editor = new ReferenceComponentEditor();
    editor->SetField(ptr);
    return editor;
}


struct GameObjectHandleEditor : public GuiFieldEditor {

    GameObjectHandle* handle;
    GameObject* control;
    GameObject* parent;
    GameObject* label;

    void SetField(void* field) override {
        handle = static_cast<GameObjectHandle*>(field);
        menu = 0;
    }

    void Initialize(Gui* gui, GameObject* parent) override {
    
        this->parent = parent;
    
        control = gui->CreateControl(parent);
        control->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Horizontal;
        
        GameObject* textBox = gui->CreateControl(control, "TextBox");
        textBox->AddComponent<Layouter>()->Min = {20, 20};
        textBox->GetComponent<Layouter>()->Desired = {100, 20};
        textBox->GetComponent<Layouter>()->Max = {5000, 20};
        textBox->GetComponent<Touchable>()->Click.Bind([this] (TouchData d){
            MenuClicked();
        });
        
        
        //gui->CreateLabel(Pocket::GameObject *parent, const Pocket::Vector2 &position, const Pocket::Vector2 &size, Pocket::GameObject *font, const std::string &text, float fontSize)
        label = gui->CreateLabel(textBox, 0, 20, 0, "", 20);
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        
        //label->AddComponent<Layouter>(textBox);
        label->AddComponent<Colorable>()->Color = Colour::Black();
        
        this->gui = gui;
        this->parent->World()->Input().TouchDown.Bind(this, &GameObjectHandleEditor::TouchUp);
        
        UpdateLabel();
    }
    
    void UpdateLabel() {
        GameObject* target = handle->operator->();
        std::string path = !target ? "" : target->TryGetRootPath();
        std::string text = FileHelper::GetFileNameFromPath(path);
        
        if (target) {
            std::stringstream ss;
            ss<<target->RootId();
            text += " : " + ss.str();
        } else {
            text += "none";
        }
        label->GetComponent<Label>()->Text = text;
    }
    
    void TouchUp(TouchEvent e) {
        if (menu) {
            menu->Remove();
        }
        menu = 0;
    }
    
    void Destroy() override {
        this->parent->World()->Input().TouchDown.Unbind(this, &GameObjectHandleEditor::TouchUp);
    }
    
    void Update(float dt) override {
        
    }
    
    GameObject* menu;
    Gui* gui;
    
    struct ClickedData {
        std::string guid;
        int objectId;
    };
    
    void MenuClicked() {
        if (!parent->World()->GetPaths) {
            return;
        }
        
        if (menu) {
            menu->Remove();
        }
        
        GameWorld* world = parent->World();
        
        std::vector<std::string> guids;
        std::vector<std::string> paths;
        world->GetPaths(guids, paths);
        
        
    
        menu = gui->CreateControl(0, "TextBox", 0, {200,200});
        menu->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Vertical;
        menu->GetComponent<Transform>()->Position = control->GetComponent<Transform>()->World().TransformPosition(0);
        
        for (int i=0; i<guids.size(); ++i) {
        
            std::ifstream file;
            file.open(paths[i]);
            
            //std::cout << "Start parse file: "<<p.second<<std::endl;
            //context.World().TryParseJson(file, GameIdHelper::GetComponentID<Transform>(), [] (int parent, int object) {
            //    std::cout << " parent: " << parent << "  object: " << object << std::endl;
            //});
            
            bool hasCreatedItem = false;
            
            world->TryParseJson(file, -1, [&, this](int parentId, int id) {
            
                if (hasCreatedItem) return;
                GameObject* button = gui->CreateControl(menu, "Box");
                button->AddComponent<Layouter>()->Desired = { 200, 30 };
                button->GetComponent<Layouter>()->Min = {50,30};
                button->GetComponent<Layouter>()->Max = {500,30};
                
                std::string text = FileHelper::GetFileNameFromPath(paths[i]);
                
                std::stringstream ss;
                ss<<id;
                text += " : " + ss.str();
                
                GameObject* label = gui->CreateLabel(button, 0, 10, 0, text, 20);
                //label->AddComponent<Layouter>(button);
                label->AddComponent<Colorable>()->Color = Colour::Black();
                
                button->GetComponent<Touchable>()->Down.Bind(this, &GameObjectHandleEditor::Clicked, { guids[i], id } );
                
                hasCreatedItem = true;
                
            }, [&, this](const std::string& componentName) -> bool {
                return hasCreatedItem || componentName != GameIdHelper::GetClassName<Cloner>();
            });
           
            std::cout << "guid: " << guids[i] << "  path:" << paths[i] << std::endl;
        }
    }
    
    void Clicked(TouchData touch, ClickedData d) {
        if (menu) {
            menu->Remove();
            menu = 0;
        }
        
        std::cout << "Guid : " << d.guid << "  object id :"<< d.objectId<<std::endl;
        
        GameObject* root = parent->World()->TryFindRoot(d.guid);
        if (!root) return;
        GameObject* object = root->FindObject(d.objectId);
        
        handle->SetRoot(root);
        handle->operator=(object);
        
        UpdateLabel();
    }
    
};

template<> IFieldEditor* FieldEditorCreator<GameObjectHandle>::Create(GameObjectHandle *ptr) {
    GameObjectHandleEditor* editor = new GameObjectHandleEditor();
    editor->SetField(ptr);
    return editor;
}
