namespace Pocket {
struct Transform;
struct Mesh;
struct Material;
struct Camera;
struct TextureComponent;
struct Orderable;
struct Touchable;
struct EditorObject;
struct Rotator;
}
struct Velocity;

class GameObject {
private:
    virtual void* GetComponent(int componentID) = 0;
    virtual void AddComponent(int componentID) = 0;
    virtual void AddComponent(int componentID, GameObject* referenceObject) = 0;
    virtual void RemoveComponent(int componentID) = 0;
    virtual void CloneComponent(int componentID, GameObject* source) = 0;
public:
    template<typename T> T* GetComponent() { return (T*)0; }
    template<typename T> T* AddComponent() { }
    template<typename T> void RemoveComponent() { }
    template<typename T> T* CloneComponent(GameObject* source) { }
};
template<> Pocket::Transform* GameObject::GetComponent<Pocket::Transform>() { return (Pocket::Transform*) GetComponent(0); }
template<> Pocket::Transform* GameObject::AddComponent<Pocket::Transform>() { AddComponent(0); return (Pocket::Transform*) GetComponent(0); }
template<> void GameObject::RemoveComponent<Pocket::Transform>() { RemoveComponent(0); }
template<> Pocket::Transform* GameObject::CloneComponent<Pocket::Transform>(GameObject* source) { CloneComponent(0, source); return (Pocket::Transform*) GetComponent(0); }
template<> Pocket::Mesh* GameObject::GetComponent<Pocket::Mesh>() { return (Pocket::Mesh*) GetComponent(1); }
template<> Pocket::Mesh* GameObject::AddComponent<Pocket::Mesh>() { AddComponent(1); return (Pocket::Mesh*) GetComponent(1); }
template<> void GameObject::RemoveComponent<Pocket::Mesh>() { RemoveComponent(1); }
template<> Pocket::Mesh* GameObject::CloneComponent<Pocket::Mesh>(GameObject* source) { CloneComponent(1, source); return (Pocket::Mesh*) GetComponent(1); }
template<> Pocket::Material* GameObject::GetComponent<Pocket::Material>() { return (Pocket::Material*) GetComponent(2); }
template<> Pocket::Material* GameObject::AddComponent<Pocket::Material>() { AddComponent(2); return (Pocket::Material*) GetComponent(2); }
template<> void GameObject::RemoveComponent<Pocket::Material>() { RemoveComponent(2); }
template<> Pocket::Material* GameObject::CloneComponent<Pocket::Material>(GameObject* source) { CloneComponent(2, source); return (Pocket::Material*) GetComponent(2); }
template<> Pocket::Camera* GameObject::GetComponent<Pocket::Camera>() { return (Pocket::Camera*) GetComponent(3); }
template<> Pocket::Camera* GameObject::AddComponent<Pocket::Camera>() { AddComponent(3); return (Pocket::Camera*) GetComponent(3); }
template<> void GameObject::RemoveComponent<Pocket::Camera>() { RemoveComponent(3); }
template<> Pocket::Camera* GameObject::CloneComponent<Pocket::Camera>(GameObject* source) { CloneComponent(3, source); return (Pocket::Camera*) GetComponent(3); }
template<> Pocket::TextureComponent* GameObject::GetComponent<Pocket::TextureComponent>() { return (Pocket::TextureComponent*) GetComponent(4); }
template<> Pocket::TextureComponent* GameObject::AddComponent<Pocket::TextureComponent>() { AddComponent(4); return (Pocket::TextureComponent*) GetComponent(4); }
template<> void GameObject::RemoveComponent<Pocket::TextureComponent>() { RemoveComponent(4); }
template<> Pocket::TextureComponent* GameObject::CloneComponent<Pocket::TextureComponent>(GameObject* source) { CloneComponent(4, source); return (Pocket::TextureComponent*) GetComponent(4); }
template<> Pocket::Orderable* GameObject::GetComponent<Pocket::Orderable>() { return (Pocket::Orderable*) GetComponent(5); }
template<> Pocket::Orderable* GameObject::AddComponent<Pocket::Orderable>() { AddComponent(5); return (Pocket::Orderable*) GetComponent(5); }
template<> void GameObject::RemoveComponent<Pocket::Orderable>() { RemoveComponent(5); }
template<> Pocket::Orderable* GameObject::CloneComponent<Pocket::Orderable>(GameObject* source) { CloneComponent(5, source); return (Pocket::Orderable*) GetComponent(5); }
template<> Pocket::Touchable* GameObject::GetComponent<Pocket::Touchable>() { return (Pocket::Touchable*) GetComponent(6); }
template<> Pocket::Touchable* GameObject::AddComponent<Pocket::Touchable>() { AddComponent(6); return (Pocket::Touchable*) GetComponent(6); }
template<> void GameObject::RemoveComponent<Pocket::Touchable>() { RemoveComponent(6); }
template<> Pocket::Touchable* GameObject::CloneComponent<Pocket::Touchable>(GameObject* source) { CloneComponent(6, source); return (Pocket::Touchable*) GetComponent(6); }
template<> Pocket::EditorObject* GameObject::GetComponent<Pocket::EditorObject>() { return (Pocket::EditorObject*) GetComponent(14); }
template<> Pocket::EditorObject* GameObject::AddComponent<Pocket::EditorObject>() { AddComponent(14); return (Pocket::EditorObject*) GetComponent(14); }
template<> void GameObject::RemoveComponent<Pocket::EditorObject>() { RemoveComponent(14); }
template<> Pocket::EditorObject* GameObject::CloneComponent<Pocket::EditorObject>(GameObject* source) { CloneComponent(14, source); return (Pocket::EditorObject*) GetComponent(14); }
template<> Pocket::Rotator* GameObject::GetComponent<Pocket::Rotator>() { return (Pocket::Rotator*) GetComponent(27); }
template<> Pocket::Rotator* GameObject::AddComponent<Pocket::Rotator>() { AddComponent(27); return (Pocket::Rotator*) GetComponent(27); }
template<> void GameObject::RemoveComponent<Pocket::Rotator>() { RemoveComponent(27); }
template<> Pocket::Rotator* GameObject::CloneComponent<Pocket::Rotator>(GameObject* source) { CloneComponent(27, source); return (Pocket::Rotator*) GetComponent(27); }
template<> Velocity* GameObject::GetComponent<Velocity>() { return (Velocity*) GetComponent(28); }
template<> Velocity* GameObject::AddComponent<Velocity>() { AddComponent(28); return (Velocity*) GetComponent(28); }
template<> void GameObject::RemoveComponent<Velocity>() { RemoveComponent(28); }
template<> Velocity* GameObject::CloneComponent<Velocity>(GameObject* source) { CloneComponent(28, source); return (Velocity*) GetComponent(28); }
class IGameSystem;
#include "TypeInfo.hpp"
#include "Property.hpp"
#include "Transform.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "ScriptExample.hpp"

#include <string>
#include <vector>
#include "Property.hpp"
template<> struct FieldInfoIndexer<int> { static int Index() { return 0; } };
template<> struct FieldInfoIndexer<std::vector<int>> { static int Index() { return 1; } };
template<> struct FieldInfoIndexer<Pocket::Property<int>> { static int Index() { return 2; } };
template<> struct FieldInfoIndexer<float> { static int Index() { return 3; } };
template<> struct FieldInfoIndexer<std::vector<float>> { static int Index() { return 4; } };
template<> struct FieldInfoIndexer<Pocket::Property<float>> { static int Index() { return 5; } };
template<> struct FieldInfoIndexer<double> { static int Index() { return 6; } };
template<> struct FieldInfoIndexer<std::vector<double>> { static int Index() { return 7; } };
template<> struct FieldInfoIndexer<Pocket::Property<double>> { static int Index() { return 8; } };
template<> struct FieldInfoIndexer<std::__1::basic_string<char>> { static int Index() { return 9; } };
template<> struct FieldInfoIndexer<std::vector<std::__1::basic_string<char>>> { static int Index() { return 10; } };
template<> struct FieldInfoIndexer<Pocket::Property<std::__1::basic_string<char> >> { static int Index() { return 11; } };
template<> struct FieldInfoIndexer<Pocket::Vector3> { static int Index() { return 12; } };
template<> struct FieldInfoIndexer<std::vector<Pocket::Vector3>> { static int Index() { return 13; } };
template<> struct FieldInfoIndexer<Pocket::Property<Pocket::Vector3>> { static int Index() { return 14; } };

extern "C" int CountSystems() {
   return 1;
}
extern "C" IGameSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 41: return new MovementSystem();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IGameSystem* scriptSystem) {
   delete scriptSystem; 
}
extern "C" int CountComponents() {
   return 1;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 28: return new Velocity();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 28: { delete ((Velocity*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 28: { Velocity* co = (Velocity*)c; 
      Velocity* so = ((Velocity*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 28: {
      Velocity* component = (Velocity*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Velocity";
	      info->AddField(component->Size, "Size");
	      info->AddField(component->children, "children");
	      info->AddField(component->coords, "coords");
	      info->AddField(component->name, "name");
	      info->AddField(component->velocity, "velocity");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(Pocket::TypeInfo* typeInfo) {
delete typeInfo;
}
