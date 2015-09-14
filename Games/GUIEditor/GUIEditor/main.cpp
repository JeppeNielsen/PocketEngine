#include "Engine.hpp"
#include "GameWorld.hpp"
#include "Gui.hpp"
#include "Colorable.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "File.hpp"

using namespace Pocket;

template<class T>
class SerializedType;

class ISerializedType {
    public:
    ~ISerializedType() {
        for (size_t i=0; i<types.size(); ++i) {
            delete types[i];
        }
    }

    std::string name;
    
    template<class T>
    void AddField(T& field, std::string name) {
        SerializedType<T>* type = new SerializedType<T>();
        type->name = name;
        type->field = &field;
        types.push_back(type);
    }
    
    void Serialize(minijson::object_writer& writer) {
        DoSerialize(writer);
        for (auto t : types) {
            t->Serialize(writer);
        }
    }
protected:
    virtual void DoSerialize(minijson::object_writer& writer) { }
private:
    typedef std::vector<ISerializedType*> Types;
    Types types;
};

template<class T>
class SerializedType : public ISerializedType {
public:
    virtual ~SerializedType() { }
    
    void DoSerialize(minijson::object_writer& writer) override {
        JsonSerializer<T> s;
        s.Serialize(name, *field, writer);
    }
public:
    T* field;
};





class TransformComponent : public ISerializable {
public:
    TransformComponent() : Position(this) { }

    Property<TransformComponent*, Vector3> Position;
    Vector3 Velocity;
    int Tester;
    
    struct Waypoint : public ISerializable {
        float x;
        float y;
        
        SERIALIZE_FIELDS_BEGIN
        SERIALIZE_FIELD(x);
        SERIALIZE_FIELD(y);
        SERIALIZE_FIELDS_END
    };
    
    std::vector<int> Numbers;
    std::vector<Waypoint> waypoints;

   SERIALIZE_FIELDS_BEGIN
   SERIALIZE_FIELD(Position);
   SERIALIZE_FIELD(Velocity);
   SERIALIZE_FIELD(Numbers);
   SERIALIZE_FIELD(waypoints);
   //SERIALIZE_FIELD(Tester)
   SERIALIZE_FIELDS_END
};




class Game : public GameState<Game> {
public:
    GameWorld world;
    Gui* gui;
    GameObject* font;
    GameObject* box;
    GameObject* label;
    
    void Initialize() {
    
    
    
        TransformComponent component;
        
        component.Position = {20,66,98};
        component.Numbers = {1,2,3,4};
        TransformComponent::Waypoint w;
        component.waypoints.push_back(w);
        component.waypoints.push_back(w);
        
        {
        minijson::writer_configuration config;
        config = config.pretty_printing(true);
        auto fields = component.GetFields();
        minijson::object_writer writer(std::cout, config);
        fields.Serialize(writer);
        writer.close();
        }
        
        TransformComponent copy;
        {
            auto fields = copy.GetFields();
            std::ifstream myfile (File::GetFullPath("GameObjectJson.txt"));
            minijson::istream_context ctx(myfile);
            fields.Deserialize(ctx);
            myfile.close();
        }
        
        /*
        minijson::parse_object(ctx, [&] (const char* name, minijson::value v) {
        
            std::cout<<name<<std::endl;
            if (v.type() == minijson::Object)
            {
                minijson::parse_object(ctx, [&] (const char* name, minijson::value v) {
                    std::cout<< v.as_string()<< std::endl;
                });
            } else if ( v.type() == minijson::Array) {
                minijson::parse_array(ctx, [&] (minijson::value v) {
                
                
                    std::cout<< v.as_string()<< std::endl;
                });
            } else {
                std::cout<<"wolla"<<std::endl;
            }
        });

        myfile.close();
        */
        
        Vector2 a;
        a.x = 4;
        
        std::cout << a;
        
    
        gui = world.CreateFactory<Gui>();
        gui->Setup("images.png", "images.xml", Manager().Viewport(), Input);
        font = gui->CreateFont("Font.fnt", "Font");
        
        box = gui->CreateControl(0, "Box", 100, 200);
        box->AddComponent<Draggable>();
        
        label = gui->CreateLabel(box, {0,0}, 200, font, "Testing new Gui Editor, yay \n line break", 11);
        label->AddComponent<TextBox>()->Text = "Hvad er dit navn?";
        label->AddComponent<Touchable>(box);
        
        GameObject* parent = world.CreateObject();
        parent->AddComponent<Transform>()->Position = {1,2,3};
        
        GameObject* child = world.CreateObject();
        child->Parent = parent;
        child->AddComponent<Transform>()->Position = {10,20,30};
        child->AddComponent<Mesh>();
        
        std::cout<<std::endl;
        std::cout<<std::endl;
        
        parent->ToJson(std::cout);
        std::cout<<std::endl;
        std::cout<<std::endl;
        std::cout<<std::endl;
        
        box->ToJson(std::cout);
        
        
        std::ifstream myfile (File::GetFullPath("TestObject.txt"));
        GameObject* instance = world.CreateObjectFromJson(myfile);
        myfile.close();
        
        Transform* transform = instance->GetComponent<Transform>();
        
        Transform* transformChild = instance->Children()[0]->GetComponent<Transform>();
        
        
        
    }
    
    void Update(float dt) {
       
        world.Update(dt);
        label->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour::Black());
       
        //std::cout<< box->GetComponent<Orderable>()->Order << " : " << label->GetComponent<Orderable>()->Order<<std::endl;
       
    }
    
    void Render() {
        world.Render();
    }
};

int main_nonono() {
    Engine e;
    e.Start<Game>();
	return 0;
}