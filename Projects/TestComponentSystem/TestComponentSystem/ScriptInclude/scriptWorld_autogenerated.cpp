namespace Pocket {
}
struct Position;
struct Velocity;

#include "GameSystem.hpp"
using Script_Position = Position;
template<> Script_Position* Pocket::GameObject::GetComponent<Script_Position>() { return (Script_Position*) GetComponent(0); }
template<> Script_Position* Pocket::GameObject::AddComponent<Script_Position>() { AddComponent(0); return (Script_Position*) GetComponent(0); }
template<> void Pocket::GameObject::RemoveComponent<Script_Position>() { RemoveComponent(0); }
template<> Script_Position* Pocket::GameObject::CloneComponent<Script_Position>(GameObject* source) { CloneComponent(0, source); return (Script_Position*) GetComponent(0); }
using Script_Velocity = Velocity;
template<> Script_Velocity* Pocket::GameObject::GetComponent<Script_Velocity>() { return (Script_Velocity*) GetComponent(1); }
template<> Script_Velocity* Pocket::GameObject::AddComponent<Script_Velocity>() { AddComponent(1); return (Script_Velocity*) GetComponent(1); }
template<> void Pocket::GameObject::RemoveComponent<Script_Velocity>() { RemoveComponent(1); }
template<> Script_Velocity* Pocket::GameObject::CloneComponent<Script_Velocity>(GameObject* source) { CloneComponent(1, source); return (Script_Velocity*) GetComponent(1); }
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
