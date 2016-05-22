//
//  ScriptExample.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Property.hpp"
#include <iostream>

struct Jumpable {
    
    Jumpable() : JumpHeight(765) {}
    
    int JumpHeight;  
    
};

struct RotatorComponent {
    RotatorComponent() { speed = {2.5f,1.5f,0}; min = 1.0f; max=1.2f; }
    Pocket::Vector3 speed;
    float min;
    float max;
    float time;
};
struct RotatorScriptSystem : public GameSystem<Pocket::Transform, RotatorComponent> {
    void Update(float dt) {
        for(auto o : Objects()) {
            auto t = o->GetComponent<Pocket::Transform>();
            auto r = o->GetComponent<RotatorComponent>();
            t->EulerRotation += r->speed * dt;
            t->Scale = r->min + (r->max - r->min) * (0.5f + sinf(r->time) * 0.5f);
            r->time += dt;
        }
    }
};

struct Sprite {
    Sprite() { Size = {2,1}; }
    Pocket::Property<Pocket::Vector2> Size;
};

struct SpriteSystem : GameSystem<Pocket::Mesh, Sprite> {
    void ObjectAdded(GameObject* object) {
        std::cout << "SpriteSystem::ObjectAdded"<<std::endl;
        object->GetComponent<Sprite>()->Size.Changed.Bind(this, &SpriteSystem::SizeChanged, object);
    }

    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Sprite>()->Size.Changed.Unbind(this, &SpriteSystem::SizeChanged, object);
    }
    
    void SizeChanged(GameObject* object) {
        auto mesh = object->GetComponent<Pocket::Mesh>();
        auto sprite = object->GetComponent<Sprite>();
        
        auto& vertices = mesh->GetMesh<Pocket::Vertex>().vertices;
        auto& triangles = mesh->GetMesh<Pocket::Vertex>().triangles;
        
        
        vertices.clear();
        vertices.resize(4);
        vertices[0].Position = {0,0,0};
        vertices[1].Position = {sprite->Size().x,0,0};
        vertices[2].Position = {sprite->Size().x,sprite->Size().y,0};
        vertices[3].Position = {0,sprite->Size().y,0};
        
        triangles.clear();
        triangles = {0,1,2,0,2,3};
        
    }
    
};

struct ScriptTransformSystem : GameSystem<Pocket::Transform, Pocket::Sizeable> {
    
    void ObjectAdded(GameObject* object) {
        std::cout << "ScriptTransformSystem::ObjectAdded"<<std::endl;
    }
        
    void ObjectRemoved(GameObject* object) {
        std::cout << "ScriptTransformSystem::ObjectRemoved"<<std::endl;
    }

    void Update(float dt) {
       for(auto o : Objects()) {
          //auto s = o->GetComponent<Pocket::Sizeable>()->Size();
          //s.x = o->GetComponent<Pocket::Transform>()->Position().x;
          //o->GetComponent<Pocket::Sizeable>()->Size = s;
       }
       //std::cout << Objects()[0]->GetComponent<Sprite>()->width<<std::endl;
    }
};

struct JumpSystem : GameSystem<Pocket::Transform, Jumpable> {
    
    void ObjectAdded(GameObject* object) {
        std::cout << "JumpSystem::ObjectAdded : JumpHeight "<< object->GetComponent<Jumpable>()->JumpHeight <<std::endl;
        object->GetComponent<Jumpable>()->JumpHeight = 10;
        std::cout << "JumpSystem::ObjectAdded : JumpHeight "<< object->GetComponent<Jumpable>()->JumpHeight <<std::endl;
    }
        
    void ObjectRemoved(GameObject* object) {
        std::cout << "JumpSystem::ObjectRemoved"<<std::endl;
    }

    void Update(float dt) {
        
        static float timer = 0;
        timer += dt;
        
        for(auto o : Objects()) {
            auto j = o->GetComponent<Jumpable>();
            auto t = o->GetComponent<Pocket::Transform>();
            
            Pocket::Vector3 p = t->Position;
            p.y = sinf(timer) * j->JumpHeight * 0.1f;
            t->Position = p;
        }
       //std::cout << Objects()[0]->GetComponent<Sprite>()->width<<std::endl;
    }
    

};

struct ColorCycler {
    float speed;
    float phase;
    float vertexOffset;
};

struct ColorCyclerSystem : public GameSystem<Pocket::Mesh, ColorCycler> {
    void Update(float dt) {
       for(auto o : Objects()) {
            auto c = o->GetComponent<ColorCycler>();           
            auto m = o->GetComponent<Pocket::Mesh>();
            auto& vertices = m->GetMesh<Pocket::Vertex>().vertices;
            
            for(int i = 0; i<vertices.size(); ++i) {
            //    vertices[i].Color = Pocket::Colour::HslToRgb(i * c->vertexOffset + c->phase,1.0, 1.0, 1.0);
            }
            c->phase += c->speed * dt;
            
       } 
    }  
};

struct ClickColorer {
       ClickColorer() {
           downColor = Pocket::Colour::Yellow();
           upColor = Pocket::Colour::Blue();
       }
  Pocket::Colour downColor;
  Pocket::Colour upColor;  
};

struct ClickColorerSystem : public GameSystem<Pocket::Touchable, Pocket::Mesh, ClickColorer> {
  
    void ObjectAdded(GameObject* object) {
        object->GetComponent<Pocket::Touchable>()->Down.Bind(this, &ClickColorerSystem::Down, object);
        object->GetComponent<Pocket::Touchable>()->Up.Bind(this, &ClickColorerSystem::Up, object);
    }  
    
    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Pocket::Touchable>()->Down.Unbind(this, &ClickColorerSystem::Down, object);
        object->GetComponent<Pocket::Touchable>()->Up.Unbind(this, &ClickColorerSystem::Up, object);
    }
    
    void Down(Pocket::TouchData d, GameObject* object) {
        auto mesh = object->GetComponent<Pocket::Mesh>();
        mesh->GetMesh<Pocket::Vertex>().SetColor(object->GetComponent<ClickColorer>()->downColor);
    }
    
     void Up(Pocket::TouchData d, GameObject* object) {
        auto mesh = object->GetComponent<Pocket::Mesh>();
        mesh->GetMesh<Pocket::Vertex>().SetColor(object->GetComponent<ClickColorer>()->upColor);
    }
    
};