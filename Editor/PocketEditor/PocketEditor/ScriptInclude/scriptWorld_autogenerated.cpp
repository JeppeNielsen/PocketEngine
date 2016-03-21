namespace Pocket {
struct Transform;
struct Mesh;
struct Material;
struct Camera;
struct TextureComponent;
struct Orderable;
struct TextBox;
struct Touchable;
struct Label;
struct Font;
struct Sizeable;
struct Atlas;
struct Sprite;
struct Draggable;
struct Layoutable;
struct Menu;
struct MenuAnimator;
struct TransformAnimator;
struct TransformAnimationDatabase;
struct MenuButton;
struct Colorable;
struct Velocity;
struct Limitable;
struct SelectedColorer;
struct Selectable;
struct Droppable;
struct Rotator;
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
template<> Pocket::Transform* GameObject::GetComponent<Pocket::Transform>() { return (Pocket::Transform*) GetComponent(0); }
template<> Pocket::Mesh* GameObject::GetComponent<Pocket::Mesh>() { return (Pocket::Mesh*) GetComponent(1); }
template<> Pocket::Material* GameObject::GetComponent<Pocket::Material>() { return (Pocket::Material*) GetComponent(2); }
template<> Pocket::Camera* GameObject::GetComponent<Pocket::Camera>() { return (Pocket::Camera*) GetComponent(3); }
template<> Pocket::TextureComponent* GameObject::GetComponent<Pocket::TextureComponent>() { return (Pocket::TextureComponent*) GetComponent(4); }
template<> Pocket::Orderable* GameObject::GetComponent<Pocket::Orderable>() { return (Pocket::Orderable*) GetComponent(5); }
template<> Pocket::TextBox* GameObject::GetComponent<Pocket::TextBox>() { return (Pocket::TextBox*) GetComponent(6); }
template<> Pocket::Touchable* GameObject::GetComponent<Pocket::Touchable>() { return (Pocket::Touchable*) GetComponent(7); }
template<> Pocket::Label* GameObject::GetComponent<Pocket::Label>() { return (Pocket::Label*) GetComponent(8); }
template<> Pocket::Font* GameObject::GetComponent<Pocket::Font>() { return (Pocket::Font*) GetComponent(9); }
template<> Pocket::Sizeable* GameObject::GetComponent<Pocket::Sizeable>() { return (Pocket::Sizeable*) GetComponent(10); }
template<> Pocket::Atlas* GameObject::GetComponent<Pocket::Atlas>() { return (Pocket::Atlas*) GetComponent(11); }
template<> Pocket::Sprite* GameObject::GetComponent<Pocket::Sprite>() { return (Pocket::Sprite*) GetComponent(12); }
template<> Pocket::Draggable* GameObject::GetComponent<Pocket::Draggable>() { return (Pocket::Draggable*) GetComponent(13); }
template<> Pocket::Layoutable* GameObject::GetComponent<Pocket::Layoutable>() { return (Pocket::Layoutable*) GetComponent(14); }
template<> Pocket::Menu* GameObject::GetComponent<Pocket::Menu>() { return (Pocket::Menu*) GetComponent(15); }
template<> Pocket::MenuAnimator* GameObject::GetComponent<Pocket::MenuAnimator>() { return (Pocket::MenuAnimator*) GetComponent(16); }
template<> Pocket::TransformAnimator* GameObject::GetComponent<Pocket::TransformAnimator>() { return (Pocket::TransformAnimator*) GetComponent(17); }
template<> Pocket::TransformAnimationDatabase* GameObject::GetComponent<Pocket::TransformAnimationDatabase>() { return (Pocket::TransformAnimationDatabase*) GetComponent(18); }
template<> Pocket::MenuButton* GameObject::GetComponent<Pocket::MenuButton>() { return (Pocket::MenuButton*) GetComponent(19); }
template<> Pocket::Colorable* GameObject::GetComponent<Pocket::Colorable>() { return (Pocket::Colorable*) GetComponent(20); }
template<> Pocket::Velocity* GameObject::GetComponent<Pocket::Velocity>() { return (Pocket::Velocity*) GetComponent(21); }
template<> Pocket::Limitable* GameObject::GetComponent<Pocket::Limitable>() { return (Pocket::Limitable*) GetComponent(22); }
template<> Pocket::SelectedColorer* GameObject::GetComponent<Pocket::SelectedColorer>() { return (Pocket::SelectedColorer*) GetComponent(23); }
template<> Pocket::Selectable* GameObject::GetComponent<Pocket::Selectable>() { return (Pocket::Selectable*) GetComponent(24); }
template<> Pocket::Droppable* GameObject::GetComponent<Pocket::Droppable>() { return (Pocket::Droppable*) GetComponent(25); }
template<> Pocket::Rotator* GameObject::GetComponent<Pocket::Rotator>() { return (Pocket::Rotator*) GetComponent(26); }
template<> Pocket::Transform* GameObject::AddComponent<Pocket::Transform>() { return (Pocket::Transform*) AddComponent(0); }
template<> Pocket::Mesh* GameObject::AddComponent<Pocket::Mesh>() { return (Pocket::Mesh*) AddComponent(1); }
template<> Pocket::Material* GameObject::AddComponent<Pocket::Material>() { return (Pocket::Material*) AddComponent(2); }
template<> Pocket::Camera* GameObject::AddComponent<Pocket::Camera>() { return (Pocket::Camera*) AddComponent(3); }
template<> Pocket::TextureComponent* GameObject::AddComponent<Pocket::TextureComponent>() { return (Pocket::TextureComponent*) AddComponent(4); }
template<> Pocket::Orderable* GameObject::AddComponent<Pocket::Orderable>() { return (Pocket::Orderable*) AddComponent(5); }
template<> Pocket::TextBox* GameObject::AddComponent<Pocket::TextBox>() { return (Pocket::TextBox*) AddComponent(6); }
template<> Pocket::Touchable* GameObject::AddComponent<Pocket::Touchable>() { return (Pocket::Touchable*) AddComponent(7); }
template<> Pocket::Label* GameObject::AddComponent<Pocket::Label>() { return (Pocket::Label*) AddComponent(8); }
template<> Pocket::Font* GameObject::AddComponent<Pocket::Font>() { return (Pocket::Font*) AddComponent(9); }
template<> Pocket::Sizeable* GameObject::AddComponent<Pocket::Sizeable>() { return (Pocket::Sizeable*) AddComponent(10); }
template<> Pocket::Atlas* GameObject::AddComponent<Pocket::Atlas>() { return (Pocket::Atlas*) AddComponent(11); }
template<> Pocket::Sprite* GameObject::AddComponent<Pocket::Sprite>() { return (Pocket::Sprite*) AddComponent(12); }
template<> Pocket::Draggable* GameObject::AddComponent<Pocket::Draggable>() { return (Pocket::Draggable*) AddComponent(13); }
template<> Pocket::Layoutable* GameObject::AddComponent<Pocket::Layoutable>() { return (Pocket::Layoutable*) AddComponent(14); }
template<> Pocket::Menu* GameObject::AddComponent<Pocket::Menu>() { return (Pocket::Menu*) AddComponent(15); }
template<> Pocket::MenuAnimator* GameObject::AddComponent<Pocket::MenuAnimator>() { return (Pocket::MenuAnimator*) AddComponent(16); }
template<> Pocket::TransformAnimator* GameObject::AddComponent<Pocket::TransformAnimator>() { return (Pocket::TransformAnimator*) AddComponent(17); }
template<> Pocket::TransformAnimationDatabase* GameObject::AddComponent<Pocket::TransformAnimationDatabase>() { return (Pocket::TransformAnimationDatabase*) AddComponent(18); }
template<> Pocket::MenuButton* GameObject::AddComponent<Pocket::MenuButton>() { return (Pocket::MenuButton*) AddComponent(19); }
template<> Pocket::Colorable* GameObject::AddComponent<Pocket::Colorable>() { return (Pocket::Colorable*) AddComponent(20); }
template<> Pocket::Velocity* GameObject::AddComponent<Pocket::Velocity>() { return (Pocket::Velocity*) AddComponent(21); }
template<> Pocket::Limitable* GameObject::AddComponent<Pocket::Limitable>() { return (Pocket::Limitable*) AddComponent(22); }
template<> Pocket::SelectedColorer* GameObject::AddComponent<Pocket::SelectedColorer>() { return (Pocket::SelectedColorer*) AddComponent(23); }
template<> Pocket::Selectable* GameObject::AddComponent<Pocket::Selectable>() { return (Pocket::Selectable*) AddComponent(24); }
template<> Pocket::Droppable* GameObject::AddComponent<Pocket::Droppable>() { return (Pocket::Droppable*) AddComponent(25); }
template<> Pocket::Rotator* GameObject::AddComponent<Pocket::Rotator>() { return (Pocket::Rotator*) AddComponent(26); }
template<> void GameObject::RemoveComponent<Pocket::Transform>() { RemoveComponent(0); }
template<> void GameObject::RemoveComponent<Pocket::Mesh>() { RemoveComponent(1); }
template<> void GameObject::RemoveComponent<Pocket::Material>() { RemoveComponent(2); }
template<> void GameObject::RemoveComponent<Pocket::Camera>() { RemoveComponent(3); }
template<> void GameObject::RemoveComponent<Pocket::TextureComponent>() { RemoveComponent(4); }
template<> void GameObject::RemoveComponent<Pocket::Orderable>() { RemoveComponent(5); }
template<> void GameObject::RemoveComponent<Pocket::TextBox>() { RemoveComponent(6); }
template<> void GameObject::RemoveComponent<Pocket::Touchable>() { RemoveComponent(7); }
template<> void GameObject::RemoveComponent<Pocket::Label>() { RemoveComponent(8); }
template<> void GameObject::RemoveComponent<Pocket::Font>() { RemoveComponent(9); }
template<> void GameObject::RemoveComponent<Pocket::Sizeable>() { RemoveComponent(10); }
template<> void GameObject::RemoveComponent<Pocket::Atlas>() { RemoveComponent(11); }
template<> void GameObject::RemoveComponent<Pocket::Sprite>() { RemoveComponent(12); }
template<> void GameObject::RemoveComponent<Pocket::Draggable>() { RemoveComponent(13); }
template<> void GameObject::RemoveComponent<Pocket::Layoutable>() { RemoveComponent(14); }
template<> void GameObject::RemoveComponent<Pocket::Menu>() { RemoveComponent(15); }
template<> void GameObject::RemoveComponent<Pocket::MenuAnimator>() { RemoveComponent(16); }
template<> void GameObject::RemoveComponent<Pocket::TransformAnimator>() { RemoveComponent(17); }
template<> void GameObject::RemoveComponent<Pocket::TransformAnimationDatabase>() { RemoveComponent(18); }
template<> void GameObject::RemoveComponent<Pocket::MenuButton>() { RemoveComponent(19); }
template<> void GameObject::RemoveComponent<Pocket::Colorable>() { RemoveComponent(20); }
template<> void GameObject::RemoveComponent<Pocket::Velocity>() { RemoveComponent(21); }
template<> void GameObject::RemoveComponent<Pocket::Limitable>() { RemoveComponent(22); }
template<> void GameObject::RemoveComponent<Pocket::SelectedColorer>() { RemoveComponent(23); }
template<> void GameObject::RemoveComponent<Pocket::Selectable>() { RemoveComponent(24); }
template<> void GameObject::RemoveComponent<Pocket::Droppable>() { RemoveComponent(25); }
template<> void GameObject::RemoveComponent<Pocket::Rotator>() { RemoveComponent(26); }
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
