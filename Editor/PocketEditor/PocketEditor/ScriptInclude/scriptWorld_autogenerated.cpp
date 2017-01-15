namespace Pocket {
struct Transform;
struct Mesh;
struct Renderable;
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
struct Layouter;
struct MenuButton;
struct Colorable;
struct Velocity;
struct Limitable;
struct SelectedColorer;
struct Selectable;
struct Droppable;
struct Cloner;
struct CloneVariable;
struct Turner;
struct EditorObject;
struct InputMapper;
struct InputController;
struct FirstPersonMover;
struct FileSystemListener;
struct VirtualTreeList;
struct VirtualTreeListSpawner;
struct FieldEditor;
struct GameObjectEditor;
struct AssetImporter;
struct FilePath;
struct ShaderComponent;
struct AssetLoader;
}
struct Collidable;
struct Controller;

#include "GameSystem.hpp"
template<> Pocket::Transform* Pocket::GameObject::GetComponent<Pocket::Transform>() { return (Pocket::Transform*) GetComponent(0); }
template<> Pocket::Transform* Pocket::GameObject::AddComponent<Pocket::Transform>() { AddComponent(0); return (Pocket::Transform*) GetComponent(0); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Transform>() { RemoveComponent(0); }
template<> Pocket::Transform* Pocket::GameObject::CloneComponent<Pocket::Transform>(GameObject* source) { CloneComponent(0, source); return (Pocket::Transform*) GetComponent(0); }
template<> Pocket::Mesh* Pocket::GameObject::GetComponent<Pocket::Mesh>() { return (Pocket::Mesh*) GetComponent(1); }
template<> Pocket::Mesh* Pocket::GameObject::AddComponent<Pocket::Mesh>() { AddComponent(1); return (Pocket::Mesh*) GetComponent(1); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Mesh>() { RemoveComponent(1); }
template<> Pocket::Mesh* Pocket::GameObject::CloneComponent<Pocket::Mesh>(GameObject* source) { CloneComponent(1, source); return (Pocket::Mesh*) GetComponent(1); }
template<> Pocket::Renderable* Pocket::GameObject::GetComponent<Pocket::Renderable>() { return (Pocket::Renderable*) GetComponent(2); }
template<> Pocket::Renderable* Pocket::GameObject::AddComponent<Pocket::Renderable>() { AddComponent(2); return (Pocket::Renderable*) GetComponent(2); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Renderable>() { RemoveComponent(2); }
template<> Pocket::Renderable* Pocket::GameObject::CloneComponent<Pocket::Renderable>(GameObject* source) { CloneComponent(2, source); return (Pocket::Renderable*) GetComponent(2); }
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
template<> Pocket::TextBox* Pocket::GameObject::GetComponent<Pocket::TextBox>() { return (Pocket::TextBox*) GetComponent(6); }
template<> Pocket::TextBox* Pocket::GameObject::AddComponent<Pocket::TextBox>() { AddComponent(6); return (Pocket::TextBox*) GetComponent(6); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::TextBox>() { RemoveComponent(6); }
template<> Pocket::TextBox* Pocket::GameObject::CloneComponent<Pocket::TextBox>(GameObject* source) { CloneComponent(6, source); return (Pocket::TextBox*) GetComponent(6); }
template<> Pocket::Touchable* Pocket::GameObject::GetComponent<Pocket::Touchable>() { return (Pocket::Touchable*) GetComponent(7); }
template<> Pocket::Touchable* Pocket::GameObject::AddComponent<Pocket::Touchable>() { AddComponent(7); return (Pocket::Touchable*) GetComponent(7); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Touchable>() { RemoveComponent(7); }
template<> Pocket::Touchable* Pocket::GameObject::CloneComponent<Pocket::Touchable>(GameObject* source) { CloneComponent(7, source); return (Pocket::Touchable*) GetComponent(7); }
template<> Pocket::Label* Pocket::GameObject::GetComponent<Pocket::Label>() { return (Pocket::Label*) GetComponent(8); }
template<> Pocket::Label* Pocket::GameObject::AddComponent<Pocket::Label>() { AddComponent(8); return (Pocket::Label*) GetComponent(8); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Label>() { RemoveComponent(8); }
template<> Pocket::Label* Pocket::GameObject::CloneComponent<Pocket::Label>(GameObject* source) { CloneComponent(8, source); return (Pocket::Label*) GetComponent(8); }
template<> Pocket::Font* Pocket::GameObject::GetComponent<Pocket::Font>() { return (Pocket::Font*) GetComponent(9); }
template<> Pocket::Font* Pocket::GameObject::AddComponent<Pocket::Font>() { AddComponent(9); return (Pocket::Font*) GetComponent(9); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Font>() { RemoveComponent(9); }
template<> Pocket::Font* Pocket::GameObject::CloneComponent<Pocket::Font>(GameObject* source) { CloneComponent(9, source); return (Pocket::Font*) GetComponent(9); }
template<> Pocket::Sizeable* Pocket::GameObject::GetComponent<Pocket::Sizeable>() { return (Pocket::Sizeable*) GetComponent(10); }
template<> Pocket::Sizeable* Pocket::GameObject::AddComponent<Pocket::Sizeable>() { AddComponent(10); return (Pocket::Sizeable*) GetComponent(10); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sizeable>() { RemoveComponent(10); }
template<> Pocket::Sizeable* Pocket::GameObject::CloneComponent<Pocket::Sizeable>(GameObject* source) { CloneComponent(10, source); return (Pocket::Sizeable*) GetComponent(10); }
template<> Pocket::Atlas* Pocket::GameObject::GetComponent<Pocket::Atlas>() { return (Pocket::Atlas*) GetComponent(11); }
template<> Pocket::Atlas* Pocket::GameObject::AddComponent<Pocket::Atlas>() { AddComponent(11); return (Pocket::Atlas*) GetComponent(11); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Atlas>() { RemoveComponent(11); }
template<> Pocket::Atlas* Pocket::GameObject::CloneComponent<Pocket::Atlas>(GameObject* source) { CloneComponent(11, source); return (Pocket::Atlas*) GetComponent(11); }
template<> Pocket::Sprite* Pocket::GameObject::GetComponent<Pocket::Sprite>() { return (Pocket::Sprite*) GetComponent(12); }
template<> Pocket::Sprite* Pocket::GameObject::AddComponent<Pocket::Sprite>() { AddComponent(12); return (Pocket::Sprite*) GetComponent(12); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sprite>() { RemoveComponent(12); }
template<> Pocket::Sprite* Pocket::GameObject::CloneComponent<Pocket::Sprite>(GameObject* source) { CloneComponent(12, source); return (Pocket::Sprite*) GetComponent(12); }
template<> Pocket::Draggable* Pocket::GameObject::GetComponent<Pocket::Draggable>() { return (Pocket::Draggable*) GetComponent(13); }
template<> Pocket::Draggable* Pocket::GameObject::AddComponent<Pocket::Draggable>() { AddComponent(13); return (Pocket::Draggable*) GetComponent(13); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Draggable>() { RemoveComponent(13); }
template<> Pocket::Draggable* Pocket::GameObject::CloneComponent<Pocket::Draggable>(GameObject* source) { CloneComponent(13, source); return (Pocket::Draggable*) GetComponent(13); }
template<> Pocket::Layouter* Pocket::GameObject::GetComponent<Pocket::Layouter>() { return (Pocket::Layouter*) GetComponent(14); }
template<> Pocket::Layouter* Pocket::GameObject::AddComponent<Pocket::Layouter>() { AddComponent(14); return (Pocket::Layouter*) GetComponent(14); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Layouter>() { RemoveComponent(14); }
template<> Pocket::Layouter* Pocket::GameObject::CloneComponent<Pocket::Layouter>(GameObject* source) { CloneComponent(14, source); return (Pocket::Layouter*) GetComponent(14); }
template<> Pocket::MenuButton* Pocket::GameObject::GetComponent<Pocket::MenuButton>() { return (Pocket::MenuButton*) GetComponent(15); }
template<> Pocket::MenuButton* Pocket::GameObject::AddComponent<Pocket::MenuButton>() { AddComponent(15); return (Pocket::MenuButton*) GetComponent(15); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::MenuButton>() { RemoveComponent(15); }
template<> Pocket::MenuButton* Pocket::GameObject::CloneComponent<Pocket::MenuButton>(GameObject* source) { CloneComponent(15, source); return (Pocket::MenuButton*) GetComponent(15); }
template<> Pocket::Colorable* Pocket::GameObject::GetComponent<Pocket::Colorable>() { return (Pocket::Colorable*) GetComponent(16); }
template<> Pocket::Colorable* Pocket::GameObject::AddComponent<Pocket::Colorable>() { AddComponent(16); return (Pocket::Colorable*) GetComponent(16); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Colorable>() { RemoveComponent(16); }
template<> Pocket::Colorable* Pocket::GameObject::CloneComponent<Pocket::Colorable>(GameObject* source) { CloneComponent(16, source); return (Pocket::Colorable*) GetComponent(16); }
template<> Pocket::Velocity* Pocket::GameObject::GetComponent<Pocket::Velocity>() { return (Pocket::Velocity*) GetComponent(17); }
template<> Pocket::Velocity* Pocket::GameObject::AddComponent<Pocket::Velocity>() { AddComponent(17); return (Pocket::Velocity*) GetComponent(17); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Velocity>() { RemoveComponent(17); }
template<> Pocket::Velocity* Pocket::GameObject::CloneComponent<Pocket::Velocity>(GameObject* source) { CloneComponent(17, source); return (Pocket::Velocity*) GetComponent(17); }
template<> Pocket::Limitable* Pocket::GameObject::GetComponent<Pocket::Limitable>() { return (Pocket::Limitable*) GetComponent(18); }
template<> Pocket::Limitable* Pocket::GameObject::AddComponent<Pocket::Limitable>() { AddComponent(18); return (Pocket::Limitable*) GetComponent(18); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Limitable>() { RemoveComponent(18); }
template<> Pocket::Limitable* Pocket::GameObject::CloneComponent<Pocket::Limitable>(GameObject* source) { CloneComponent(18, source); return (Pocket::Limitable*) GetComponent(18); }
template<> Pocket::SelectedColorer* Pocket::GameObject::GetComponent<Pocket::SelectedColorer>() { return (Pocket::SelectedColorer*) GetComponent(19); }
template<> Pocket::SelectedColorer* Pocket::GameObject::AddComponent<Pocket::SelectedColorer>() { AddComponent(19); return (Pocket::SelectedColorer*) GetComponent(19); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SelectedColorer>() { RemoveComponent(19); }
template<> Pocket::SelectedColorer* Pocket::GameObject::CloneComponent<Pocket::SelectedColorer>(GameObject* source) { CloneComponent(19, source); return (Pocket::SelectedColorer*) GetComponent(19); }
template<> Pocket::Selectable* Pocket::GameObject::GetComponent<Pocket::Selectable>() { return (Pocket::Selectable*) GetComponent(20); }
template<> Pocket::Selectable* Pocket::GameObject::AddComponent<Pocket::Selectable>() { AddComponent(20); return (Pocket::Selectable*) GetComponent(20); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Selectable>() { RemoveComponent(20); }
template<> Pocket::Selectable* Pocket::GameObject::CloneComponent<Pocket::Selectable>(GameObject* source) { CloneComponent(20, source); return (Pocket::Selectable*) GetComponent(20); }
template<> Pocket::Droppable* Pocket::GameObject::GetComponent<Pocket::Droppable>() { return (Pocket::Droppable*) GetComponent(21); }
template<> Pocket::Droppable* Pocket::GameObject::AddComponent<Pocket::Droppable>() { AddComponent(21); return (Pocket::Droppable*) GetComponent(21); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Droppable>() { RemoveComponent(21); }
template<> Pocket::Droppable* Pocket::GameObject::CloneComponent<Pocket::Droppable>(GameObject* source) { CloneComponent(21, source); return (Pocket::Droppable*) GetComponent(21); }
template<> Pocket::Cloner* Pocket::GameObject::GetComponent<Pocket::Cloner>() { return (Pocket::Cloner*) GetComponent(22); }
template<> Pocket::Cloner* Pocket::GameObject::AddComponent<Pocket::Cloner>() { AddComponent(22); return (Pocket::Cloner*) GetComponent(22); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Cloner>() { RemoveComponent(22); }
template<> Pocket::Cloner* Pocket::GameObject::CloneComponent<Pocket::Cloner>(GameObject* source) { CloneComponent(22, source); return (Pocket::Cloner*) GetComponent(22); }
template<> Pocket::CloneVariable* Pocket::GameObject::GetComponent<Pocket::CloneVariable>() { return (Pocket::CloneVariable*) GetComponent(23); }
template<> Pocket::CloneVariable* Pocket::GameObject::AddComponent<Pocket::CloneVariable>() { AddComponent(23); return (Pocket::CloneVariable*) GetComponent(23); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::CloneVariable>() { RemoveComponent(23); }
template<> Pocket::CloneVariable* Pocket::GameObject::CloneComponent<Pocket::CloneVariable>(GameObject* source) { CloneComponent(23, source); return (Pocket::CloneVariable*) GetComponent(23); }
template<> Pocket::Turner* Pocket::GameObject::GetComponent<Pocket::Turner>() { return (Pocket::Turner*) GetComponent(24); }
template<> Pocket::Turner* Pocket::GameObject::AddComponent<Pocket::Turner>() { AddComponent(24); return (Pocket::Turner*) GetComponent(24); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Turner>() { RemoveComponent(24); }
template<> Pocket::Turner* Pocket::GameObject::CloneComponent<Pocket::Turner>(GameObject* source) { CloneComponent(24, source); return (Pocket::Turner*) GetComponent(24); }
template<> Pocket::EditorObject* Pocket::GameObject::GetComponent<Pocket::EditorObject>() { return (Pocket::EditorObject*) GetComponent(25); }
template<> Pocket::EditorObject* Pocket::GameObject::AddComponent<Pocket::EditorObject>() { AddComponent(25); return (Pocket::EditorObject*) GetComponent(25); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::EditorObject>() { RemoveComponent(25); }
template<> Pocket::EditorObject* Pocket::GameObject::CloneComponent<Pocket::EditorObject>(GameObject* source) { CloneComponent(25, source); return (Pocket::EditorObject*) GetComponent(25); }
template<> Pocket::InputMapper* Pocket::GameObject::GetComponent<Pocket::InputMapper>() { return (Pocket::InputMapper*) GetComponent(26); }
template<> Pocket::InputMapper* Pocket::GameObject::AddComponent<Pocket::InputMapper>() { AddComponent(26); return (Pocket::InputMapper*) GetComponent(26); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::InputMapper>() { RemoveComponent(26); }
template<> Pocket::InputMapper* Pocket::GameObject::CloneComponent<Pocket::InputMapper>(GameObject* source) { CloneComponent(26, source); return (Pocket::InputMapper*) GetComponent(26); }
template<> Pocket::InputController* Pocket::GameObject::GetComponent<Pocket::InputController>() { return (Pocket::InputController*) GetComponent(27); }
template<> Pocket::InputController* Pocket::GameObject::AddComponent<Pocket::InputController>() { AddComponent(27); return (Pocket::InputController*) GetComponent(27); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::InputController>() { RemoveComponent(27); }
template<> Pocket::InputController* Pocket::GameObject::CloneComponent<Pocket::InputController>(GameObject* source) { CloneComponent(27, source); return (Pocket::InputController*) GetComponent(27); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::GetComponent<Pocket::FirstPersonMover>() { return (Pocket::FirstPersonMover*) GetComponent(28); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::AddComponent<Pocket::FirstPersonMover>() { AddComponent(28); return (Pocket::FirstPersonMover*) GetComponent(28); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FirstPersonMover>() { RemoveComponent(28); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::CloneComponent<Pocket::FirstPersonMover>(GameObject* source) { CloneComponent(28, source); return (Pocket::FirstPersonMover*) GetComponent(28); }
template<> Pocket::FileSystemListener* Pocket::GameObject::GetComponent<Pocket::FileSystemListener>() { return (Pocket::FileSystemListener*) GetComponent(29); }
template<> Pocket::FileSystemListener* Pocket::GameObject::AddComponent<Pocket::FileSystemListener>() { AddComponent(29); return (Pocket::FileSystemListener*) GetComponent(29); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FileSystemListener>() { RemoveComponent(29); }
template<> Pocket::FileSystemListener* Pocket::GameObject::CloneComponent<Pocket::FileSystemListener>(GameObject* source) { CloneComponent(29, source); return (Pocket::FileSystemListener*) GetComponent(29); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::GetComponent<Pocket::VirtualTreeList>() { return (Pocket::VirtualTreeList*) GetComponent(30); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::AddComponent<Pocket::VirtualTreeList>() { AddComponent(30); return (Pocket::VirtualTreeList*) GetComponent(30); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::VirtualTreeList>() { RemoveComponent(30); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::CloneComponent<Pocket::VirtualTreeList>(GameObject* source) { CloneComponent(30, source); return (Pocket::VirtualTreeList*) GetComponent(30); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::GetComponent<Pocket::VirtualTreeListSpawner>() { return (Pocket::VirtualTreeListSpawner*) GetComponent(31); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::AddComponent<Pocket::VirtualTreeListSpawner>() { AddComponent(31); return (Pocket::VirtualTreeListSpawner*) GetComponent(31); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::VirtualTreeListSpawner>() { RemoveComponent(31); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::CloneComponent<Pocket::VirtualTreeListSpawner>(GameObject* source) { CloneComponent(31, source); return (Pocket::VirtualTreeListSpawner*) GetComponent(31); }
template<> Pocket::FieldEditor* Pocket::GameObject::GetComponent<Pocket::FieldEditor>() { return (Pocket::FieldEditor*) GetComponent(32); }
template<> Pocket::FieldEditor* Pocket::GameObject::AddComponent<Pocket::FieldEditor>() { AddComponent(32); return (Pocket::FieldEditor*) GetComponent(32); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FieldEditor>() { RemoveComponent(32); }
template<> Pocket::FieldEditor* Pocket::GameObject::CloneComponent<Pocket::FieldEditor>(GameObject* source) { CloneComponent(32, source); return (Pocket::FieldEditor*) GetComponent(32); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::GetComponent<Pocket::GameObjectEditor>() { return (Pocket::GameObjectEditor*) GetComponent(33); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::AddComponent<Pocket::GameObjectEditor>() { AddComponent(33); return (Pocket::GameObjectEditor*) GetComponent(33); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::GameObjectEditor>() { RemoveComponent(33); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::CloneComponent<Pocket::GameObjectEditor>(GameObject* source) { CloneComponent(33, source); return (Pocket::GameObjectEditor*) GetComponent(33); }
template<> Pocket::AssetImporter* Pocket::GameObject::GetComponent<Pocket::AssetImporter>() { return (Pocket::AssetImporter*) GetComponent(34); }
template<> Pocket::AssetImporter* Pocket::GameObject::AddComponent<Pocket::AssetImporter>() { AddComponent(34); return (Pocket::AssetImporter*) GetComponent(34); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::AssetImporter>() { RemoveComponent(34); }
template<> Pocket::AssetImporter* Pocket::GameObject::CloneComponent<Pocket::AssetImporter>(GameObject* source) { CloneComponent(34, source); return (Pocket::AssetImporter*) GetComponent(34); }
template<> Pocket::FilePath* Pocket::GameObject::GetComponent<Pocket::FilePath>() { return (Pocket::FilePath*) GetComponent(35); }
template<> Pocket::FilePath* Pocket::GameObject::AddComponent<Pocket::FilePath>() { AddComponent(35); return (Pocket::FilePath*) GetComponent(35); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FilePath>() { RemoveComponent(35); }
template<> Pocket::FilePath* Pocket::GameObject::CloneComponent<Pocket::FilePath>(GameObject* source) { CloneComponent(35, source); return (Pocket::FilePath*) GetComponent(35); }
template<> Pocket::ShaderComponent* Pocket::GameObject::GetComponent<Pocket::ShaderComponent>() { return (Pocket::ShaderComponent*) GetComponent(36); }
template<> Pocket::ShaderComponent* Pocket::GameObject::AddComponent<Pocket::ShaderComponent>() { AddComponent(36); return (Pocket::ShaderComponent*) GetComponent(36); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ShaderComponent>() { RemoveComponent(36); }
template<> Pocket::ShaderComponent* Pocket::GameObject::CloneComponent<Pocket::ShaderComponent>(GameObject* source) { CloneComponent(36, source); return (Pocket::ShaderComponent*) GetComponent(36); }
template<> Pocket::AssetLoader* Pocket::GameObject::GetComponent<Pocket::AssetLoader>() { return (Pocket::AssetLoader*) GetComponent(37); }
template<> Pocket::AssetLoader* Pocket::GameObject::AddComponent<Pocket::AssetLoader>() { AddComponent(37); return (Pocket::AssetLoader*) GetComponent(37); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::AssetLoader>() { RemoveComponent(37); }
template<> Pocket::AssetLoader* Pocket::GameObject::CloneComponent<Pocket::AssetLoader>(GameObject* source) { CloneComponent(37, source); return (Pocket::AssetLoader*) GetComponent(37); }
using Script_Collidable = Collidable;
template<> Script_Collidable* Pocket::GameObject::GetComponent<Script_Collidable>() { return (Script_Collidable*) GetComponent(38); }
template<> Script_Collidable* Pocket::GameObject::AddComponent<Script_Collidable>() { AddComponent(38); return (Script_Collidable*) GetComponent(38); }
template<> void Pocket::GameObject::RemoveComponent<Script_Collidable>() { RemoveComponent(38); }
template<> Script_Collidable* Pocket::GameObject::CloneComponent<Script_Collidable>(GameObject* source) { CloneComponent(38, source); return (Script_Collidable*) GetComponent(38); }
using Script_Controller = Controller;
template<> Script_Controller* Pocket::GameObject::GetComponent<Script_Controller>() { return (Script_Controller*) GetComponent(39); }
template<> Script_Controller* Pocket::GameObject::AddComponent<Script_Controller>() { AddComponent(39); return (Script_Controller*) GetComponent(39); }
template<> void Pocket::GameObject::RemoveComponent<Script_Controller>() { RemoveComponent(39); }
template<> Script_Controller* Pocket::GameObject::CloneComponent<Script_Controller>(GameObject* source) { CloneComponent(39, source); return (Script_Controller*) GetComponent(39); }
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
#include "Velocity.hpp"
#include "Collidable.hpp"
#include "Controller.hpp"
#include "VelocityCollider.hpp"

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
   return 3;
}
extern "C" IGameSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 49: return new CollisionSystem();
      case 50: return new ControllerSystem();
      case 51: return new VelocityCollider();
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
      case 38: return new Collidable();
      case 39: return new Controller();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 38: { delete ((Collidable*)component); break; }
      case 39: { delete ((Controller*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 38: { Collidable* co = (Collidable*)c; 
      Collidable* so = ((Collidable*)s);
        co->operator=(*so);             break; }
      case 39: { Controller* co = (Controller*)c; 
      Controller* so = ((Controller*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 38: {
      Collidable* component = (Collidable*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Collidable";
	      info->AddField(component->collisions, "collisions");
	      info->AddField(component->previousCollisions, "previousCollisions");
	      info->AddField(component->radius, "radius");
      return info;
      break; }
      case 39: {
      Controller* component = (Controller*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Controller";
	      info->AddField(component->downKey, "downKey");
	      info->AddField(component->speed, "speed");
	      info->AddField(component->upKey, "upKey");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(Pocket::TypeInfo* typeInfo) {
delete typeInfo;
}
