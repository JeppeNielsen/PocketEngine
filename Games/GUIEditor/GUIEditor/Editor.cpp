#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"
#include "Colorable.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "File.hpp"
#include "SizeModifierSystem.hpp"
#include "SizeModifierNodeSystem.hpp"
#include "SizeModifierLineSystem.h"
#include "ClickSelectorSystem.hpp"
#include "SelectableCollection.hpp"
#include "GridSystem.h"
#include <vector>

using namespace Pocket;

template<class T>
struct StringSerializer {
    static void FromString(T* value, std::string s) {
        
    }
};

template<>
struct StringSerializer<float> {
    static void FromString(float* value, std::string s) {
        *value = (float)atof(s.c_str());
    }
};

template<>
struct StringSerializer<Vector3> {
    static void FromString(Vector3* value, std::string s) {
        *value = Vector3::Deserialize(s);
    }
};




Component(Floater)
    public:
        float value;
    SERIALIZE_FIELDS_BEGIN
    SERIALIZE_FIELD(value)
    SERIALIZE_FIELDS_END
};


Component(FieldEditor)
    public:
        FieldEditor() : Object(this), Field(this), editor(0) {}
        void Reset() {
            if (editor) {
                editor->Destroy();
            }
            editor = 0;
            Object = 0;
            Field = "";
        }
        ISerializedFieldEditor* editor;
        Property<FieldEditor*, ISerializable*> Object;
        Property<FieldEditor*, std::string> Field;
};

SYSTEM(FieldEditorSystem, FieldEditor, Sizeable)
public:
    Gui* gui;

    void ObjectAdded(GameObject* object) {
        object->GetComponent<FieldEditor>()->Object.Changed += event_handler(this, &FieldEditorSystem::FieldChanged, object);
        object->GetComponent<FieldEditor>()->Field.Changed += event_handler(this, &FieldEditorSystem::FieldChanged, object);
        FieldChanged(object->GetComponent<FieldEditor>(), object);
    }

    void ObjectRemoved(GameObject* object) {
        object->GetComponent<FieldEditor>()->Object.Changed -= event_handler(this, &FieldEditorSystem::FieldChanged, object);
        object->GetComponent<FieldEditor>()->Field.Changed -= event_handler(this, &FieldEditorSystem::FieldChanged, object);
    }

    void Update(float dt) {
        for(auto object : Objects()) {
            if (object->GetComponent<FieldEditor>()->editor) {
                object->GetComponent<FieldEditor>()->editor->Update(dt);
            }
        }
    }

    void FieldChanged(FieldEditor* editor, GameObject* object) {
        if (editor->editor) {
            editor->editor->Destroy();
            delete editor->editor;
            editor->editor = 0;
        }
        if (!editor->Object()) {
            return;
        }
        auto fields = editor->Object()->GetFields();
        ISerializedField* field = fields.GetField(editor->Field);
        if (field) {
            editor->editor = field->CreateEditor(gui, object);
        }
    }
};


struct FieldEditorVector3 : public SerializedFieldEditor<Vector3, Gui, GameObject> {
    
    GameObject* textBox[3];
    
    void Initialize(Gui* context, GameObject* parent) {
        Vector2 size = parent->GetComponent<Sizeable>()->Size;
        size.x /= 3.0f;
        for (int i=0; i<3; ++i) {
            textBox[i] = context->CreateTextBox(parent, "Box", {i*size.x,0}, size, 0, "", 15.0f);
            textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &FieldEditorVector3::TextChanged,  textBox[i]);
            textBox[i]->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
        }
    }
    
    void Destroy() {
        for (int i=0; i<3; ++i) {
            textBox[i]->Children()[0]->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &FieldEditorVector3::TextChanged, textBox[i]);
            textBox[i]->Remove();
        }
    }
    
    void TextChanged(TextBox* textBox, GameObject* object) {
        if (textBox->Active) return;
        float value = (float)atof(textBox->Text().c_str());
        if (object == this->textBox[0]) {
            (*field).x = value;
        } else if (object == this->textBox[1]) {
            (*field).y = value;
        } else {
            (*field).z = value;
        }
        //prev = (*field);
    }
    
    void Update(float dt) {
        for (int i=0; i<3; ++i) {
            bool changed = prev[i]!=(*field)[i];
            if (changed && !textBox[i]->Children()[0]->GetComponent<TextBox>()->Active()) {
                std::stringstream s;
                s<<(*field)[i];
                textBox[i]->Children()[0]->GetComponent<TextBox>()->Text = s.str();
            }
        }
        
        prev = (*field);
    }
    Vector3 prev;
};

struct FieldEditorVector2 : public SerializedFieldEditor<Vector2, Gui, GameObject> {
    
    GameObject* textBox;
    GameObject* textBox2;
    
    void Initialize(Gui* context, GameObject* parent) {
        Vector2 size = parent->GetComponent<Sizeable>()->Size;
        size.x*=0.5f;
    
        textBox = context->CreateTextBox(parent, "Box", 0, size, 0, "", 15.0f);
        textBox->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &FieldEditorVector2::TextChanged);
        textBox->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
        textBox2 = context->CreateTextBox(parent, "Box", {size.x,0}, size, 0, "", 15.0f);
        textBox2->Children()[0]->GetComponent<TextBox>()->Active.Changed += event_handler(this, &FieldEditorVector2::TextChanged2);
        textBox2->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    }
    
    void Destroy() {
        textBox->Children()[0]->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &FieldEditorVector2::TextChanged);
        textBox->Remove();
        textBox2->Children()[0]->GetComponent<TextBox>()->Active.Changed -= event_handler(this, &FieldEditorVector2::TextChanged2);
        textBox2->Remove();
    }
    
    void TextChanged(TextBox* textBox) {
        //if (textBox->Active) return;
        (*field).x = (float)atof(textBox->Text().c_str());
    }
    
    void TextChanged2(TextBox* textBox) {
        //if (textBox->Active) return;
        (*field).y = (float)atof(textBox->Text().c_str());
    }
    
    void Update(float dt) {
        bool xChanged = prev.x!=(*field).x;
    
        if (xChanged && !textBox->Children()[0]->GetComponent<TextBox>()->Active()) {
            std::stringstream s;
            s<<(*field).x;
            textBox->Children()[0]->GetComponent<TextBox>()->Text = s.str();
        }
         bool yChanged = prev.y!=(*field).y;
        
        if (yChanged && !textBox2->Children()[0]->GetComponent<TextBox>()->Active()) {
            std::stringstream s;
            s<<(*field).y;
            textBox2->Children()[0]->GetComponent<TextBox>()->Text = s.str();
        }

        prev = (*field);
    }
    Vector2 prev;
};


/*

template<>
struct SerializedFieldEditor<Vector3> : public ISerializedFieldEditor {
    
    GameObject* textBox;
    Vector3* field;

    void Initialize(Vector3* field, void* context) {
        this->field = field;
        std::pair<Gui*, GameObject*> c = *((std::pair<Gui*, GameObject*>*)context);
        
        textBox = c.first->CreateTextBox(c.second, "Box", 0, c.second->GetComponent<Sizeable>()->Size, 0, "", 15.0f);
        textBox->Children()[0]->GetComponent<TextBox>()->Text.Changed += event_handler(this, &SerializedFieldEditor<Vector3>::TextChanged);
        textBox->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    }
    
    void Destroy() {
        textBox->Children()[0]->GetComponent<TextBox>()->Text.Changed -= event_handler(this, &SerializedFieldEditor<Vector3>::TextChanged);
        textBox->Remove();
    }
    
    void TextChanged(TextBox* textBox) {
        (*field).x = (float)atof(textBox->Text().c_str());
    }
};

*/

/*

template<>
struct SerializedFieldEditor<float> : public ISerializedFieldEditor {
    
    GameObject* textBox;
    float* field;

    void Initialize(float* field, void* context) {
        this->field = field;
        std::pair<Gui*, GameObject*> c = *((std::pair<Gui*, GameObject*>*)context);
        
        textBox = c.first->CreateTextBox(c.second, "Box", 0, c.second->GetComponent<Sizeable>()->Size, 0, "", 15.0f);
        textBox->Children()[0]->GetComponent<TextBox>()->Text.Changed += event_handler(this, &SerializedFieldEditor<float>::TextChanged);
        textBox->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    }
    
    void Destroy() {
        textBox->Children()[0]->GetComponent<TextBox>()->Text.Changed -= event_handler(this, &SerializedFieldEditor<float>::TextChanged);
        textBox->Remove();
    }
    
    void TextChanged(TextBox* textBox) {
        *field = (float)atof(textBox->Text().c_str());
    }
};

*/

Component(Speed)
    public:
        Speed() : speed(this) { }
        Property<Speed*, Vector2> speed;

    SERIALIZE_FIELDS_BEGIN
    SERIALIZE_FIELD(speed)
    SERIALIZE_FIELDS_END
};

class Editor : public GameState<Editor> {
public:
    GameWorld world;
    Gui* gui;
    GameObject* font;
    GameObject* box;
    SelectableCollection* selected;
    Transform* transform;
    
    void Initialize() {
    
        SerializedField<Vector3>::Editor = [] () { return new FieldEditorVector3(); };
        SerializedField<Vector2>::Editor = [] () { return new FieldEditorVector2(); };
       
    
        //SerializedFieldEditor<float> floatT;
        //floatT.Initialize((float*)0,(void*)0);
        
        
        
        gui = world.CreateFactory<Gui>();
        world.CreateSystem<ClickSelectorSystem>();
        world.CreateSystem<SizeModifierSystem>();
        world.CreateSystem<SizeModifierNodeSystem>();
        world.CreateSystem<SizeModifierLineSystem>();
        selected = world.CreateSystem<SelectableCollection>();
        world.CreateSystem<GridSystem>();
        world.CreateSystem<FieldEditorSystem>()->gui = gui;
        
        gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
        font = gui->CreateFont("Font.fnt", "Font");
        
        box = gui->CreateControl(0, "Box", 100, 200);
        box->AddComponent<Draggable>();
        box->AddComponent<Selectable>();
        box->AddComponent<SizeModifier>();
        box->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        box->AddComponent<Layoutable>()->VerticalAlignment = Layoutable::VAlignment::Relative;
        box->AddComponent<Speed>()->speed = {10,23};
        
        //GameObject* textBox = gui->CreateTextBox(0, "Box", {500,100}, {200,50}, 0, "Testing Editor", 10);
        
        {
        GameObject* editor = gui->CreateControl(0, "Box", 300, {300,50});
        auto e = editor->AddComponent<FieldEditor>();
        e->Object = box->GetComponent<Transform>();
        e->Field = "Anchor";
        }
        {
        GameObject* editor = gui->CreateControl(0, "Box", {300,350}, {300,50});
        auto e = editor->AddComponent<FieldEditor>();
        e->Object = box->GetComponent<Sizeable>();
        e->Field = "Size";
        }
        
        Input.ButtonDown += event_handler(this, &Editor::ButtonDown);
    }
    
    void ButtonDown(std::string button) {
    
    }
    
    void Update(float dt) {
        world.Update(dt*0.5f);
        world.Update(dt*0.5f);
        box->GetComponent<Speed>()->speed += {0,0.01f};
        box->GetComponent<Transform>()->Position += {10 * dt,0,0};
        std::cout<<box->GetComponent<Speed>()->speed<<std::endl;
    }
    
    void Render() {
        glClearColor(0, 0.5, 0.3f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<Editor>();
	return 0;
}