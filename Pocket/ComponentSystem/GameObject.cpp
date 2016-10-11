//
//  GameObject.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 06/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObject.hpp"
#include <assert.h>
#include "GameWorld.hpp"
#include <map>
#include <set>

using namespace Pocket;

static bool forceSetNextParent = false;

const ObjectCollection& GameObject::Children() const { return data->children; }
Property<GameObject*>& GameObject::Parent() { return data->Parent; }
Property<bool>& GameObject::Enabled() { return data->Enabled; }
DirtyProperty<bool>& GameObject::WorldEnabled() { return data->WorldEnabled; }
Property<int>& GameObject::Order() { return data->Order; }

GameObject::GameObject()
    :
    world(0), index(0), data(0)
{
    data = new Data();
    data->Reset();
    data->Parent = 0;
    
    data->Parent.Changed.Bind([this]() {
        assert(data->Parent!=this);
        GameObject* prevParent = data->Parent.PreviousValue();
        GameObject* currentParent = data->Parent;
        
        if (data->removed && !forceSetNextParent) return;
        
        if (!prevParent) {
            prevParent = &world->root;
        }
        if (!currentParent && !forceSetNextParent) {
            currentParent = &world->root;
        }
        
        if (prevParent) {
            auto& children = prevParent->data->children;
            children.erase(std::find(children.begin(), children.end(), this));
            prevParent->data->WorldEnabled.HasBecomeDirty.Unbind(this, &GameObject::SetWorldEnableDirty);
        }
        
        if (currentParent) {
            auto& children = currentParent->data->children;
            children.push_back(this);
            currentParent->data->WorldEnabled.HasBecomeDirty.Bind(this, &GameObject::SetWorldEnableDirty);
            
            bool prevWorldEnabled = data->WorldEnabled;
            data->WorldEnabled.MakeDirty();
            if (data->WorldEnabled()!=prevWorldEnabled) {
                SetWorldEnableDirty();
            }
        }
    });
    
    data->WorldEnabled.Method = [this](bool& value) {
        value = (data->Parent) ? data->Parent()->data->WorldEnabled && data->Enabled : data->Enabled;
    };
    
    data->Enabled.Changed.Bind(this, &GameObject::SetWorldEnableDirty);
}

GameObject::~GameObject() {
    delete data;
}

bool GameObject::HasComponent(ComponentID id) const {
    assert(id<world->numComponentTypes);
    return data->activeComponents[id];
}

void* GameObject::GetComponent(ComponentID id) {
    assert(id<world->numComponentTypes);
    if (!data->activeComponents[id]) return 0;
    auto& objectComponent = world->objectComponents[id][index];
    return objectComponent.container->Get(objectComponent.index);
}

void GameObject::AddComponent(ComponentID id) {
    assert(id<world->numComponentTypes);
    if (HasComponent(id)) {
        return;
    }
    if (data->removed) return;
    IContainer* container = world->components[id];
    world->objectComponents[id][index] = { container->Create(this), container };
    data->activeComponents.Set(id, true);
    world->delayedActions.emplace_back([this, id]() {
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::AddComponent(ComponentID id, GameObject* source) {
    assert(id<world->numComponentTypes);
    assert(source->HasComponent(id));
    if (HasComponent(id)) {
        return;
    }
    if (data->removed) return;
    auto& sourceObjectComponent = source->world->objectComponents[id][source->index];
    world->objectComponents[id][index] = sourceObjectComponent;
    sourceObjectComponent.container->Reference(sourceObjectComponent.index);
    data->activeComponents.Set(id, true);
    world->delayedActions.emplace_back([this, id]() {
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::CloneComponent(ComponentID id, GameObject* source) {
    assert(id<world->numComponentTypes);
    assert(source->HasComponent(id));
    if (HasComponent(id)) {
        return;
    }
    if (data->removed) return;
    auto& sourceObjectComponent = source->world->objectComponents[id][source->index];
    IContainer* container = world->components[id];
    int componentIndex = 0;
    if (container == sourceObjectComponent.container) {
        componentIndex = container->Clone(this, sourceObjectComponent.index);
    } else {
        componentIndex = container->Clone(this, sourceObjectComponent.container->Get(sourceObjectComponent.index));
    }
    world->objectComponents[id][index] = { componentIndex, container };
    data->activeComponents.Set(id, true);
    world->delayedActions.emplace_back([this, id]() {
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::RemoveComponent(ComponentID id) {
    if (id>=world->numComponentTypes) return;
    if (!HasComponent(id)) {
        return;
    }
    if (data->removed) return;
    world->delayedActions.emplace_back([this, id]() {
        if (!data->activeComponents[id]) {
           return; // might have been removed by earlier remove action, eg if two consecutive RemoveComponent<> was called
        }
        TrySetComponentEnabled(id, false);
        auto& objectComponent = world->objectComponents[id][index];
        objectComponent.container->Delete(objectComponent.index);
        world->objectComponents[id][index].index = -1;
        data->activeComponents.Set(id, false);
    });
}

void GameObject::Remove() {
    if (data->removed) return;
    int localIndex = index;
    world->delayedActions.emplace_back([this, localIndex]() {
        forceSetNextParent = true;
        if (data->Parent()) {
            data->Parent = 0;
        } else {
            auto& children = world->root.data->children;
            children.erase(std::find(children.begin(), children.end(), this));
        }
        forceSetNextParent = false;
        SetEnabled(false);
        world->objectsFreeIndicies.push_back(localIndex);
        --world->objectCount;
    });
    data->removed = true;
    for(auto child : data->children) {
        child->Remove();
    }
}

bool GameObject::IsRemoved() {
    return data->removed;
}

std::vector<TypeInfo> GameObject::GetComponentTypes(std::function<bool(int componentID)> predicate) {
    std::vector<TypeInfo> infos;
    for (int i=0; i<world->components.size(); ++i) {
        if (!world->componentInfos[i].getTypeInfo) continue; // component has no type
        if (!data->activeComponents[i]) continue; // gameobject hasn't got component
        if (predicate && !predicate(i)) continue; // component type not allowed
        infos.emplace_back(world->componentInfos[i].getTypeInfo(this));
        infos[infos.size()-1].name = world->componentInfos[i].name;
    }
    return infos;
}

std::vector<int> GameObject::GetComponentIndicies() {
    std::vector<int> indicies;
    for (int i=0; i<world->components.size(); ++i) {
        if (data->activeComponents[i]) {
            indicies.push_back(i);
        }
    }
    return indicies;
}

//SERIALIZATION


std::map<GameObject*, std::string> serializedObjects;

struct AddReferenceComponent {
    GameObject* object;
    int componentID;
    std::string referenceID;
};

std::vector<AddReferenceComponent> addReferenceComponents;

void RecurseTree(GameObject* object, std::map<GameObject*, std::string>& objects) {
    std::string id = object->GetID();
    if (id == "") {
        std::stringstream s;
        s<<(objects.size() + 1);
        id = s.str();
    }
    objects[object] = id;
    for(auto s : object->Children()) {
        RecurseTree(s, objects);
    }
}

struct CloneReferenceComponent {
    GameObject* object;
    int componentID;
    GameObject* referencedObject;
};

std::set<GameObject*> clonedSourceObjects;
std::vector<CloneReferenceComponent> clonedReferenceComponents;
std::map<GameObject*, GameObject*> sourceToClonedObjects;

void RecurseTree(GameObject* object, std::set<GameObject*>& objects) {
    objects.insert(object);
    for(auto s : object->Children()) {
        RecurseTree(s, objects);
    }
}

void GameObject::ToJson(std::ostream& stream, SerializePredicate predicate) {
    serializedObjects.clear();
    RecurseTree(this, serializedObjects);

    minijson::writer_configuration config;
    config = config.pretty_printing(true);
    minijson::object_writer writer(stream, config);
    WriteJson(writer, predicate);
    writer.close();
}

void GameObject::WriteJson(minijson::object_writer& writer, SerializePredicate predicate) {

    bool isRoot = this==world->Root();

    minijson::object_writer gameObject = writer.nested_object(!isRoot ? "GameObject" : "World");
    
    if (!isRoot) {
        gameObject.write("id", serializedObjects[this]);
        minijson::array_writer components = gameObject.nested_array("Components");
        
        if (data->activeComponents.Size()>0) {
            for(int i=0; i<world->components.size(); ++i) {
                if (data->activeComponents[i] && !(predicate && !predicate(this, i))) {
                    GameWorld::ObjectComponent& objectComponent = world->objectComponents[i][index];
                    GameObject* componentOwner = objectComponent.container->GetOwner(objectComponent.index);
                    bool isReference =  componentOwner != this;
                    if (isReference) {
                        if (predicate && !predicate(componentOwner, i)) {
                            continue;
                        }
                    }
                    SerializeComponent(i, components, isReference, componentOwner);
                }
            }
        }
        components.close();
    }
    
    if (!data->children.empty()) {
        minijson::array_writer children_object = gameObject.nested_array("Children");
        for(auto child : data->children) {
            if (predicate && !predicate(child, -1)) {
                continue;
            }
            GameObject* childSpecific = (GameObject*)child;
            minijson::object_writer child_object = children_object.nested_object();
            childSpecific->WriteJson(child_object, predicate);
            child_object.close();
        }
        children_object.close();
    }
    
    gameObject.close();
}

void GameObject::SerializeComponent(int componentID, minijson::array_writer& writer, bool isReference, GameObject* referenceObject ) {
    minijson::object_writer componentWriter = writer.nested_object();
    ComponentInfo& componentInfo = world->componentInfos[componentID];
    
    if (!isReference) {
        minijson::object_writer jsonComponent = componentWriter.nested_object(componentInfo.name.c_str());
        if (componentInfo.getTypeInfo) {
            auto type = componentInfo.getTypeInfo(this);
            type.Serialize(jsonComponent);
        }
        jsonComponent.close();
    } else {
        std::string referenceName = componentInfo.name + ":ref";
        minijson::object_writer jsonComponent = componentWriter.nested_object(referenceName.c_str());
        if (!referenceObject) {
            jsonComponent.write("id", "");
        } else {
            std::string id = serializedObjects[referenceObject];
            if (id=="") {
                std::string* idFromObject = world->FindIDFromReferenceObject(this, componentID);
                id = idFromObject ? *idFromObject : "";
            }
            jsonComponent.write("id", id);
        }
        jsonComponent.close();
    }
    componentWriter.close();
}

void GameObject::AddComponent(minijson::istream_context& context, std::string componentName) {
    int componentID;
    bool isReference;
    if (world->TryGetComponentIndex(componentName, componentID, isReference) && !data->activeComponents[componentID]) {
        if (!isReference) {
            AddComponent(componentID);
            ComponentInfo& componentInfo = world->componentInfos[componentID];
            if (componentInfo.getTypeInfo) {
                auto type = componentInfo.getTypeInfo(this);
                type.Deserialize(context);
            } else {
                minijson::ignore(context);
            }
        } else {
            std::string referenceID = "";
            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                std::string id = n;
                if (id == "id" && v.type()==minijson::String) {
                    referenceID = v.as_string();
                    addReferenceComponents.push_back({ this, componentID, referenceID });
                } else {
                    minijson::ignore(context);
                }
            });
        }
    } else {
        minijson::ignore(context);
    }
}

//END SERIALIZATION

void GameObject::SetID(std::string id) {
    world->AddObjectID(this, id);
}

std::string GameObject::GetID() {
    std::string* id = world->GetObjectID(this);
    return id ? *id : "";
}

GameObject* GameObject::Clone(GameObject* parent, std::function<bool(GameObject*)> predicate) {
    clonedSourceObjects.clear();
    RecurseTree(this, clonedSourceObjects);
    clonedReferenceComponents.clear();
    sourceToClonedObjects.clear();
    GameObject* clone = CloneInternal(parent, predicate);
    for(auto& c : clonedReferenceComponents) {
        c.object->AddComponent(c.componentID, sourceToClonedObjects[c.referencedObject]);
    }
    return clone;
}


void GameObject::TryAddComponentContainer(ComponentID id, std::function<IContainer *(GameObject::ComponentInfo&)>&& constructor) {
    world->TryAddComponentContainer(id, std::move(constructor));
}

void GameObject::SetWorldEnableDirty() {
    data->WorldEnabled.MakeDirty();
    world->delayedActions.emplace_back([this](){
        SetEnabled(data->WorldEnabled);
    });
}

void GameObject::SetEnabled(bool enabled) {
    for(int i=0; i<world->numComponentTypes; ++i) {
        if (data->activeComponents[i]) {
            TrySetComponentEnabled(i, enabled);
        }
    }
}

void GameObject::TrySetComponentEnabled(ComponentID id, bool enable) {
    
    enable = enable && WorldEnabled();

    bool isEnabled = data->enabledComponents[id];
    if (isEnabled==enable) {
        return; //cannot double enable/disable components
    }
    
    if (enable) {
        data->enabledComponents.Set(id, true);
        if (id>=world->systemsPerComponent.size()) return; // component id is beyond systems
        auto& systemsUsingComponent = world->systemsPerComponent[id];
        for(auto systemIndex : systemsUsingComponent) {
            auto& systemEntry = world->systemsIndexed[systemIndex];
            bool isInterest = systemEntry.bitset.Contains(data->enabledComponents);
            if (isInterest) {
                systemEntry.system->AddObject(this);
                systemEntry.system->ObjectAdded(this);
            }
        }
    } else {
        if (id>=world->systemsPerComponent.size()) return; // component id is beyond systems
        auto& systemsUsingComponent = world->systemsPerComponent[id];
        for(auto systemIndex : systemsUsingComponent) {
            auto& systemEntry = world->systemsIndexed[systemIndex];
            bool wasInterest = systemEntry.bitset.Contains(data->enabledComponents);
            if (wasInterest) {
                systemEntry.system->ObjectRemoved(this);
                systemEntry.system->RemoveObject(this);
            }
        }
        data->enabledComponents.Set(id, false);
    }
}

bool GameObject::GetAddReferenceComponent(Pocket::GameObject **object, int &componentID, std::string &referenceID) {
    if (addReferenceComponents.empty()) return false;
    auto& refObj = addReferenceComponents.back();
    addReferenceComponents.pop_back();
    *object = refObj.object;
    componentID = refObj.componentID;
    referenceID = refObj.referenceID;
    return true;
}

GameObject* GameObject::CloneInternal(GameObject* parent, std::function<bool(GameObject*)> predicate) {
    if (predicate && !predicate(this)) {
        return 0;
    }
    GameObject* clone = world->CreateObject();
    clone->Parent() = parent;
    for(int i=0; i<world->components.size();++i) {
        if (data->activeComponents[i]) {
            GameWorld::ObjectComponent& objectComponent = world->objectComponents[i][index];
            GameObject* componentOwner = objectComponent.container->GetOwner(objectComponent.index);
            bool isReference =  componentOwner != this;
            if (!isReference) {
                clone->CloneComponent(i, this);
            } else {
                if (clonedSourceObjects.find(componentOwner)==clonedSourceObjects.end()) {
                    clone->AddComponent(i, this);
                } else {
                    clonedReferenceComponents.push_back({ clone, i, componentOwner });
                }
            }
        }
    }
    sourceToClonedObjects[this] = clone;
    for(auto child : data->children) {
        child->CloneInternal(clone, predicate);
    }
    return clone;
}

void GameObject::Data::Reset() {
    activeComponents.Reset();
    enabledComponents.Reset();
    removed = false;
    Enabled = true;
    Order = 0;
}