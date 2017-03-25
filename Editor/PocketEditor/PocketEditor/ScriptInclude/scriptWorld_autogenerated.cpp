namespace Pocket {
struct Transform;
struct Mesh;
struct Renderable;
struct Camera;
struct TextureComponent;
struct Orderable;
struct TextBox;
struct Touchable;
struct Sprite;
struct Sizeable;
struct Atlas;
struct Draggable;
struct Layouter;
struct MenuButton;
struct Colorable;
struct Velocity;
struct Limitable;
struct SelectedColorer;
struct Selectable;
struct Droppable;
struct Font;
struct Label;
struct Cloner;
struct CloneVariable;
struct EditorDropTarget;
struct ProjectSettings;
struct InputMapper;
struct InputController;
struct Switch;
struct SwitchEnabler;
struct SlicedQuad;
struct SlicedTexture;
struct Trigger;
struct SceneManager;
struct AssetLoader;
struct ShaderComponent;
struct FirstPersonMover;
struct LineRenderer;
struct DistanceScaler;
struct FileSystemListener;
struct VirtualTreeList;
struct VirtualTreeListSpawner;
struct FieldEditor;
struct GameObjectEditor;
struct AssetImporter;
struct FilePath;
}
struct RectCollider;
struct Rotator;
struct TransformController;

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
template<> Pocket::Sprite* Pocket::GameObject::GetComponent<Pocket::Sprite>() { return (Pocket::Sprite*) GetComponent(8); }
template<> Pocket::Sprite* Pocket::GameObject::AddComponent<Pocket::Sprite>() { AddComponent(8); return (Pocket::Sprite*) GetComponent(8); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sprite>() { RemoveComponent(8); }
template<> Pocket::Sprite* Pocket::GameObject::CloneComponent<Pocket::Sprite>(GameObject* source) { CloneComponent(8, source); return (Pocket::Sprite*) GetComponent(8); }
template<> Pocket::Sizeable* Pocket::GameObject::GetComponent<Pocket::Sizeable>() { return (Pocket::Sizeable*) GetComponent(9); }
template<> Pocket::Sizeable* Pocket::GameObject::AddComponent<Pocket::Sizeable>() { AddComponent(9); return (Pocket::Sizeable*) GetComponent(9); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sizeable>() { RemoveComponent(9); }
template<> Pocket::Sizeable* Pocket::GameObject::CloneComponent<Pocket::Sizeable>(GameObject* source) { CloneComponent(9, source); return (Pocket::Sizeable*) GetComponent(9); }
template<> Pocket::Atlas* Pocket::GameObject::GetComponent<Pocket::Atlas>() { return (Pocket::Atlas*) GetComponent(10); }
template<> Pocket::Atlas* Pocket::GameObject::AddComponent<Pocket::Atlas>() { AddComponent(10); return (Pocket::Atlas*) GetComponent(10); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Atlas>() { RemoveComponent(10); }
template<> Pocket::Atlas* Pocket::GameObject::CloneComponent<Pocket::Atlas>(GameObject* source) { CloneComponent(10, source); return (Pocket::Atlas*) GetComponent(10); }
template<> Pocket::Draggable* Pocket::GameObject::GetComponent<Pocket::Draggable>() { return (Pocket::Draggable*) GetComponent(11); }
template<> Pocket::Draggable* Pocket::GameObject::AddComponent<Pocket::Draggable>() { AddComponent(11); return (Pocket::Draggable*) GetComponent(11); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Draggable>() { RemoveComponent(11); }
template<> Pocket::Draggable* Pocket::GameObject::CloneComponent<Pocket::Draggable>(GameObject* source) { CloneComponent(11, source); return (Pocket::Draggable*) GetComponent(11); }
template<> Pocket::Layouter* Pocket::GameObject::GetComponent<Pocket::Layouter>() { return (Pocket::Layouter*) GetComponent(12); }
template<> Pocket::Layouter* Pocket::GameObject::AddComponent<Pocket::Layouter>() { AddComponent(12); return (Pocket::Layouter*) GetComponent(12); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Layouter>() { RemoveComponent(12); }
template<> Pocket::Layouter* Pocket::GameObject::CloneComponent<Pocket::Layouter>(GameObject* source) { CloneComponent(12, source); return (Pocket::Layouter*) GetComponent(12); }
template<> Pocket::MenuButton* Pocket::GameObject::GetComponent<Pocket::MenuButton>() { return (Pocket::MenuButton*) GetComponent(13); }
template<> Pocket::MenuButton* Pocket::GameObject::AddComponent<Pocket::MenuButton>() { AddComponent(13); return (Pocket::MenuButton*) GetComponent(13); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::MenuButton>() { RemoveComponent(13); }
template<> Pocket::MenuButton* Pocket::GameObject::CloneComponent<Pocket::MenuButton>(GameObject* source) { CloneComponent(13, source); return (Pocket::MenuButton*) GetComponent(13); }
template<> Pocket::Colorable* Pocket::GameObject::GetComponent<Pocket::Colorable>() { return (Pocket::Colorable*) GetComponent(14); }
template<> Pocket::Colorable* Pocket::GameObject::AddComponent<Pocket::Colorable>() { AddComponent(14); return (Pocket::Colorable*) GetComponent(14); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Colorable>() { RemoveComponent(14); }
template<> Pocket::Colorable* Pocket::GameObject::CloneComponent<Pocket::Colorable>(GameObject* source) { CloneComponent(14, source); return (Pocket::Colorable*) GetComponent(14); }
template<> Pocket::Velocity* Pocket::GameObject::GetComponent<Pocket::Velocity>() { return (Pocket::Velocity*) GetComponent(15); }
template<> Pocket::Velocity* Pocket::GameObject::AddComponent<Pocket::Velocity>() { AddComponent(15); return (Pocket::Velocity*) GetComponent(15); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Velocity>() { RemoveComponent(15); }
template<> Pocket::Velocity* Pocket::GameObject::CloneComponent<Pocket::Velocity>(GameObject* source) { CloneComponent(15, source); return (Pocket::Velocity*) GetComponent(15); }
template<> Pocket::Limitable* Pocket::GameObject::GetComponent<Pocket::Limitable>() { return (Pocket::Limitable*) GetComponent(16); }
template<> Pocket::Limitable* Pocket::GameObject::AddComponent<Pocket::Limitable>() { AddComponent(16); return (Pocket::Limitable*) GetComponent(16); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Limitable>() { RemoveComponent(16); }
template<> Pocket::Limitable* Pocket::GameObject::CloneComponent<Pocket::Limitable>(GameObject* source) { CloneComponent(16, source); return (Pocket::Limitable*) GetComponent(16); }
template<> Pocket::SelectedColorer* Pocket::GameObject::GetComponent<Pocket::SelectedColorer>() { return (Pocket::SelectedColorer*) GetComponent(17); }
template<> Pocket::SelectedColorer* Pocket::GameObject::AddComponent<Pocket::SelectedColorer>() { AddComponent(17); return (Pocket::SelectedColorer*) GetComponent(17); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SelectedColorer>() { RemoveComponent(17); }
template<> Pocket::SelectedColorer* Pocket::GameObject::CloneComponent<Pocket::SelectedColorer>(GameObject* source) { CloneComponent(17, source); return (Pocket::SelectedColorer*) GetComponent(17); }
template<> Pocket::Selectable* Pocket::GameObject::GetComponent<Pocket::Selectable>() { return (Pocket::Selectable*) GetComponent(18); }
template<> Pocket::Selectable* Pocket::GameObject::AddComponent<Pocket::Selectable>() { AddComponent(18); return (Pocket::Selectable*) GetComponent(18); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Selectable>() { RemoveComponent(18); }
template<> Pocket::Selectable* Pocket::GameObject::CloneComponent<Pocket::Selectable>(GameObject* source) { CloneComponent(18, source); return (Pocket::Selectable*) GetComponent(18); }
template<> Pocket::Droppable* Pocket::GameObject::GetComponent<Pocket::Droppable>() { return (Pocket::Droppable*) GetComponent(19); }
template<> Pocket::Droppable* Pocket::GameObject::AddComponent<Pocket::Droppable>() { AddComponent(19); return (Pocket::Droppable*) GetComponent(19); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Droppable>() { RemoveComponent(19); }
template<> Pocket::Droppable* Pocket::GameObject::CloneComponent<Pocket::Droppable>(GameObject* source) { CloneComponent(19, source); return (Pocket::Droppable*) GetComponent(19); }
template<> Pocket::Font* Pocket::GameObject::GetComponent<Pocket::Font>() { return (Pocket::Font*) GetComponent(20); }
template<> Pocket::Font* Pocket::GameObject::AddComponent<Pocket::Font>() { AddComponent(20); return (Pocket::Font*) GetComponent(20); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Font>() { RemoveComponent(20); }
template<> Pocket::Font* Pocket::GameObject::CloneComponent<Pocket::Font>(GameObject* source) { CloneComponent(20, source); return (Pocket::Font*) GetComponent(20); }
template<> Pocket::Label* Pocket::GameObject::GetComponent<Pocket::Label>() { return (Pocket::Label*) GetComponent(21); }
template<> Pocket::Label* Pocket::GameObject::AddComponent<Pocket::Label>() { AddComponent(21); return (Pocket::Label*) GetComponent(21); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Label>() { RemoveComponent(21); }
template<> Pocket::Label* Pocket::GameObject::CloneComponent<Pocket::Label>(GameObject* source) { CloneComponent(21, source); return (Pocket::Label*) GetComponent(21); }
template<> Pocket::Cloner* Pocket::GameObject::GetComponent<Pocket::Cloner>() { return (Pocket::Cloner*) GetComponent(22); }
template<> Pocket::Cloner* Pocket::GameObject::AddComponent<Pocket::Cloner>() { AddComponent(22); return (Pocket::Cloner*) GetComponent(22); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Cloner>() { RemoveComponent(22); }
template<> Pocket::Cloner* Pocket::GameObject::CloneComponent<Pocket::Cloner>(GameObject* source) { CloneComponent(22, source); return (Pocket::Cloner*) GetComponent(22); }
template<> Pocket::CloneVariable* Pocket::GameObject::GetComponent<Pocket::CloneVariable>() { return (Pocket::CloneVariable*) GetComponent(23); }
template<> Pocket::CloneVariable* Pocket::GameObject::AddComponent<Pocket::CloneVariable>() { AddComponent(23); return (Pocket::CloneVariable*) GetComponent(23); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::CloneVariable>() { RemoveComponent(23); }
template<> Pocket::CloneVariable* Pocket::GameObject::CloneComponent<Pocket::CloneVariable>(GameObject* source) { CloneComponent(23, source); return (Pocket::CloneVariable*) GetComponent(23); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::GetComponent<Pocket::EditorDropTarget>() { return (Pocket::EditorDropTarget*) GetComponent(24); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::AddComponent<Pocket::EditorDropTarget>() { AddComponent(24); return (Pocket::EditorDropTarget*) GetComponent(24); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::EditorDropTarget>() { RemoveComponent(24); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::CloneComponent<Pocket::EditorDropTarget>(GameObject* source) { CloneComponent(24, source); return (Pocket::EditorDropTarget*) GetComponent(24); }
template<> Pocket::ProjectSettings* Pocket::GameObject::GetComponent<Pocket::ProjectSettings>() { return (Pocket::ProjectSettings*) GetComponent(25); }
template<> Pocket::ProjectSettings* Pocket::GameObject::AddComponent<Pocket::ProjectSettings>() { AddComponent(25); return (Pocket::ProjectSettings*) GetComponent(25); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ProjectSettings>() { RemoveComponent(25); }
template<> Pocket::ProjectSettings* Pocket::GameObject::CloneComponent<Pocket::ProjectSettings>(GameObject* source) { CloneComponent(25, source); return (Pocket::ProjectSettings*) GetComponent(25); }
template<> Pocket::InputMapper* Pocket::GameObject::GetComponent<Pocket::InputMapper>() { return (Pocket::InputMapper*) GetComponent(26); }
template<> Pocket::InputMapper* Pocket::GameObject::AddComponent<Pocket::InputMapper>() { AddComponent(26); return (Pocket::InputMapper*) GetComponent(26); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::InputMapper>() { RemoveComponent(26); }
template<> Pocket::InputMapper* Pocket::GameObject::CloneComponent<Pocket::InputMapper>(GameObject* source) { CloneComponent(26, source); return (Pocket::InputMapper*) GetComponent(26); }
template<> Pocket::InputController* Pocket::GameObject::GetComponent<Pocket::InputController>() { return (Pocket::InputController*) GetComponent(27); }
template<> Pocket::InputController* Pocket::GameObject::AddComponent<Pocket::InputController>() { AddComponent(27); return (Pocket::InputController*) GetComponent(27); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::InputController>() { RemoveComponent(27); }
template<> Pocket::InputController* Pocket::GameObject::CloneComponent<Pocket::InputController>(GameObject* source) { CloneComponent(27, source); return (Pocket::InputController*) GetComponent(27); }
template<> Pocket::Switch* Pocket::GameObject::GetComponent<Pocket::Switch>() { return (Pocket::Switch*) GetComponent(28); }
template<> Pocket::Switch* Pocket::GameObject::AddComponent<Pocket::Switch>() { AddComponent(28); return (Pocket::Switch*) GetComponent(28); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Switch>() { RemoveComponent(28); }
template<> Pocket::Switch* Pocket::GameObject::CloneComponent<Pocket::Switch>(GameObject* source) { CloneComponent(28, source); return (Pocket::Switch*) GetComponent(28); }
template<> Pocket::SwitchEnabler* Pocket::GameObject::GetComponent<Pocket::SwitchEnabler>() { return (Pocket::SwitchEnabler*) GetComponent(29); }
template<> Pocket::SwitchEnabler* Pocket::GameObject::AddComponent<Pocket::SwitchEnabler>() { AddComponent(29); return (Pocket::SwitchEnabler*) GetComponent(29); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SwitchEnabler>() { RemoveComponent(29); }
template<> Pocket::SwitchEnabler* Pocket::GameObject::CloneComponent<Pocket::SwitchEnabler>(GameObject* source) { CloneComponent(29, source); return (Pocket::SwitchEnabler*) GetComponent(29); }
template<> Pocket::SlicedQuad* Pocket::GameObject::GetComponent<Pocket::SlicedQuad>() { return (Pocket::SlicedQuad*) GetComponent(30); }
template<> Pocket::SlicedQuad* Pocket::GameObject::AddComponent<Pocket::SlicedQuad>() { AddComponent(30); return (Pocket::SlicedQuad*) GetComponent(30); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SlicedQuad>() { RemoveComponent(30); }
template<> Pocket::SlicedQuad* Pocket::GameObject::CloneComponent<Pocket::SlicedQuad>(GameObject* source) { CloneComponent(30, source); return (Pocket::SlicedQuad*) GetComponent(30); }
template<> Pocket::SlicedTexture* Pocket::GameObject::GetComponent<Pocket::SlicedTexture>() { return (Pocket::SlicedTexture*) GetComponent(31); }
template<> Pocket::SlicedTexture* Pocket::GameObject::AddComponent<Pocket::SlicedTexture>() { AddComponent(31); return (Pocket::SlicedTexture*) GetComponent(31); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SlicedTexture>() { RemoveComponent(31); }
template<> Pocket::SlicedTexture* Pocket::GameObject::CloneComponent<Pocket::SlicedTexture>(GameObject* source) { CloneComponent(31, source); return (Pocket::SlicedTexture*) GetComponent(31); }
template<> Pocket::Trigger* Pocket::GameObject::GetComponent<Pocket::Trigger>() { return (Pocket::Trigger*) GetComponent(32); }
template<> Pocket::Trigger* Pocket::GameObject::AddComponent<Pocket::Trigger>() { AddComponent(32); return (Pocket::Trigger*) GetComponent(32); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Trigger>() { RemoveComponent(32); }
template<> Pocket::Trigger* Pocket::GameObject::CloneComponent<Pocket::Trigger>(GameObject* source) { CloneComponent(32, source); return (Pocket::Trigger*) GetComponent(32); }
template<> Pocket::SceneManager* Pocket::GameObject::GetComponent<Pocket::SceneManager>() { return (Pocket::SceneManager*) GetComponent(33); }
template<> Pocket::SceneManager* Pocket::GameObject::AddComponent<Pocket::SceneManager>() { AddComponent(33); return (Pocket::SceneManager*) GetComponent(33); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SceneManager>() { RemoveComponent(33); }
template<> Pocket::SceneManager* Pocket::GameObject::CloneComponent<Pocket::SceneManager>(GameObject* source) { CloneComponent(33, source); return (Pocket::SceneManager*) GetComponent(33); }
template<> Pocket::AssetLoader* Pocket::GameObject::GetComponent<Pocket::AssetLoader>() { return (Pocket::AssetLoader*) GetComponent(34); }
template<> Pocket::AssetLoader* Pocket::GameObject::AddComponent<Pocket::AssetLoader>() { AddComponent(34); return (Pocket::AssetLoader*) GetComponent(34); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::AssetLoader>() { RemoveComponent(34); }
template<> Pocket::AssetLoader* Pocket::GameObject::CloneComponent<Pocket::AssetLoader>(GameObject* source) { CloneComponent(34, source); return (Pocket::AssetLoader*) GetComponent(34); }
template<> Pocket::ShaderComponent* Pocket::GameObject::GetComponent<Pocket::ShaderComponent>() { return (Pocket::ShaderComponent*) GetComponent(35); }
template<> Pocket::ShaderComponent* Pocket::GameObject::AddComponent<Pocket::ShaderComponent>() { AddComponent(35); return (Pocket::ShaderComponent*) GetComponent(35); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ShaderComponent>() { RemoveComponent(35); }
template<> Pocket::ShaderComponent* Pocket::GameObject::CloneComponent<Pocket::ShaderComponent>(GameObject* source) { CloneComponent(35, source); return (Pocket::ShaderComponent*) GetComponent(35); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::GetComponent<Pocket::FirstPersonMover>() { return (Pocket::FirstPersonMover*) GetComponent(36); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::AddComponent<Pocket::FirstPersonMover>() { AddComponent(36); return (Pocket::FirstPersonMover*) GetComponent(36); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FirstPersonMover>() { RemoveComponent(36); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::CloneComponent<Pocket::FirstPersonMover>(GameObject* source) { CloneComponent(36, source); return (Pocket::FirstPersonMover*) GetComponent(36); }
template<> Pocket::LineRenderer* Pocket::GameObject::GetComponent<Pocket::LineRenderer>() { return (Pocket::LineRenderer*) GetComponent(38); }
template<> Pocket::LineRenderer* Pocket::GameObject::AddComponent<Pocket::LineRenderer>() { AddComponent(38); return (Pocket::LineRenderer*) GetComponent(38); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::LineRenderer>() { RemoveComponent(38); }
template<> Pocket::LineRenderer* Pocket::GameObject::CloneComponent<Pocket::LineRenderer>(GameObject* source) { CloneComponent(38, source); return (Pocket::LineRenderer*) GetComponent(38); }
template<> Pocket::DistanceScaler* Pocket::GameObject::GetComponent<Pocket::DistanceScaler>() { return (Pocket::DistanceScaler*) GetComponent(40); }
template<> Pocket::DistanceScaler* Pocket::GameObject::AddComponent<Pocket::DistanceScaler>() { AddComponent(40); return (Pocket::DistanceScaler*) GetComponent(40); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::DistanceScaler>() { RemoveComponent(40); }
template<> Pocket::DistanceScaler* Pocket::GameObject::CloneComponent<Pocket::DistanceScaler>(GameObject* source) { CloneComponent(40, source); return (Pocket::DistanceScaler*) GetComponent(40); }
template<> Pocket::FileSystemListener* Pocket::GameObject::GetComponent<Pocket::FileSystemListener>() { return (Pocket::FileSystemListener*) GetComponent(41); }
template<> Pocket::FileSystemListener* Pocket::GameObject::AddComponent<Pocket::FileSystemListener>() { AddComponent(41); return (Pocket::FileSystemListener*) GetComponent(41); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FileSystemListener>() { RemoveComponent(41); }
template<> Pocket::FileSystemListener* Pocket::GameObject::CloneComponent<Pocket::FileSystemListener>(GameObject* source) { CloneComponent(41, source); return (Pocket::FileSystemListener*) GetComponent(41); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::GetComponent<Pocket::VirtualTreeList>() { return (Pocket::VirtualTreeList*) GetComponent(42); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::AddComponent<Pocket::VirtualTreeList>() { AddComponent(42); return (Pocket::VirtualTreeList*) GetComponent(42); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::VirtualTreeList>() { RemoveComponent(42); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::CloneComponent<Pocket::VirtualTreeList>(GameObject* source) { CloneComponent(42, source); return (Pocket::VirtualTreeList*) GetComponent(42); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::GetComponent<Pocket::VirtualTreeListSpawner>() { return (Pocket::VirtualTreeListSpawner*) GetComponent(43); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::AddComponent<Pocket::VirtualTreeListSpawner>() { AddComponent(43); return (Pocket::VirtualTreeListSpawner*) GetComponent(43); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::VirtualTreeListSpawner>() { RemoveComponent(43); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::CloneComponent<Pocket::VirtualTreeListSpawner>(GameObject* source) { CloneComponent(43, source); return (Pocket::VirtualTreeListSpawner*) GetComponent(43); }
template<> Pocket::FieldEditor* Pocket::GameObject::GetComponent<Pocket::FieldEditor>() { return (Pocket::FieldEditor*) GetComponent(44); }
template<> Pocket::FieldEditor* Pocket::GameObject::AddComponent<Pocket::FieldEditor>() { AddComponent(44); return (Pocket::FieldEditor*) GetComponent(44); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FieldEditor>() { RemoveComponent(44); }
template<> Pocket::FieldEditor* Pocket::GameObject::CloneComponent<Pocket::FieldEditor>(GameObject* source) { CloneComponent(44, source); return (Pocket::FieldEditor*) GetComponent(44); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::GetComponent<Pocket::GameObjectEditor>() { return (Pocket::GameObjectEditor*) GetComponent(45); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::AddComponent<Pocket::GameObjectEditor>() { AddComponent(45); return (Pocket::GameObjectEditor*) GetComponent(45); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::GameObjectEditor>() { RemoveComponent(45); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::CloneComponent<Pocket::GameObjectEditor>(GameObject* source) { CloneComponent(45, source); return (Pocket::GameObjectEditor*) GetComponent(45); }
template<> Pocket::AssetImporter* Pocket::GameObject::GetComponent<Pocket::AssetImporter>() { return (Pocket::AssetImporter*) GetComponent(46); }
template<> Pocket::AssetImporter* Pocket::GameObject::AddComponent<Pocket::AssetImporter>() { AddComponent(46); return (Pocket::AssetImporter*) GetComponent(46); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::AssetImporter>() { RemoveComponent(46); }
template<> Pocket::AssetImporter* Pocket::GameObject::CloneComponent<Pocket::AssetImporter>(GameObject* source) { CloneComponent(46, source); return (Pocket::AssetImporter*) GetComponent(46); }
template<> Pocket::FilePath* Pocket::GameObject::GetComponent<Pocket::FilePath>() { return (Pocket::FilePath*) GetComponent(47); }
template<> Pocket::FilePath* Pocket::GameObject::AddComponent<Pocket::FilePath>() { AddComponent(47); return (Pocket::FilePath*) GetComponent(47); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FilePath>() { RemoveComponent(47); }
template<> Pocket::FilePath* Pocket::GameObject::CloneComponent<Pocket::FilePath>(GameObject* source) { CloneComponent(47, source); return (Pocket::FilePath*) GetComponent(47); }
using Script_RectCollider = RectCollider;
template<> Script_RectCollider* Pocket::GameObject::GetComponent<Script_RectCollider>() { return (Script_RectCollider*) GetComponent(48); }
template<> Script_RectCollider* Pocket::GameObject::AddComponent<Script_RectCollider>() { AddComponent(48); return (Script_RectCollider*) GetComponent(48); }
template<> void Pocket::GameObject::RemoveComponent<Script_RectCollider>() { RemoveComponent(48); }
template<> Script_RectCollider* Pocket::GameObject::CloneComponent<Script_RectCollider>(GameObject* source) { CloneComponent(48, source); return (Script_RectCollider*) GetComponent(48); }
using Script_Rotator = Rotator;
template<> Script_Rotator* Pocket::GameObject::GetComponent<Script_Rotator>() { return (Script_Rotator*) GetComponent(49); }
template<> Script_Rotator* Pocket::GameObject::AddComponent<Script_Rotator>() { AddComponent(49); return (Script_Rotator*) GetComponent(49); }
template<> void Pocket::GameObject::RemoveComponent<Script_Rotator>() { RemoveComponent(49); }
template<> Script_Rotator* Pocket::GameObject::CloneComponent<Script_Rotator>(GameObject* source) { CloneComponent(49, source); return (Script_Rotator*) GetComponent(49); }
using Script_TransformController = TransformController;
template<> Script_TransformController* Pocket::GameObject::GetComponent<Script_TransformController>() { return (Script_TransformController*) GetComponent(50); }
template<> Script_TransformController* Pocket::GameObject::AddComponent<Script_TransformController>() { AddComponent(50); return (Script_TransformController*) GetComponent(50); }
template<> void Pocket::GameObject::RemoveComponent<Script_TransformController>() { RemoveComponent(50); }
template<> Script_TransformController* Pocket::GameObject::CloneComponent<Script_TransformController>(GameObject* source) { CloneComponent(50, source); return (Script_TransformController*) GetComponent(50); }
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
#include "RectCollider.hpp"
#include "Rotator.hpp"
#include "TransformController.hpp"
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
   return 4;
}
extern "C" IGameSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 59: return new RectCollisionSystem();
      case 60: return new RotatorSystem();
      case 61: return new TransformControllerSystem();
      case 62: return new VelocityCollider();
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
      case 48: return new RectCollider();
      case 49: return new Rotator();
      case 50: return new TransformController();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 48: { delete ((RectCollider*)component); break; }
      case 49: { delete ((Rotator*)component); break; }
      case 50: { delete ((TransformController*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 48: { RectCollider* co = (RectCollider*)c; 
      RectCollider* so = ((RectCollider*)s);
        co->operator=(*so);             break; }
      case 49: { Rotator* co = (Rotator*)c; 
      Rotator* so = ((Rotator*)s);
        co->operator=(*so);             break; }
      case 50: { TransformController* co = (TransformController*)c; 
      TransformController* so = ((TransformController*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 48: {
      RectCollider* component = (RectCollider*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "RectCollider";
	      info->AddField(component->collisions, "collisions");
	      info->AddField(component->previousCollisions, "previousCollisions");
      return info;
      break; }
      case 49: {
      Rotator* component = (Rotator*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Rotator";
	      info->AddField(component->speed, "speed");
      return info;
      break; }
      case 50: {
      TransformController* component = (TransformController*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "TransformController";
	      info->AddField(component->speed, "speed");
      return info;
      break; }
      default: return 0;
   }
}
extern "C" void DeleteTypeInfo(Pocket::TypeInfo* typeInfo) {
delete typeInfo;
}
