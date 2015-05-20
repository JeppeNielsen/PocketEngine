//
//  testSpine.cpp
//  Platformer
//
//  Created by Jeppe Nielsen on 01/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "Atlas.hpp"
#include "SpineFactory.hpp"
#include "Timeline.hpp"
#include "Timer.hpp"

using namespace Pocket;

class TestSpine : public GameState<TestSpine> {
public:
    GameWorld world;
    RenderSystem* renderer;
    GameObject* camera;
    GameObject* cube;
    float rotation;
    GameObject* atlas;
    GameObject* skeleton;
    GameObject* spineBoy;
    SpineAnimationCollection animations;
    Timeline<std::string> timeline;
    SpineAnimationCollection::iterator currentAnimation;
    
    void Initialize() {
        
        renderer = world.CreateSystem<RenderSystem>();
        world.CreateSystem<FirstPersonMoverSystem>()->Input = &Input;
        SpineFactory* spineFactory = world.CreateFactory<SpineFactory>();
        
        camera = world.CreateObject();
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = { 0, 4, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 60;
        //camera->GetComponent<Camera>()->Orthographic = true;
        //float aspect = 1.0f/Manager().Viewport().Aspect();
        //camera->GetComponent<Camera>()->Viewport = {-25,25*aspect,25,-25*aspect};
        camera->AddComponent<FirstPersonMover>();
        
        rotation = 0;
        
        atlas = world.CreateObject();
        if (!atlas->AddComponent<Atlas>()->LoadSpineAtlas("spineboy.atlas")) {
            std::cout<<"Spine atlas failed"<<std::endl;
        }
        atlas->AddComponent<TextureComponent>()->Texture().LoadFromPng("spineboy.png");
        
        skeleton = spineFactory->LoadSkeleton("spineboy.json");
        if (!skeleton) {
             std::cout<<"Spine skeleton failed"<<std::endl;
        } else {
            std::cout<<"Spine skeleton SUCCESS"<<std::endl;
        }
        
        
        if (!spineFactory->LoadAnimations("spineboy.json", animations)) {
            std::cout<<"Spine animations failed"<<std::endl;
        }else {
            std::cout<<"Spine animations SUCCESS"<<std::endl;
        }
        
        
        currentAnimation = animations.find("walk");
        
        spineBoy = spineFactory->CreateModel(atlas, skeleton);
        
        
        for (int x=1; x<50; x++) {
        //++currentAnimation;
        if (currentAnimation==animations.end()) currentAnimation = animations.begin();
            GameObject* clone = spineBoy->Clone();
            clone->GetComponent<Transform>()->Position = {(x % 15)*5.0f,(x / 15)*7.0f,0};
            clone->GetComponent<Transform>()->Scale = 1;
        }
        
        spineBoy->GetComponent<SpineAnimator>()->Play(&currentAnimation->second);
       
        
        timeline.Add(0, "Zero");
        timeline.Add(1, "One");
        timeline.Add(2, "Two");
        timeline.Add(3, "Three");
        
        TestTimeline(0);
        TestTimeline(0.5f);
        TestTimeline(1.0f);
        TestTimeline(1.95f);
        TestTimeline(2.4f);
        TestTimeline(100.0f);
        
        
        Input.ButtonDown += event_handler(this, &TestSpine::ButtonDown);
    }
    
    void ButtonDown(std::string button) {
        if (button == "a") {
            ++currentAnimation;
            if (currentAnimation==animations.end()) {
                currentAnimation = animations.begin();
            }
            spineBoy->GetComponent<SpineAnimator>()->Play(&currentAnimation->second);
        }
    }
    
    void TestTimeline(float time) {
        std::string* source;
        std::string* dest;
        float t;
        timeline.Get(time, &source, &dest, t);
        std::cout<<"time:"<<time<< ",  source: "<<*source<<", dest: "<<*dest<< ", t:"<<t<<std::endl;
    }
    
    Timer timer;
    Timer renderTimer;
    
    void Update(float dt) {
        //spineBoy->GetComponent<SpineModel>()->SetImage("front_fist", closed ? "front_fist_closed" : "front_fist_open");
        //spineModel->bones["front_fist"].transform->Rotation = rotation;
        //spineBoy->GetComponent<Transform>()->Scale = 0.5f+sinf(rotation)*0.5f;
        rotation += dt;
        
        timer.Begin();
        world.Update(dt);
        double time = timer.End();
        std::cout<<"Update time: " << 1.0 / time << "    "<<std::endl;
    }
    
    void Render() {
        renderTimer.Begin();
        renderer->Render();
        double time = renderTimer.End();
        std::cout<<"Render time: " << 1.0 / time << "    "<<std::endl;
    }
};

/*
int main() {
    Engine e;
    e.Start<TestSpine>();
	return 0;
}
*/
