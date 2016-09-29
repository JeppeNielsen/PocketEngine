namespace Pocket {
struct Transform;
struct Mesh;
struct Material;
struct Camera;
struct TextureComponent;
struct Orderable;
struct Touchable;
struct Cloner;
struct Turner;
struct EditorObject;
}
struct Blinker;
struct Jumper;

namespace Pocket {
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
}
template<> Pocket::Transform* Pocket::GameObject::GetComponent<Pocket::Transform>() { return (Pocket::Transform*) GetComponent(0); }
template<> Pocket::Transform* Pocket::GameObject::AddComponent<Pocket::Transform>() { AddComponent(0); return (Pocket::Transform*) GetComponent(0); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Transform>() { RemoveComponent(0); }
template<> Pocket::Transform* Pocket::GameObject::CloneComponent<Pocket::Transform>(GameObject* source) { CloneComponent(0, source); return (Pocket::Transform*) GetComponent(0); }
template<> Pocket::Mesh* Pocket::GameObject::GetComponent<Pocket::Mesh>() { return (Pocket::Mesh*) GetComponent(1); }
template<> Pocket::Mesh* Pocket::GameObject::AddComponent<Pocket::Mesh>() { AddComponent(1); return (Pocket::Mesh*) GetComponent(1); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Mesh>() { RemoveComponent(1); }
template<> Pocket::Mesh* Pocket::GameObject::CloneComponent<Pocket::Mesh>(GameObject* source) { CloneComponent(1, source); return (Pocket::Mesh*) GetComponent(1); }
template<> Pocket::Material* Pocket::GameObject::GetComponent<Pocket::Material>() { return (Pocket::Material*) GetComponent(2); }
template<> Pocket::Material* Pocket::GameObject::AddComponent<Pocket::Material>() { AddComponent(2); return (Pocket::Material*) GetComponent(2); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Material>() { RemoveComponent(2); }
template<> Pocket::Material* Pocket::GameObject::CloneComponent<Pocket::Material>(GameObject* source) { CloneComponent(2, source); return (Pocket::Material*) GetComponent(2); }
template<> Pocket::Camera* Pocket::GameObject::GetComponent<Pocket::Camera>() { return (Pocket::Camera*) GetComponent(3); }
template<> Pocket::Camera* Pocket::GameObject::AddComponent<Pocket::Camera>() { AddComponent(3); return (Pocket::Camera*) GetComponent(3); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Camera>() { RemoveComponent(3); }
template<> Pocket::Camera* Pocket::GameObject::CloneComponent<Pocket::Camera>(GameObject* source) { CloneComponent(3, source); return (Pocket::Camera*) GetComponent(3); }
template<> Pocket::TextureComponent* Pocket::GameObject::GetComponent<Pocket::TextureComponent>() { return (Pocket::TextureComponent*) GetComponent(4); }
template<> Pocket::TextureComponent* Pocket::GameObject::AddComponent<Pocket::TextureComponent>() { AddComponent(4); return (Pocket::TextureComponent*) GetComponent(4); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::TextureComponent>() { RemoveComponent(4); }
template<> Pocket::TextureComponent* Pocket::GameObject::CloneComponent<Pocket::TextureComponent>(GameObject* source) { CloneComponent(4, source); return (Pocket::TextureComponent*) GetComponent(4); }
template<> Pocket::Orderable* Pocket::GameObject::GetComponent<Pocket::Orderable>() { return (Pocket::Orderable*) GetComponent(5); }
template<> Pocket::Orderable* Pocket::GameObject::AddComponent<Pocket::Orderable>() { AddComponent(5); return (Pocket::Orderable*) GetComponent(5); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Orderable>() { RemoveComponent(5); }
template<> Pocket::Orderable* Pocket::GameObject::CloneComponent<Pocket::Orderable>(GameObject* source) { CloneComponent(5, source); return (Pocket::Orderable*) GetComponent(5); }
template<> Pocket::Touchable* Pocket::GameObject::GetComponent<Pocket::Touchable>() { return (Pocket::Touchable*) GetComponent(7); }
template<> Pocket::Touchable* Pocket::GameObject::AddComponent<Pocket::Touchable>() { AddComponent(7); return (Pocket::Touchable*) GetComponent(7); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Touchable>() { RemoveComponent(7); }
template<> Pocket::Touchable* Pocket::GameObject::CloneComponent<Pocket::Touchable>(GameObject* source) { CloneComponent(7, source); return (Pocket::Touchable*) GetComponent(7); }
template<> Pocket::Cloner* Pocket::GameObject::GetComponent<Pocket::Cloner>() { return (Pocket::Cloner*) GetComponent(28); }
template<> Pocket::Cloner* Pocket::GameObject::AddComponent<Pocket::Cloner>() { AddComponent(28); return (Pocket::Cloner*) GetComponent(28); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Cloner>() { RemoveComponent(28); }
template<> Pocket::Cloner* Pocket::GameObject::CloneComponent<Pocket::Cloner>(GameObject* source) { CloneComponent(28, source); return (Pocket::Cloner*) GetComponent(28); }
template<> Pocket::Turner* Pocket::GameObject::GetComponent<Pocket::Turner>() { return (Pocket::Turner*) GetComponent(29); }
template<> Pocket::Turner* Pocket::GameObject::AddComponent<Pocket::Turner>() { AddComponent(29); return (Pocket::Turner*) GetComponent(29); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Turner>() { RemoveComponent(29); }
template<> Pocket::Turner* Pocket::GameObject::CloneComponent<Pocket::Turner>(GameObject* source) { CloneComponent(29, source); return (Pocket::Turner*) GetComponent(29); }
template<> Pocket::EditorObject* Pocket::GameObject::GetComponent<Pocket::EditorObject>() { return (Pocket::EditorObject*) GetComponent(30); }
template<> Pocket::EditorObject* Pocket::GameObject::AddComponent<Pocket::EditorObject>() { AddComponent(30); return (Pocket::EditorObject*) GetComponent(30); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::EditorObject>() { RemoveComponent(30); }
template<> Pocket::EditorObject* Pocket::GameObject::CloneComponent<Pocket::EditorObject>(GameObject* source) { CloneComponent(30, source); return (Pocket::EditorObject*) GetComponent(30); }
template<> Blinker* Pocket::GameObject::GetComponent<Blinker>() { return (Blinker*) GetComponent(31); }
template<> Blinker* Pocket::GameObject::AddComponent<Blinker>() { AddComponent(31); return (Blinker*) GetComponent(31); }
template<> void Pocket::GameObject::RemoveComponent<Blinker>() { RemoveComponent(31); }
template<> Blinker* Pocket::GameObject::CloneComponent<Blinker>(GameObject* source) { CloneComponent(31, source); return (Blinker*) GetComponent(31); }
template<> Jumper* Pocket::GameObject::GetComponent<Jumper>() { return (Jumper*) GetComponent(32); }
template<> Jumper* Pocket::GameObject::AddComponent<Jumper>() { AddComponent(32); return (Jumper*) GetComponent(32); }
template<> void Pocket::GameObject::RemoveComponent<Jumper>() { RemoveComponent(32); }
template<> Jumper* Pocket::GameObject::CloneComponent<Jumper>(GameObject* source) { CloneComponent(32, source); return (Jumper*) GetComponent(32); }
class IGameSystem;
#include "TypeInfo.hpp"
#include "Property.hpp"
#include "Transform.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "VertexMesh.hpp"
#include "TextureAtlas.hpp"
#include "Colour.hpp"
#include "Touchable.hpp"
#include "Blinker.cpp"
#include "Jumper.cpp"

#include <string>
#include <vector>
#include "Property.hpp"
template<> struct Pocket::FieldInfoIndexer<bool> { static int Index() { return 0; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<bool>> { static int Index() { return 1; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<bool>> { static int Index() { return 2; } };
template<> struct Pocket::FieldInfoIndexer<int> { static int Index() { return 3; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<int>> { static int Index() { return 4; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<int>> { static int Index() { return 5; } };
template<> struct Pocket::FieldInfoIndexer<float> { static int Index() { return 6; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<float>> { static int Index() { return 7; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<float>> { static int Index() { return 8; } };
template<> struct Pocket::FieldInfoIndexer<double> { static int Index() { return 9; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<double>> { static int Index() { return 10; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<double>> { static int Index() { return 11; } };
template<> struct Pocket::FieldInfoIndexer<std::__1::basic_string<char>> { static int Index() { return 12; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<std::__1::basic_string<char>>> { static int Index() { return 13; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<std::__1::basic_string<char> >> { static int Index() { return 14; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Vector3> { static int Index() { return 15; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<Pocket::Vector3>> { static int Index() { return 16; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<Pocket::Vector3>> { static int Index() { return 17; } };

extern "C" int CountSystems() {
   return 2;
}
extern "C" IGameSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 37: return new BlinkerSystem();
      case 38: return new JumperSystem();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IGameSystem* scriptSystem) {
   delete scriptSystem; 
}
extern "C" int CountComponents() {
   return 2;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 31: return new Blinker();
      case 32: return new Jumper();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 31: { delete ((Blinker*)component); break; }
      case 32: { delete ((Jumper*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 31: { Blinker* co = (Blinker*)c; 
      Blinker* so = ((Blinker*)s);
        co->operator=(*so);             break; }
      case 32: { Jumper* co = (Jumper*)c; 
      Jumper* so = ((Jumper*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 31: {
      Blinker* component = (Blinker*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Blinker";
	      info->AddField(component->speed, "speed");
	      info->AddField(component->time, "time");
      return info;
      break; }
      case 32: {
      Jumper* component = (Jumper*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Jumper";
	      info->AddField(component->height, "height");
	      info->AddField(component->speed, "speed");
	      info->AddField(component->time, "time");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(Pocket::TypeInfo* typeInfo) {
delete typeInfo;
}
