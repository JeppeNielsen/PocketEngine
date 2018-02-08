//
//  GameObjectJsonSerializer.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 29/12/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObjectSerializer.hpp"
#include "minijson_writer.hpp"
#include "minijson_reader.hpp"

namespace Pocket {
    class GameObjectJsonSerializer : public GameObjectSerializer {
    public:
        GameObject* Deserialize(GameObject* object, std::istream &stream, const std::function<void (GameObject *)>& objectCreated = nullptr) override;
        bool Serialize(GameObject* object, std::ostream& stream, const SerializePredicate& predicate = nullptr) override;
        void SerializeComponents(const std::vector<GameObject*> objects, std::ostream& stream, const SerializePredicate& predicate = nullptr) override;
        void DeserializeAndAddComponents(const std::vector<GameObject *> objects, std::istream &stream) override;

        static std::string ReadGuidFromJson(std::istream& jsonStream);
        
    private:
        void WriteJson(GameObject* object, minijson::object_writer& writer, const SerializePredicate& predicate) const;
        void WriteJsonComponents(GameObject* object,minijson::object_writer& writer, const SerializePredicate& predicate) const;
        void SerializeComponent(GameObject* object,int componentID, minijson::array_writer& writer, bool isReference, const GameObject* referenceObject ) const;
        
        
        void AddComponent(GameObject* object, AddReferenceComponentList& addReferenceComponents, minijson::istream_context& context, const std::string& componentName);
        bool GetAddReferenceComponent(AddReferenceComponentList& addReferenceComponents, Pocket::GameObject **object, int &componentID, GameObject** referenceObject);
        void EndGetAddReferenceComponent();
        
    
        void TryParseJsonObject(int parent, minijson::istream_context &context, const std::string& componentName,
                                const std::function<void (int, int)>& callback,
                                const std::function<bool (const std::string& componentName)>& componentCallback);
        void LoadObject(AddReferenceComponentList& addReferenceComponents, GameObject* object, minijson::istream_context &context, const std::function<void(GameObject*)>& objectCreated);
        
    };
}
