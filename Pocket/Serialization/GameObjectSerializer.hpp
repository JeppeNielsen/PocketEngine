//
//  GameObjectSerializer.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 29/12/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <istream>
#include <ostream>
#include <functional>
#include <string>

namespace Pocket {
    class GameObject;
    class GameScene;
    
    using SerializePredicate = std::function<bool(const GameObject*, int)>;
    
    class GameObjectSerializer {
    public:
        virtual ~GameObjectSerializer() {}
        virtual GameObject* Deserialize(GameObject* object, std::istream &stream, const std::function<void (GameObject *)>& objectCreated = nullptr) = 0;
        virtual bool Serialize(GameObject* object, std::ostream& stream, const SerializePredicate& predicate = nullptr) = 0;
        
        virtual void SerializeComponents(const std::vector<GameObject*> objects, std::ostream& stream, const SerializePredicate& predicate = nullptr) = 0;
        virtual void DeserializeAndAddComponents(const std::vector<GameObject *> objects, std::istream &stream) = 0;

    protected:
        struct AddReferenceComponent {
            GameObject* object;
            int componentID;
            std::string referenceId;
        };
        
        using AddReferenceComponentList = std::vector<AddReferenceComponent>;
    };
}
