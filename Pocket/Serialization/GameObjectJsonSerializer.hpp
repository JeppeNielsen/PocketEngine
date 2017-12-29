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
        GameObject* Deserialize(GameScene* scene, std::istream &stream, const std::function<void (GameObject *)>& objectCreated) override;
        bool Serialize(GameObject* object, std::ostream& stream, const SerializePredicate& predicate) override;
        
    private:
        void WriteJson(GameObject* object, minijson::object_writer& writer, const SerializePredicate& predicate) const;
        void WriteJsonComponents(GameObject* object,minijson::object_writer& writer, const SerializePredicate& predicate) const;
        void SerializeComponent(GameObject* object,int componentID, minijson::array_writer& writer, bool isReference, const GameObject* referenceObject ) const;
        
        
        void AddComponent(GameObject* object, AddReferenceComponentList& addReferenceComponents, minijson::istream_context& context, std::string componentName);
        bool GetAddReferenceComponent(AddReferenceComponentList& addReferenceComponents, Pocket::GameObject **object, int &componentID, GameObject** referenceObject);
        void EndGetAddReferenceComponent();
        
        
    private:
        GameObject* LoadObject(AddReferenceComponentList& addReferenceComponents, GameObject* parent, minijson::istream_context &context, const std::function<void(GameObject*)>& objectCreated);
        
    };
}
