namespace Pocket {
}
struct Collidable;
struct Controller;
struct Velocity;

#include "GameSystem.hpp"
template<> Collidable* Pocket::GameObject::GetComponent<Collidable>() { return (Collidable*) GetComponent(0); }
template<> Collidable* Pocket::GameObject::AddComponent<Collidable>() { AddComponent(0); return (Collidable*) GetComponent(0); }
template<> void Pocket::GameObject::RemoveComponent<Collidable>() { RemoveComponent(0); }
template<> Collidable* Pocket::GameObject::CloneComponent<Collidable>(GameObject* source) { CloneComponent(0, source); return (Collidable*) GetComponent(0); }
template<> Controller* Pocket::GameObject::GetComponent<Controller>() { return (Controller*) GetComponent(1); }
template<> Controller* Pocket::GameObject::AddComponent<Controller>() { AddComponent(1); return (Controller*) GetComponent(1); }
template<> void Pocket::GameObject::RemoveComponent<Controller>() { RemoveComponent(1); }
template<> Controller* Pocket::GameObject::CloneComponent<Controller>(GameObject* source) { CloneComponent(1, source); return (Controller*) GetComponent(1); }
template<> Velocity* Pocket::GameObject::GetComponent<Velocity>() { return (Velocity*) GetComponent(2); }
template<> Velocity* Pocket::GameObject::AddComponent<Velocity>() { AddComponent(2); return (Velocity*) GetComponent(2); }
template<> void Pocket::GameObject::RemoveComponent<Velocity>() { RemoveComponent(2); }
template<> Velocity* Pocket::GameObject::CloneComponent<Velocity>(GameObject* source) { CloneComponent(2, source); return (Velocity*) GetComponent(2); }
namespace Pocket { class IGameSystem; }
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
#include "InputController.hpp"
#include "Collidable.cpp"
#include "Controller.cpp"
#include "Velocity.cpp"
#include "VelocityCollider.cpp"

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
   return 4;
}
extern "C" IGameSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 0: return new CollisionSystem();
      case 1: return new ControllerSystem();
      case 2: return new VelocityCollider();
      case 3: return new VelocitySystem();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IGameSystem* scriptSystem) {
   delete scriptSystem; 
}
extern "C" int CountComponents() {
   return 3;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 0: return new Collidable();
      case 1: return new Controller();
      case 2: return new Velocity();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 0: { delete ((Collidable*)component); break; }
      case 1: { delete ((Controller*)component); break; }
      case 2: { delete ((Velocity*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 0: { Collidable* co = (Collidable*)c; 
      Collidable* so = ((Collidable*)s);
        co->operator=(*so);             break; }
      case 1: { Controller* co = (Controller*)c; 
      Controller* so = ((Controller*)s);
        co->operator=(*so);             break; }
      case 2: { Velocity* co = (Velocity*)c; 
      Velocity* so = ((Velocity*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 0: {
      Collidable* component = (Collidable*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Collidable";
	      info->AddField(component->collisions, "collisions");
	      info->AddField(component->previousCollisions, "previousCollisions");
	      info->AddField(component->radius, "radius");
      return info;
      break; }
      case 1: {
      Controller* component = (Controller*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Controller";
	      info->AddField(component->downKey, "downKey");
	      info->AddField(component->speed, "speed");
	      info->AddField(component->upKey, "upKey");
      return info;
      break; }
      case 2: {
      Velocity* component = (Velocity*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Velocity";
	      info->AddField(component->velocity, "velocity");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(Pocket::TypeInfo* typeInfo) {
delete typeInfo;
}
