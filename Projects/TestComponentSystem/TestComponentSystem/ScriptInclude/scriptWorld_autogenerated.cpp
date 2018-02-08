namespace Pocket {
struct Hierarchy;
}
struct Position;
struct Velocity;

#include "GameSystem.hpp"
template<> Pocket::Hierarchy* Pocket::GameObject::GetComponent<Pocket::Hierarchy>() { return (Pocket::Hierarchy*) GetComponent(0); }
template<> Pocket::Hierarchy* Pocket::GameObject::AddComponent<Pocket::Hierarchy>() { AddComponent(0); return (Pocket::Hierarchy*) GetComponent(0); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Hierarchy>() { RemoveComponent(0); }
template<> Pocket::Hierarchy* Pocket::GameObject::CloneComponent<Pocket::Hierarchy>(GameObject* source) { CloneComponent(0, source); return (Pocket::Hierarchy*) GetComponent(0); }
template<> Pocket::Hierarchy* Pocket::GameObject::ReplaceComponent<Pocket::Hierarchy>(GameObject* source) { ReplaceComponent(0, source); return (Pocket::Hierarchy*) GetComponent(0); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Hierarchy>(bool enable) { EnableComponent(0, enable); }
template<> Position* Pocket::GameObject::GetComponent<Position>() { return (Position*) GetComponent(1); }
template<> Position* Pocket::GameObject::AddComponent<Position>() { AddComponent(1); return (Position*) GetComponent(1); }
template<> void Pocket::GameObject::RemoveComponent<Position>() { RemoveComponent(1); }
template<> Position* Pocket::GameObject::CloneComponent<Position>(GameObject* source) { CloneComponent(1, source); return (Position*) GetComponent(1); }
template<> Position* Pocket::GameObject::ReplaceComponent<Position>(GameObject* source) { ReplaceComponent(1, source); return (Position*) GetComponent(1); }
template<> void Pocket::GameObject::EnableComponent<Position>(bool enable) { EnableComponent(1, enable); }
template<> Velocity* Pocket::GameObject::GetComponent<Velocity>() { return (Velocity*) GetComponent(2); }
template<> Velocity* Pocket::GameObject::AddComponent<Velocity>() { AddComponent(2); return (Velocity*) GetComponent(2); }
template<> void Pocket::GameObject::RemoveComponent<Velocity>() { RemoveComponent(2); }
template<> Velocity* Pocket::GameObject::CloneComponent<Velocity>(GameObject* source) { CloneComponent(2, source); return (Velocity*) GetComponent(2); }
template<> Velocity* Pocket::GameObject::ReplaceComponent<Velocity>(GameObject* source) { ReplaceComponent(2, source); return (Velocity*) GetComponent(2); }
template<> void Pocket::GameObject::EnableComponent<Velocity>(bool enable) { EnableComponent(2, enable); }
#include "TypeInfo.hpp"
#include "Property.hpp"
#include "Timeline.hpp"
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
#include "ScriptTestRecompile1.hpp"

#include <string>
#include <vector>
#include "Property.hpp"
template<> struct Pocket::FieldInfoIndexer<int> { static int Index() { return 0; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::GameObjectHandle> { static int Index() { return 1; } };
template<> struct Pocket::FieldInfoIndexer<std::__1::basic_string<char>> { static int Index() { return 2; } };
template<> struct Pocket::FieldInfoIndexer<bool> { static int Index() { return 3; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<bool>> { static int Index() { return 4; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<bool>> { static int Index() { return 5; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<bool>> { static int Index() { return 6; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<int>> { static int Index() { return 7; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<int>> { static int Index() { return 8; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<int>> { static int Index() { return 9; } };
template<> struct Pocket::FieldInfoIndexer<float> { static int Index() { return 10; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<float>> { static int Index() { return 11; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<float>> { static int Index() { return 12; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<float>> { static int Index() { return 13; } };
template<> struct Pocket::FieldInfoIndexer<double> { static int Index() { return 14; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<double>> { static int Index() { return 15; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<double>> { static int Index() { return 16; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<double>> { static int Index() { return 17; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<std::__1::basic_string<char>>> { static int Index() { return 18; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<std::__1::basic_string<char> >> { static int Index() { return 19; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<std::__1::basic_string<char> >> { static int Index() { return 20; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<Pocket::GameObjectHandle>> { static int Index() { return 21; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<Pocket::GameObjectHandle>> { static int Index() { return 22; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<Pocket::GameObjectHandle>> { static int Index() { return 23; } };

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
      case 1: return new Position();
      case 2: return new Velocity();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 1: { delete ((Position*)component); break; }
      case 2: { delete ((Velocity*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 1: { Position* co = (Position*)c; 
      Position* so = ((Position*)s);
        co->operator=(*so);             break; }
      case 2: { Velocity* co = (Velocity*)c; 
      Velocity* so = ((Velocity*)s);
        co->operator=(*so);             break; }
   }
}
extern "C" Pocket::IGameObjectHandleRetriever* SetGameObjectHandleRetriever(Pocket::IGameObjectHandleRetriever* ret) {
 static Pocket::IGameObjectHandleRetriever* retriever = nullptr; 
 if (ret) { 
    retriever = ret; 
 }
 return retriever;
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 1: {
      Position* component = (Position*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Position";
	      info->AddScriptField(component->position, "position");
      return info;
      break; }
      case 2: {
      Velocity* component = (Velocity*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Velocity";
	      info->AddScriptField(component->velocity, "velocity");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(Pocket::TypeInfo* typeInfo) {
delete typeInfo;
}
extern "C" const char* GetComponentName(int componentID) {
   switch (componentID) { 
      case 1: 
         return "Position";
      case 2: 
         return "Velocity";
      default: return "";
   }
}
extern "C" int* GetSystems() {
 return new int[4] {-1,1,2,-2,};}
extern "C" void DeleteGetSystems(int* indicies) {
delete indicies;
}
