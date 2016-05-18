namespace Pocket {
struct Transform;
struct Mesh;
struct Material;
struct Camera;
struct TextureComponent;
struct Orderable;
struct EditorObject;
struct Rotator;
}
struct Jumpable;
struct RotatorComponent;
struct Sprite;

class GameObject {
private:
    virtual void* GetComponent(int componentID) = 0;
    virtual void* AddComponent(int componentID) = 0;
    virtual void* AddComponent(int componentID, GameObject* referenceObject) = 0;
    virtual void RemoveComponent(int componentID) = 0;
    virtual void* GetScriptComponent(int componentID) = 0; 
    virtual void* AddScriptComponent(int componentID) = 0; 
    virtual void RemoveScriptComponent(int componentID) = 0; 
public:
    template<typename T> T* GetComponent() { return (T*)0; }
    template<typename T> T* AddComponent() { return (T*)0; }
    template<typename T> void RemoveComponent() { }
};
template<> Pocket::Transform* GameObject::GetComponent<Pocket::Transform>() { return (Pocket::Transform*) GetComponent(0); }
template<> Pocket::Mesh* GameObject::GetComponent<Pocket::Mesh>() { return (Pocket::Mesh*) GetComponent(1); }
template<> Pocket::Material* GameObject::GetComponent<Pocket::Material>() { return (Pocket::Material*) GetComponent(2); }
template<> Pocket::Camera* GameObject::GetComponent<Pocket::Camera>() { return (Pocket::Camera*) GetComponent(3); }
template<> Pocket::TextureComponent* GameObject::GetComponent<Pocket::TextureComponent>() { return (Pocket::TextureComponent*) GetComponent(4); }
template<> Pocket::Orderable* GameObject::GetComponent<Pocket::Orderable>() { return (Pocket::Orderable*) GetComponent(5); }
template<> Pocket::EditorObject* GameObject::GetComponent<Pocket::EditorObject>() { return (Pocket::EditorObject*) GetComponent(14); }
template<> Pocket::Rotator* GameObject::GetComponent<Pocket::Rotator>() { return (Pocket::Rotator*) GetComponent(31); }
template<> Pocket::Transform* GameObject::AddComponent<Pocket::Transform>() { return (Pocket::Transform*) AddComponent(0); }
template<> Pocket::Mesh* GameObject::AddComponent<Pocket::Mesh>() { return (Pocket::Mesh*) AddComponent(1); }
template<> Pocket::Material* GameObject::AddComponent<Pocket::Material>() { return (Pocket::Material*) AddComponent(2); }
template<> Pocket::Camera* GameObject::AddComponent<Pocket::Camera>() { return (Pocket::Camera*) AddComponent(3); }
template<> Pocket::TextureComponent* GameObject::AddComponent<Pocket::TextureComponent>() { return (Pocket::TextureComponent*) AddComponent(4); }
template<> Pocket::Orderable* GameObject::AddComponent<Pocket::Orderable>() { return (Pocket::Orderable*) AddComponent(5); }
template<> Pocket::EditorObject* GameObject::AddComponent<Pocket::EditorObject>() { return (Pocket::EditorObject*) AddComponent(14); }
template<> Pocket::Rotator* GameObject::AddComponent<Pocket::Rotator>() { return (Pocket::Rotator*) AddComponent(31); }
template<> void GameObject::RemoveComponent<Pocket::Transform>() { RemoveComponent(0); }
template<> void GameObject::RemoveComponent<Pocket::Mesh>() { RemoveComponent(1); }
template<> void GameObject::RemoveComponent<Pocket::Material>() { RemoveComponent(2); }
template<> void GameObject::RemoveComponent<Pocket::Camera>() { RemoveComponent(3); }
template<> void GameObject::RemoveComponent<Pocket::TextureComponent>() { RemoveComponent(4); }
template<> void GameObject::RemoveComponent<Pocket::Orderable>() { RemoveComponent(5); }
template<> void GameObject::RemoveComponent<Pocket::EditorObject>() { RemoveComponent(14); }
template<> void GameObject::RemoveComponent<Pocket::Rotator>() { RemoveComponent(31); }
template<> Jumpable* GameObject::GetComponent<Jumpable>() { return (Jumpable*) GetScriptComponent(0); }
template<> Jumpable* GameObject::AddComponent<Jumpable>() { return (Jumpable*) AddScriptComponent(0); }
template<> void GameObject::RemoveComponent<Jumpable>() { RemoveScriptComponent(0); }
template<> RotatorComponent* GameObject::GetComponent<RotatorComponent>() { return (RotatorComponent*) GetScriptComponent(1); }
template<> RotatorComponent* GameObject::AddComponent<RotatorComponent>() { return (RotatorComponent*) AddScriptComponent(1); }
template<> void GameObject::RemoveComponent<RotatorComponent>() { RemoveScriptComponent(1); }
template<> Sprite* GameObject::GetComponent<Sprite>() { return (Sprite*) GetScriptComponent(2); }
template<> Sprite* GameObject::AddComponent<Sprite>() { return (Sprite*) AddScriptComponent(2); }
template<> void GameObject::RemoveComponent<Sprite>() { RemoveScriptComponent(2); }
#include "TypeInfo.hpp"
#include "Transform.hpp"
#include "Property.hpp"
#include "Vector3.hpp"
#include "Sizeable.hpp"
#include "ScriptExample.hpp"

extern "C" int CountSystems() {
   return 4;
}
extern "C" IScriptSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 0: return new JumpSystem();
      case 1: return new RotatorScriptSystem();
      case 2: return new ScriptTransformSystem();
      case 3: return new SpriteSystem();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IScriptSystem* scriptSystem) {
   delete scriptSystem; 
}
extern "C" int CountComponents() {
   return 3;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 0: return new Jumpable();
      case 1: return new RotatorComponent();
      case 2: return new Sprite();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 0: { delete ((Jumpable*)component); break; }
      case 1: { delete ((RotatorComponent*)component); break; }
      case 2: { delete ((Sprite*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 0: { Jumpable* co = (Jumpable*)c; 
      Jumpable* so = ((Jumpable*)s);
        co->operator=(*so);             break; }
      case 1: { RotatorComponent* co = (RotatorComponent*)c; 
      RotatorComponent* so = ((RotatorComponent*)s);
        co->operator=(*so);             break; }
      case 2: { Sprite* co = (Sprite*)c; 
      Sprite* so = ((Sprite*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 0: {
      Jumpable* component = (Jumpable*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Jumpable";
	      info->AddField(component->JumpHeight, "JumpHeight");
      return info;
      break; }
      case 1: {
      RotatorComponent* component = (RotatorComponent*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "RotatorComponent";
	      info->AddField(component->max, "max");
	      info->AddField(component->min, "min");
	      info->AddField(component->speed, "speed");
	      info->AddField(component->time, "time");
      return info;
      break; }
      case 2: {
      Sprite* component = (Sprite*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Sprite";
	      info->AddField(component->height, "height");
	      info->AddField(component->width, "width");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(Pocket::TypeInfo* typeInfo) {
delete typeInfo;
}
