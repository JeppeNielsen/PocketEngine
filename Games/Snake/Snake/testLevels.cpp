//
//  testLevels.cpp
//  Snake
//
//  Created by Jeppe Nielsen on 04/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "GameState.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "LevelCreator.hpp"
#include "MathHelper.hpp"
#include "Level.hpp"

using namespace Pocket;


struct TestLevels : public GameState<TestLevels> {

    LevelCreator levelCreator;
    GameWorld world;
    Level level;
    
    void CreateQuad(Point p, Colour color) {
        GameObject* object = world.CreateObject();
        object->AddComponent<Transform>()->Position = {(float)p.x, (float)p.y};
        object->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1, color);
        object->AddComponent<Material>();
    }

    void Initialize() {
        world.CreateSystem<RenderSystem>();
        
        GameObject* camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = { 0, 0, 100 };
        camera->AddComponent<Camera>()->FieldOfView = 25;
        
        level.LoadFromPng("Level1.png");
        
        /*
        for(int i=0; i<20; i++) {
            levelCreator.AddCircle({-20+MathHelper::Random(40),-20+MathHelper::Random(40)}, 3+ MathHelper::Random(8));
        }
        */
        for(auto& p : level.blockable) {
            CreateQuad(p, Colour::White());
        }
        
        for(auto& p : level.spawnable) {
            CreateQuad(p, Colour::Green());
        }
        
    }

    void Update(float dt) {
        world.Update(dt);
    }

    void Render() {
        world.Render();
    }
};

int main_nono() {
    Engine e;
    e.Start<TestLevels>();
    return 0;
}