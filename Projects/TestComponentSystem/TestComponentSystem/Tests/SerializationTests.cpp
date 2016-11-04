//
//  SerializationTests.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 02/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "SerializationTests.hpp"
#include "GameSystem.hpp"

using namespace Pocket;

struct Transform {
    int x;
    int y;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELDS_END
};

struct Velocity {
    int x;
    int y;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELDS_END
};

struct Renderable {
    int imageNo;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(imageNo)
    TYPE_FIELDS_END
};

struct Mesh {
    std::vector<int> vertices;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(vertices)
    TYPE_FIELDS_END
};

struct VelocitySystem : public GameSystem<Transform, Velocity> {

};

struct RenderSystem : public GameSystem<Transform, Mesh, Renderable> {

};

void LogStream(const std::string& text, const std::stringstream& s) {
    std::cout << text << ":" <<std::endl;
    std::cout << s.str() << std::endl<<std::endl;
}

void SerializationTests::RunTests() {

    
    AddTest("Serialize gameobject id", [] {
        
        GameWorld world;
        world.AddSystemType<VelocitySystem>();
        GameObject* root = world.CreateRoot();
        GameObject* child = root->CreateChild();
        
        int rootId1 = root->RootId();
        int childId1 = child->RootId();
        
        std::stringstream savedWorld;
        root->ToJson(savedWorld);
        LogStream("Before", savedWorld);
    
        GameWorld world2;
        GameObject* root2 = world2.CreateRootFromJson(savedWorld, [] (GameObject* object) {
            
        });
        
        int rootId2 = root2->RootId();
        int childId2 = root2->Children()[0]->RootId();
        
        std::stringstream savedWorld2;
        root2->ToJson(savedWorld2);
        LogStream("After", savedWorld2);
        
        return rootId1 == rootId2 && childId1 == childId2 && savedWorld.str() == savedWorld2.str();
    });

    AddTest("Reference Component", [] {
        
        GameWorld world;
        world.AddSystemType<RenderSystem>();
        GameObject* meshRoot = world.CreateRoot();
        auto& verts = meshRoot->AddComponent<Mesh>()->vertices;
        for (int i=0; i<10; ++i) {
            verts.push_back(i);
        }
        
        GameObject* objectRoot = world.CreateRoot();
        
        GameObject* cube1 = objectRoot->CreateChild();
        cube1->AddComponent<Transform>();
        cube1->AddComponent<Renderable>();
        cube1->AddComponent<Mesh>(meshRoot);
        
        std::stringstream savedMeshRoot;
        std::stringstream savedObjectRoot;
        
        meshRoot->ToJson(savedMeshRoot);
        objectRoot->ToJson(savedObjectRoot);
        
        LogStream("MeshRoot", savedMeshRoot);
        LogStream("ObjectRoot", savedObjectRoot);
        
        GameWorld loadWorld;
        loadWorld.AddSystemType<RenderSystem>();
        
        GameObject* loadedMeshRoot = loadWorld.CreateRootFromJson(savedMeshRoot, [](GameObject* object) { });
        GameObject* loadedWorldRoot = loadWorld.CreateRootFromJson(savedObjectRoot, [](GameObject* object) { });
        
        Mesh* mesh = loadedWorldRoot->Children()[0]->GetComponent<Mesh>();
    
    
        return mesh->vertices.size() == 10;
    });
    


}

