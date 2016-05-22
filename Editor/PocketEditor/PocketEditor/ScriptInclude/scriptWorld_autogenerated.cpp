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
struct ClickColorer;
struct ColorCycler;
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
template<> Pocket::Touchable* GameObject::GetComponent<Pocket::Touchable>() { return (Pocket::Touchable*) GetComponent(6); }
template<> Pocket::EditorObject* GameObject::GetComponent<Pocket::EditorObject>() { return (Pocket::EditorObject*) GetComponent(14); }
template<> Pocket::Rotator* GameObject::GetComponent<Pocket::Rotator>() { return (Pocket::Rotator*) GetComponent(31); }
template<> Pocket::Transform* GameObject::AddComponent<Pocket::Transform>() { return (Pocket::Transform*) AddComponent(0); }
template<> Pocket::Mesh* GameObject::AddComponent<Pocket::Mesh>() { return (Pocket::Mesh*) AddComponent(1); }
template<> Pocket::Material* GameObject::AddComponent<Pocket::Material>() { return (Pocket::Material*) AddComponent(2); }
template<> Pocket::Camera* GameObject::AddComponent<Pocket::Camera>() { return (Pocket::Camera*) AddComponent(3); }
template<> Pocket::TextureComponent* GameObject::AddComponent<Pocket::TextureComponent>() { return (Pocket::TextureComponent*) AddComponent(4); }
template<> Pocket::Orderable* GameObject::AddComponent<Pocket::Orderable>() { return (Pocket::Orderable*) AddComponent(5); }
template<> Pocket::Touchable* GameObject::AddComponent<Pocket::Touchable>() { return (Pocket::Touchable*) AddComponent(6); }
template<> Pocket::EditorObject* GameObject::AddComponent<Pocket::EditorObject>() { return (Pocket::EditorObject*) AddComponent(14); }
template<> Pocket::Rotator* GameObject::AddComponent<Pocket::Rotator>() { return (Pocket::Rotator*) AddComponent(31); }
template<> void GameObject::RemoveComponent<Pocket::Transform>() { RemoveComponent(0); }
template<> void GameObject::RemoveComponent<Pocket::Mesh>() { RemoveComponent(1); }
template<> void GameObject::RemoveComponent<Pocket::Material>() { RemoveComponent(2); }
template<> void GameObject::RemoveComponent<Pocket::Camera>() { RemoveComponent(3); }
template<> void GameObject::RemoveComponent<Pocket::TextureComponent>() { RemoveComponent(4); }
template<> void GameObject::RemoveComponent<Pocket::Orderable>() { RemoveComponent(5); }
template<> void GameObject::RemoveComponent<Pocket::Touchable>() { RemoveComponent(6); }
template<> void GameObject::RemoveComponent<Pocket::EditorObject>() { RemoveComponent(14); }
template<> void GameObject::RemoveComponent<Pocket::Rotator>() { RemoveComponent(31); }
template<> ClickColorer* GameObject::GetComponent<ClickColorer>() { return (ClickColorer*) GetScriptComponent(0); }
template<> ClickColorer* GameObject::AddComponent<ClickColorer>() { return (ClickColorer*) AddScriptComponent(0); }
template<> void GameObject::RemoveComponent<ClickColorer>() { RemoveScriptComponent(0); }
template<> ColorCycler* GameObject::GetComponent<ColorCycler>() { return (ColorCycler*) GetScriptComponent(1); }
template<> ColorCycler* GameObject::AddComponent<ColorCycler>() { return (ColorCycler*) AddScriptComponent(1); }
template<> void GameObject::RemoveComponent<ColorCycler>() { RemoveScriptComponent(1); }
template<> Jumpable* GameObject::GetComponent<Jumpable>() { return (Jumpable*) GetScriptComponent(2); }
template<> Jumpable* GameObject::AddComponent<Jumpable>() { return (Jumpable*) AddScriptComponent(2); }
template<> void GameObject::RemoveComponent<Jumpable>() { RemoveScriptComponent(2); }
template<> RotatorComponent* GameObject::GetComponent<RotatorComponent>() { return (RotatorComponent*) GetScriptComponent(3); }
template<> RotatorComponent* GameObject::AddComponent<RotatorComponent>() { return (RotatorComponent*) AddScriptComponent(3); }
template<> void GameObject::RemoveComponent<RotatorComponent>() { RemoveScriptComponent(3); }
template<> Sprite* GameObject::GetComponent<Sprite>() { return (Sprite*) GetScriptComponent(4); }
template<> Sprite* GameObject::AddComponent<Sprite>() { return (Sprite*) AddScriptComponent(4); }
template<> void GameObject::RemoveComponent<Sprite>() { RemoveScriptComponent(4); }
#include "TypeInfo.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Property.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Sizeable.hpp"
#include "VertexMesh.hpp"
#include "TextureAtlas.hpp"
#include "Colour.hpp"
#include "Touchable.hpp"
#include "ScriptExample.hpp"

extern "C" int CountSystems() {
   return 6;
}
extern "C" IScriptSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 0: return new ClickColorerSystem();
      case 1: return new ColorCyclerSystem();
      case 2: return new JumpSystem();
      case 3: return new RotatorScriptSystem();
      case 4: return new ScriptTransformSystem();
      case 5: return new SpriteSystem();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IScriptSystem* scriptSystem) {
   delete scriptSystem; 
}
extern "C" int CountComponents() {
   return 5;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 0: return new ClickColorer();
      case 1: return new ColorCycler();
      case 2: return new Jumpable();
      case 3: return new RotatorComponent();
      case 4: return new Sprite();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 0: { delete ((ClickColorer*)component); break; }
      case 1: { delete ((ColorCycler*)component); break; }
      case 2: { delete ((Jumpable*)component); break; }
      case 3: { delete ((RotatorComponent*)component); break; }
      case 4: { delete ((Sprite*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 0: { ClickColorer* co = (ClickColorer*)c; 
      ClickColorer* so = ((ClickColorer*)s);
        co->operator=(*so);             break; }
      case 1: { ColorCycler* co = (ColorCycler*)c; 
      ColorCycler* so = ((ColorCycler*)s);
        co->operator=(*so);             break; }
      case 2: { Jumpable* co = (Jumpable*)c; 
      Jumpable* so = ((Jumpable*)s);
        co->operator=(*so);             break; }
      case 3: { RotatorComponent* co = (RotatorComponent*)c; 
      RotatorComponent* so = ((RotatorComponent*)s);
        co->operator=(*so);             break; }
      case 4: { Sprite* co = (Sprite*)c; 
      Sprite* so = ((Sprite*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 0: {
      ClickColorer* component = (ClickColorer*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "ClickColorer";
	      info->AddField(component->downColor, "downColor");
	      info->AddField(component->upColor, "upColor");
      return info;
      break; }
      case 1: {
      ColorCycler* component = (ColorCycler*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "ColorCycler";
	      info->AddField(component->phase, "phase");
	      info->AddField(component->speed, "speed");
	      info->AddField(component->vertexOffset, "vertexOffset");
      return info;
      break; }
      case 2: {
      Jumpable* component = (Jumpable*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Jumpable";
	      info->AddField(component->JumpHeight, "JumpHeight");
      return info;
      break; }
      case 3: {
      RotatorComponent* component = (RotatorComponent*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "RotatorComponent";
	      info->AddField(component->max, "max");
	      info->AddField(component->min, "min");
	      info->AddField(component->speed, "speed");
	      info->AddField(component->time, "time");
      return info;
      break; }
      case 4: {
      Sprite* component = (Sprite*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Sprite";
	      info->AddField(component->Size, "Size");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(Pocket::TypeInfo* typeInfo) {
delete typeInfo;
}
