namespace Pocket {
}
struct Position;
struct Velocity;

#include "GameSystem.hpp"
template<> Position* Pocket::GameObject::GetComponent<Position>() { return (Position*) GetComponent(0); }
template<> Position* Pocket::GameObject::AddComponent<Position>() { AddComponent(0); return (Position*) GetComponent(0); }
template<> void Pocket::GameObject::RemoveComponent<Position>() { RemoveComponent(0); }
template<> Position* Pocket::GameObject::CloneComponent<Position>(GameObject* source) { CloneComponent(0, source); return (Position*) GetComponent(0); }
template<> Velocity* Pocket::GameObject::GetComponent<Velocity>() { return (Velocity*) GetComponent(1); }
template<> Velocity* Pocket::GameObject::AddComponent<Velocity>() { AddComponent(1); return (Velocity*) GetComponent(1); }
template<> void Pocket::GameObject::RemoveComponent<Velocity>() { RemoveComponent(1); }
template<> Velocity* Pocket::GameObject::CloneComponent<Velocity>(GameObject* source) { CloneComponent(1, source); return (Velocity*) GetComponent(1); }
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
#include "ScriptTest.cpp"

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

extern "C" int CountSystems() {
   return 1;
}
extern "C" IGameSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 0: return new VelocitySystem();
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
      case 0: return new Position();
      case 1: return new Velocity();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 0: { delete ((Position*)component); break; }
      case 1: { delete ((Velocity*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 0: { Position* co = (Position*)c; 
      Position* so = ((Position*)s);
        co->operator=(*so);             break; }
      case 1: { Velocity* co = (Velocity*)c; 
      Velocity* so = ((Velocity*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 0: {
      Position* component = (Position*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Position";
	      info->AddField(component->position, "position");
      return info;
      break; }
      case 1: {
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
