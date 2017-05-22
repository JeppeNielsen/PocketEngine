//
//  SerializationTests.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 02/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "SerializationTests.hpp"
#include "GameSystem.hpp"
#include "GameObjectHandle.hpp"

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

struct Cloner {
    GameObjectHandle Source;
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(Source)
    TYPE_FIELDS_END
};

struct VelocitySystem : public GameSystem<Transform, Velocity> {

};

struct RenderSystem : public GameSystem<Transform, Mesh, Renderable> {

};

struct ClonerSystem : public GameSystem<Cloner> {

};

void LogStream(const std::string& text, const std::stringstream& s) {
    std::cout << text << ":" <<std::endl;
    std::cout << s.str() << std::endl<<std::endl;
}

void SerializationTests::RunTests() {

    
    AddTest("Serialize gameobject id", [] {
        
        GameWorld world;
        GameObject* root = world.CreateRoot();
        root->CreateSystem<VelocitySystem>();
        
        GameObject* child = root->CreateChild();
        
        int rootId1 = root->RootId();
        int childId1 = child->RootId();
        
        std::stringstream savedWorld;
        root->ToJson(savedWorld);
        LogStream("Before", savedWorld);
    
        GameWorld world2;
        GameObject* root2 = world2.CreateRootFromJson(savedWorld, [] (GameObject* root) {
            root->CreateSystem<VelocitySystem>();
        });
        
        int rootId2 = root2->RootId();
        int childId2 = root2->Children()[0]->RootId();
        
        std::stringstream savedWorld2;
        root2->ToJson(savedWorld2);
        LogStream("After", savedWorld2);
        
        return rootId1 == rootId2 && childId1 == childId2 && savedWorld.str() == savedWorld2.str();
    });
    
     AddTest("Reference Component same root", [] {
        
        GameWorld world;
        GameObject* root = world.CreateRoot();
        root->CreateSystem<RenderSystem>();
        
        GameObject* meshObject = root->CreateChild();
        
        auto& verts = meshObject->AddComponent<Mesh>()->vertices;
        for (int i=0; i<10; ++i) {
            verts.push_back(i);
        }
        
        GameObject* clonedObject = root->CreateChild();
        
        clonedObject->AddComponent<Mesh>(meshObject);
        
        std::stringstream savedRoot;
        
        root->ToJson(savedRoot);
        
        LogStream("Root", savedRoot);
        
        GameWorld loadWorld;
        
        GameObject* loadedRoot = loadWorld.CreateRootFromJson(savedRoot, [](GameObject* root) {
            root->CreateSystem<RenderSystem>();
        });
        Mesh* mesh = loadedRoot->Children()[0]->GetComponent<Mesh>();
        Mesh* mesh2 = loadedRoot->Children()[1]->GetComponent<Mesh>();
    
        return mesh == mesh2 && mesh->vertices.size() == 10;
    });
    

    AddTest("Reference Component Different roots", [] {
        
        GameWorld world;
        GameObject* meshRoot = world.CreateRoot();
        meshRoot->CreateSystem<RenderSystem>();
        auto& verts = meshRoot->AddComponent<Mesh>()->vertices;
        for (int i=0; i<10; ++i) {
            verts.push_back(i);
        }
        
        GameObject* objectRoot = world.CreateRoot();
        objectRoot->CreateSystem<RenderSystem>();
        
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
        
        GameObject* loadedMeshRoot = loadWorld.CreateRootFromJson(savedMeshRoot, [](GameObject* root) { root->CreateSystem<RenderSystem>(); });
        GameObject* loadedWorldRoot = loadWorld.CreateRootFromJson(savedObjectRoot, [](GameObject* root) { root->CreateSystem<RenderSystem>(); });
        
        Mesh* mesh1 = loadedMeshRoot->GetComponent<Mesh>();
        Mesh* mesh2 = loadedWorldRoot->Children()[0]->GetComponent<Mesh>();
        
        return mesh1 == mesh2 && mesh2->vertices.size() == 10;
    });
    
    AddTest("Reference Component, source removed", [] () {
        GameWorld world;
        
        GameObject* root = world.CreateRoot();
        root->CreateSystem<RenderSystem>();
        GameObject* mesh = root->CreateChild();
        GameObject* reference = root->CreateChild();
        
        mesh->AddComponent<Mesh>()->vertices.resize(5);
        reference->AddComponent<Mesh>(mesh);
        
        mesh->Remove();
        
        world.Update(0);
        
        std::stringstream savedRoot;
        root->ToJson(savedRoot);
        
        LogStream("", savedRoot);
        
        GameWorld loadWorld;
        
        GameObject* loadedRoot = loadWorld.CreateRootFromJson(savedRoot, [](GameObject* root) { root->CreateSystem<RenderSystem>(); });
        Mesh* mesh1 = loadedRoot->Children()[0]->GetComponent<Mesh>();
        return mesh1->vertices.size() == 5;
    });
    
    
    AddTest("GameObjectHandle", [] () {
        
        GameWorld world;
    
        GameObject* root = world.CreateRoot();
        root->CreateSystem<ClonerSystem>();
        root->CreateSystem<RenderSystem>();
        
        GameObject* cubePrefab = root->CreateChild();
        cubePrefab->AddComponent<Transform>();
        cubePrefab->AddComponent<Mesh>();
        cubePrefab->AddComponent<Renderable>();
        
        std::stringstream savedCloner;
    
        GameObject* clonerPrefab = root->CreateChild();
        clonerPrefab->AddComponent<Cloner>()->Source = cubePrefab;
        clonerPrefab->ToJson(savedCloner);
    
        LogStream("Clone", savedCloner);
        
        GameObject* loadedCloner = root->CreateChildFromJson(savedCloner);
        Cloner* cloner = loadedCloner->GetComponent<Cloner>();
        cloner->Source.SetRoot(root);
        return cloner->Source() == cubePrefab;
    });
    
    
    AddTest("GameObjectHandle to saved root", [] () {
        
        std::stringstream savedPrefab;
        std::stringstream savedCloner;
        std::string prefabGuid;
        
        {
            GameWorld world;
        
            GameObject* cubePrefab = world.CreateRoot();
            cubePrefab->CreateSystem<ClonerSystem>();
            cubePrefab->CreateSystem<RenderSystem>();
            
            prefabGuid = cubePrefab->RootGuid();
            
            cubePrefab->AddComponent<Transform>()->x = 123;
            cubePrefab->GetComponent<Transform>()->y = 456;
            
            cubePrefab->AddComponent<Mesh>();
            cubePrefab->AddComponent<Renderable>();
            
            cubePrefab->ToJson(savedPrefab);
            
            
            GameObject* cloner = world.CreateRoot();
            cloner->AddComponent<Cloner>()->Source = cubePrefab;
            
            cloner->ToJson(savedCloner);
        }
        
        LogStream("Prefab", savedPrefab);
        LogStream("Cloner", savedCloner);
        
        GameWorld world;
        
        world.GuidToRoot = [&] (const std::string& guid) -> GameObject* {
            if (guid == prefabGuid) {
                return world.CreateRootFromJson(savedPrefab, [] (GameObject* root) {
                    root->CreateSystem<RenderSystem>();
                    root->CreateSystem<ClonerSystem>();
                });
            }
            return 0;
        };
        
        GameObject* clone = world.CreateRootFromJson(savedCloner, [](GameObject* root) {
            root->CreateSystem<ClonerSystem>();
            root->CreateSystem<RenderSystem>();
        });
        
        std::stringstream s;
        clone->GetComponent<Cloner>()->Source.SetRoot(clone);
        clone->GetComponent<Cloner>()->Source->ToJson(s);
        LogStream("Cloner Source", s);

        return clone->GetComponent<Cloner>()->Source->GetComponent<Transform>()->x == 123;
    });

    
    AddTest("Read Guid from json", [] () {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        std::stringstream stream;
        root->ToJson(stream);
        std::string loadedGuid = world.ReadGuidFromJson(stream);
        return loadedGuid == root->RootGuid();
    });
    
    
    AddTest("Serialize Range", [] () {
        
        GameWorld world;
    
        GameObject* root = world.CreateRoot();
        root->CreateSystem<ClonerSystem>();
        root->CreateSystem<RenderSystem>();
        root->AddComponent<Renderable>()->imageNo = 53;
        
        std::vector<GameObject*> objects;
        
        for (int i = 0; i<1; i++) {
            GameObject* cubePrefab = root->CreateChild();
            cubePrefab->AddComponent<Transform>();
            cubePrefab->AddComponent<Mesh>();
            cubePrefab->AddComponent<Renderable>()->imageNo = i;
            objects.push_back(cubePrefab);
        }
        
        std::stringstream savedCloner;
    
        int componentToDelete = GameIdHelper::GetComponentID<Renderable>();
    
        world.SerializeAndRemoveComponents(savedCloner, [componentToDelete] (const GameObject* go, int componentId)  {
            return componentId == componentToDelete;
        });
        
        LogStream("Clone", savedCloner);
        
        world.DeserializeAndAddComponents(savedCloner);
        
        int index = 0;
        for(auto o : objects) {
            if (o->GetComponent<Renderable>()->imageNo != index) return false;
            index++;
        }
        return true;
    });
}

