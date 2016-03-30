namespace Pocket {
}
struct Jumpable;
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
template<> Jumpable* GameObject::GetComponent<Jumpable>() { return (Jumpable*) GetScriptComponent(0); }
template<> Jumpable* GameObject::AddComponent<Jumpable>() { return (Jumpable*) AddScriptComponent(0); }
template<> void GameObject::RemoveComponent<Jumpable>() { RemoveScriptComponent(0); }
template<> Sprite* GameObject::GetComponent<Sprite>() { return (Sprite*) GetScriptComponent(1); }
template<> Sprite* GameObject::AddComponent<Sprite>() { return (Sprite*) AddScriptComponent(1); }
template<> void GameObject::RemoveComponent<Sprite>() { RemoveScriptComponent(1); }
#include "TypeInfo.hpp"
#include "Transform.hpp"
#include "Property.hpp"
#include "Vector3.hpp"
#include "Sizeable.hpp"
#include "ScriptExample.hpp"

extern "C" int CountSystems() {
   return 3;
}
extern "C" IScriptSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 0: return new JumpSystem();
      case 1: return new ScriptTransformSystem();
      case 2: return new SpriteSystem();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IScriptSystem* scriptSystem) {
   delete scriptSystem; 
}
extern "C" int CountComponents() {
   return 2;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 0: return new Jumpable();
      case 1: return new Sprite();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 0: { delete ((Jumpable*)component); break; }
      case 1: { delete ((Sprite*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 0: { Jumpable* co = (Jumpable*)c; 
      Jumpable* so = ((Jumpable*)s);
        co->operator=(*so);             break; }
      case 1: { Sprite* co = (Sprite*)c; 
      Sprite* so = ((Sprite*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 0: {
      Jumpable* component = (Jumpable*)componentPtr;
      Pocket::TypeInfo* info = new Pocket::TypeInfo();
      info->AddField(component->JumpHeight, "JumpHeight");
      return info;
      break; }
      case 1: {
      Sprite* component = (Sprite*)componentPtr;
      Pocket::TypeInfo* info = new Pocket::TypeInfo();
      info->AddField(component->width, "width");
      info->AddField(component->height, "height");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(Pocket::TypeInfo* typeInfo) {
delete typeInfo;
}
