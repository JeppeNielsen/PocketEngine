namespace Pocket {
struct Atlas;
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
struct Draggable;
struct Layouter;
struct Colorable;
struct Velocity;
struct Limitable;
struct SelectedColorer;
struct Selectable;
struct Droppable;
struct Font;
struct Label;
struct TouchableCanceller;
struct Cloner;
struct CloneVariable;
struct EditorDropTarget;
struct ProjectSettings;
struct FilePath;
struct Spawner;
struct Animation;
struct Animator;
struct AssetLoader;
struct ShaderComponent;
struct Sound;
struct SoundEmitter;
struct SoundListener;
struct ParticleEmitter;
struct ParticleEffect;
struct SlicedQuad;
struct SlicedTexture;
struct InputMapper;
struct InputController;
struct FirstPersonMover;
struct RigidBody;
struct RigidBody2d;
struct Joint2d;
struct DistanceScaler;
struct LineRenderer;
struct SceneManager;
struct Trigger;
struct LogMessage;
struct FileSystemListener;
struct AssetImporter;
}
struct KillTrigger;
struct RectCollider;
struct Rotator;
struct Score;
struct ScoreTrigger;
struct TransformController;

#include "GameSystem.hpp"
template<> Pocket::Atlas* Pocket::GameObject::GetComponent<Pocket::Atlas>() { return (Pocket::Atlas*) GetComponent(0); }
template<> Pocket::Atlas* Pocket::GameObject::AddComponent<Pocket::Atlas>() { AddComponent(0); return (Pocket::Atlas*) GetComponent(0); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Atlas>() { RemoveComponent(0); }
template<> Pocket::Atlas* Pocket::GameObject::CloneComponent<Pocket::Atlas>(GameObject* source) { CloneComponent(0, source); return (Pocket::Atlas*) GetComponent(0); }
template<> Pocket::Atlas* Pocket::GameObject::ReplaceComponent<Pocket::Atlas>(GameObject* source) { ReplaceComponent(0, source); return (Pocket::Atlas*) GetComponent(0); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Atlas>(bool enable) { EnableComponent(0, enable); }
template<> Pocket::Transform* Pocket::GameObject::GetComponent<Pocket::Transform>() { return (Pocket::Transform*) GetComponent(1); }
template<> Pocket::Transform* Pocket::GameObject::AddComponent<Pocket::Transform>() { AddComponent(1); return (Pocket::Transform*) GetComponent(1); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Transform>() { RemoveComponent(1); }
template<> Pocket::Transform* Pocket::GameObject::CloneComponent<Pocket::Transform>(GameObject* source) { CloneComponent(1, source); return (Pocket::Transform*) GetComponent(1); }
template<> Pocket::Transform* Pocket::GameObject::ReplaceComponent<Pocket::Transform>(GameObject* source) { ReplaceComponent(1, source); return (Pocket::Transform*) GetComponent(1); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Transform>(bool enable) { EnableComponent(1, enable); }
template<> Pocket::Mesh* Pocket::GameObject::GetComponent<Pocket::Mesh>() { return (Pocket::Mesh*) GetComponent(2); }
template<> Pocket::Mesh* Pocket::GameObject::AddComponent<Pocket::Mesh>() { AddComponent(2); return (Pocket::Mesh*) GetComponent(2); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Mesh>() { RemoveComponent(2); }
template<> Pocket::Mesh* Pocket::GameObject::CloneComponent<Pocket::Mesh>(GameObject* source) { CloneComponent(2, source); return (Pocket::Mesh*) GetComponent(2); }
template<> Pocket::Mesh* Pocket::GameObject::ReplaceComponent<Pocket::Mesh>(GameObject* source) { ReplaceComponent(2, source); return (Pocket::Mesh*) GetComponent(2); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Mesh>(bool enable) { EnableComponent(2, enable); }
template<> Pocket::Renderable* Pocket::GameObject::GetComponent<Pocket::Renderable>() { return (Pocket::Renderable*) GetComponent(3); }
template<> Pocket::Renderable* Pocket::GameObject::AddComponent<Pocket::Renderable>() { AddComponent(3); return (Pocket::Renderable*) GetComponent(3); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Renderable>() { RemoveComponent(3); }
template<> Pocket::Renderable* Pocket::GameObject::CloneComponent<Pocket::Renderable>(GameObject* source) { CloneComponent(3, source); return (Pocket::Renderable*) GetComponent(3); }
template<> Pocket::Renderable* Pocket::GameObject::ReplaceComponent<Pocket::Renderable>(GameObject* source) { ReplaceComponent(3, source); return (Pocket::Renderable*) GetComponent(3); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Renderable>(bool enable) { EnableComponent(3, enable); }
template<> Pocket::Camera* Pocket::GameObject::GetComponent<Pocket::Camera>() { return (Pocket::Camera*) GetComponent(4); }
template<> Pocket::Camera* Pocket::GameObject::AddComponent<Pocket::Camera>() { AddComponent(4); return (Pocket::Camera*) GetComponent(4); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Camera>() { RemoveComponent(4); }
template<> Pocket::Camera* Pocket::GameObject::CloneComponent<Pocket::Camera>(GameObject* source) { CloneComponent(4, source); return (Pocket::Camera*) GetComponent(4); }
template<> Pocket::Camera* Pocket::GameObject::ReplaceComponent<Pocket::Camera>(GameObject* source) { ReplaceComponent(4, source); return (Pocket::Camera*) GetComponent(4); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Camera>(bool enable) { EnableComponent(4, enable); }
template<> Pocket::TextureComponent* Pocket::GameObject::GetComponent<Pocket::TextureComponent>() { return (Pocket::TextureComponent*) GetComponent(5); }
template<> Pocket::TextureComponent* Pocket::GameObject::AddComponent<Pocket::TextureComponent>() { AddComponent(5); return (Pocket::TextureComponent*) GetComponent(5); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::TextureComponent>() { RemoveComponent(5); }
template<> Pocket::TextureComponent* Pocket::GameObject::CloneComponent<Pocket::TextureComponent>(GameObject* source) { CloneComponent(5, source); return (Pocket::TextureComponent*) GetComponent(5); }
template<> Pocket::TextureComponent* Pocket::GameObject::ReplaceComponent<Pocket::TextureComponent>(GameObject* source) { ReplaceComponent(5, source); return (Pocket::TextureComponent*) GetComponent(5); }
template<> void Pocket::GameObject::EnableComponent<Pocket::TextureComponent>(bool enable) { EnableComponent(5, enable); }
template<> Pocket::Orderable* Pocket::GameObject::GetComponent<Pocket::Orderable>() { return (Pocket::Orderable*) GetComponent(6); }
template<> Pocket::Orderable* Pocket::GameObject::AddComponent<Pocket::Orderable>() { AddComponent(6); return (Pocket::Orderable*) GetComponent(6); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Orderable>() { RemoveComponent(6); }
template<> Pocket::Orderable* Pocket::GameObject::CloneComponent<Pocket::Orderable>(GameObject* source) { CloneComponent(6, source); return (Pocket::Orderable*) GetComponent(6); }
template<> Pocket::Orderable* Pocket::GameObject::ReplaceComponent<Pocket::Orderable>(GameObject* source) { ReplaceComponent(6, source); return (Pocket::Orderable*) GetComponent(6); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Orderable>(bool enable) { EnableComponent(6, enable); }
template<> Pocket::TextBox* Pocket::GameObject::GetComponent<Pocket::TextBox>() { return (Pocket::TextBox*) GetComponent(7); }
template<> Pocket::TextBox* Pocket::GameObject::AddComponent<Pocket::TextBox>() { AddComponent(7); return (Pocket::TextBox*) GetComponent(7); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::TextBox>() { RemoveComponent(7); }
template<> Pocket::TextBox* Pocket::GameObject::CloneComponent<Pocket::TextBox>(GameObject* source) { CloneComponent(7, source); return (Pocket::TextBox*) GetComponent(7); }
template<> Pocket::TextBox* Pocket::GameObject::ReplaceComponent<Pocket::TextBox>(GameObject* source) { ReplaceComponent(7, source); return (Pocket::TextBox*) GetComponent(7); }
template<> void Pocket::GameObject::EnableComponent<Pocket::TextBox>(bool enable) { EnableComponent(7, enable); }
template<> Pocket::Touchable* Pocket::GameObject::GetComponent<Pocket::Touchable>() { return (Pocket::Touchable*) GetComponent(8); }
template<> Pocket::Touchable* Pocket::GameObject::AddComponent<Pocket::Touchable>() { AddComponent(8); return (Pocket::Touchable*) GetComponent(8); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Touchable>() { RemoveComponent(8); }
template<> Pocket::Touchable* Pocket::GameObject::CloneComponent<Pocket::Touchable>(GameObject* source) { CloneComponent(8, source); return (Pocket::Touchable*) GetComponent(8); }
template<> Pocket::Touchable* Pocket::GameObject::ReplaceComponent<Pocket::Touchable>(GameObject* source) { ReplaceComponent(8, source); return (Pocket::Touchable*) GetComponent(8); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Touchable>(bool enable) { EnableComponent(8, enable); }
template<> Pocket::Sprite* Pocket::GameObject::GetComponent<Pocket::Sprite>() { return (Pocket::Sprite*) GetComponent(9); }
template<> Pocket::Sprite* Pocket::GameObject::AddComponent<Pocket::Sprite>() { AddComponent(9); return (Pocket::Sprite*) GetComponent(9); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sprite>() { RemoveComponent(9); }
template<> Pocket::Sprite* Pocket::GameObject::CloneComponent<Pocket::Sprite>(GameObject* source) { CloneComponent(9, source); return (Pocket::Sprite*) GetComponent(9); }
template<> Pocket::Sprite* Pocket::GameObject::ReplaceComponent<Pocket::Sprite>(GameObject* source) { ReplaceComponent(9, source); return (Pocket::Sprite*) GetComponent(9); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Sprite>(bool enable) { EnableComponent(9, enable); }
template<> Pocket::Sizeable* Pocket::GameObject::GetComponent<Pocket::Sizeable>() { return (Pocket::Sizeable*) GetComponent(10); }
template<> Pocket::Sizeable* Pocket::GameObject::AddComponent<Pocket::Sizeable>() { AddComponent(10); return (Pocket::Sizeable*) GetComponent(10); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sizeable>() { RemoveComponent(10); }
template<> Pocket::Sizeable* Pocket::GameObject::CloneComponent<Pocket::Sizeable>(GameObject* source) { CloneComponent(10, source); return (Pocket::Sizeable*) GetComponent(10); }
template<> Pocket::Sizeable* Pocket::GameObject::ReplaceComponent<Pocket::Sizeable>(GameObject* source) { ReplaceComponent(10, source); return (Pocket::Sizeable*) GetComponent(10); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Sizeable>(bool enable) { EnableComponent(10, enable); }
template<> Pocket::Draggable* Pocket::GameObject::GetComponent<Pocket::Draggable>() { return (Pocket::Draggable*) GetComponent(11); }
template<> Pocket::Draggable* Pocket::GameObject::AddComponent<Pocket::Draggable>() { AddComponent(11); return (Pocket::Draggable*) GetComponent(11); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Draggable>() { RemoveComponent(11); }
template<> Pocket::Draggable* Pocket::GameObject::CloneComponent<Pocket::Draggable>(GameObject* source) { CloneComponent(11, source); return (Pocket::Draggable*) GetComponent(11); }
template<> Pocket::Draggable* Pocket::GameObject::ReplaceComponent<Pocket::Draggable>(GameObject* source) { ReplaceComponent(11, source); return (Pocket::Draggable*) GetComponent(11); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Draggable>(bool enable) { EnableComponent(11, enable); }
template<> Pocket::Layouter* Pocket::GameObject::GetComponent<Pocket::Layouter>() { return (Pocket::Layouter*) GetComponent(12); }
template<> Pocket::Layouter* Pocket::GameObject::AddComponent<Pocket::Layouter>() { AddComponent(12); return (Pocket::Layouter*) GetComponent(12); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Layouter>() { RemoveComponent(12); }
template<> Pocket::Layouter* Pocket::GameObject::CloneComponent<Pocket::Layouter>(GameObject* source) { CloneComponent(12, source); return (Pocket::Layouter*) GetComponent(12); }
template<> Pocket::Layouter* Pocket::GameObject::ReplaceComponent<Pocket::Layouter>(GameObject* source) { ReplaceComponent(12, source); return (Pocket::Layouter*) GetComponent(12); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Layouter>(bool enable) { EnableComponent(12, enable); }
template<> Pocket::Colorable* Pocket::GameObject::GetComponent<Pocket::Colorable>() { return (Pocket::Colorable*) GetComponent(13); }
template<> Pocket::Colorable* Pocket::GameObject::AddComponent<Pocket::Colorable>() { AddComponent(13); return (Pocket::Colorable*) GetComponent(13); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Colorable>() { RemoveComponent(13); }
template<> Pocket::Colorable* Pocket::GameObject::CloneComponent<Pocket::Colorable>(GameObject* source) { CloneComponent(13, source); return (Pocket::Colorable*) GetComponent(13); }
template<> Pocket::Colorable* Pocket::GameObject::ReplaceComponent<Pocket::Colorable>(GameObject* source) { ReplaceComponent(13, source); return (Pocket::Colorable*) GetComponent(13); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Colorable>(bool enable) { EnableComponent(13, enable); }
template<> Pocket::Velocity* Pocket::GameObject::GetComponent<Pocket::Velocity>() { return (Pocket::Velocity*) GetComponent(14); }
template<> Pocket::Velocity* Pocket::GameObject::AddComponent<Pocket::Velocity>() { AddComponent(14); return (Pocket::Velocity*) GetComponent(14); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Velocity>() { RemoveComponent(14); }
template<> Pocket::Velocity* Pocket::GameObject::CloneComponent<Pocket::Velocity>(GameObject* source) { CloneComponent(14, source); return (Pocket::Velocity*) GetComponent(14); }
template<> Pocket::Velocity* Pocket::GameObject::ReplaceComponent<Pocket::Velocity>(GameObject* source) { ReplaceComponent(14, source); return (Pocket::Velocity*) GetComponent(14); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Velocity>(bool enable) { EnableComponent(14, enable); }
template<> Pocket::Limitable* Pocket::GameObject::GetComponent<Pocket::Limitable>() { return (Pocket::Limitable*) GetComponent(15); }
template<> Pocket::Limitable* Pocket::GameObject::AddComponent<Pocket::Limitable>() { AddComponent(15); return (Pocket::Limitable*) GetComponent(15); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Limitable>() { RemoveComponent(15); }
template<> Pocket::Limitable* Pocket::GameObject::CloneComponent<Pocket::Limitable>(GameObject* source) { CloneComponent(15, source); return (Pocket::Limitable*) GetComponent(15); }
template<> Pocket::Limitable* Pocket::GameObject::ReplaceComponent<Pocket::Limitable>(GameObject* source) { ReplaceComponent(15, source); return (Pocket::Limitable*) GetComponent(15); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Limitable>(bool enable) { EnableComponent(15, enable); }
template<> Pocket::SelectedColorer* Pocket::GameObject::GetComponent<Pocket::SelectedColorer>() { return (Pocket::SelectedColorer*) GetComponent(16); }
template<> Pocket::SelectedColorer* Pocket::GameObject::AddComponent<Pocket::SelectedColorer>() { AddComponent(16); return (Pocket::SelectedColorer*) GetComponent(16); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SelectedColorer>() { RemoveComponent(16); }
template<> Pocket::SelectedColorer* Pocket::GameObject::CloneComponent<Pocket::SelectedColorer>(GameObject* source) { CloneComponent(16, source); return (Pocket::SelectedColorer*) GetComponent(16); }
template<> Pocket::SelectedColorer* Pocket::GameObject::ReplaceComponent<Pocket::SelectedColorer>(GameObject* source) { ReplaceComponent(16, source); return (Pocket::SelectedColorer*) GetComponent(16); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SelectedColorer>(bool enable) { EnableComponent(16, enable); }
template<> Pocket::Selectable* Pocket::GameObject::GetComponent<Pocket::Selectable>() { return (Pocket::Selectable*) GetComponent(17); }
template<> Pocket::Selectable* Pocket::GameObject::AddComponent<Pocket::Selectable>() { AddComponent(17); return (Pocket::Selectable*) GetComponent(17); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Selectable>() { RemoveComponent(17); }
template<> Pocket::Selectable* Pocket::GameObject::CloneComponent<Pocket::Selectable>(GameObject* source) { CloneComponent(17, source); return (Pocket::Selectable*) GetComponent(17); }
template<> Pocket::Selectable* Pocket::GameObject::ReplaceComponent<Pocket::Selectable>(GameObject* source) { ReplaceComponent(17, source); return (Pocket::Selectable*) GetComponent(17); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Selectable>(bool enable) { EnableComponent(17, enable); }
template<> Pocket::Droppable* Pocket::GameObject::GetComponent<Pocket::Droppable>() { return (Pocket::Droppable*) GetComponent(18); }
template<> Pocket::Droppable* Pocket::GameObject::AddComponent<Pocket::Droppable>() { AddComponent(18); return (Pocket::Droppable*) GetComponent(18); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Droppable>() { RemoveComponent(18); }
template<> Pocket::Droppable* Pocket::GameObject::CloneComponent<Pocket::Droppable>(GameObject* source) { CloneComponent(18, source); return (Pocket::Droppable*) GetComponent(18); }
template<> Pocket::Droppable* Pocket::GameObject::ReplaceComponent<Pocket::Droppable>(GameObject* source) { ReplaceComponent(18, source); return (Pocket::Droppable*) GetComponent(18); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Droppable>(bool enable) { EnableComponent(18, enable); }
template<> Pocket::Font* Pocket::GameObject::GetComponent<Pocket::Font>() { return (Pocket::Font*) GetComponent(19); }
template<> Pocket::Font* Pocket::GameObject::AddComponent<Pocket::Font>() { AddComponent(19); return (Pocket::Font*) GetComponent(19); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Font>() { RemoveComponent(19); }
template<> Pocket::Font* Pocket::GameObject::CloneComponent<Pocket::Font>(GameObject* source) { CloneComponent(19, source); return (Pocket::Font*) GetComponent(19); }
template<> Pocket::Font* Pocket::GameObject::ReplaceComponent<Pocket::Font>(GameObject* source) { ReplaceComponent(19, source); return (Pocket::Font*) GetComponent(19); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Font>(bool enable) { EnableComponent(19, enable); }
template<> Pocket::Label* Pocket::GameObject::GetComponent<Pocket::Label>() { return (Pocket::Label*) GetComponent(20); }
template<> Pocket::Label* Pocket::GameObject::AddComponent<Pocket::Label>() { AddComponent(20); return (Pocket::Label*) GetComponent(20); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Label>() { RemoveComponent(20); }
template<> Pocket::Label* Pocket::GameObject::CloneComponent<Pocket::Label>(GameObject* source) { CloneComponent(20, source); return (Pocket::Label*) GetComponent(20); }
template<> Pocket::Label* Pocket::GameObject::ReplaceComponent<Pocket::Label>(GameObject* source) { ReplaceComponent(20, source); return (Pocket::Label*) GetComponent(20); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Label>(bool enable) { EnableComponent(20, enable); }
template<> Pocket::TouchableCanceller* Pocket::GameObject::GetComponent<Pocket::TouchableCanceller>() { return (Pocket::TouchableCanceller*) GetComponent(25); }
template<> Pocket::TouchableCanceller* Pocket::GameObject::AddComponent<Pocket::TouchableCanceller>() { AddComponent(25); return (Pocket::TouchableCanceller*) GetComponent(25); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::TouchableCanceller>() { RemoveComponent(25); }
template<> Pocket::TouchableCanceller* Pocket::GameObject::CloneComponent<Pocket::TouchableCanceller>(GameObject* source) { CloneComponent(25, source); return (Pocket::TouchableCanceller*) GetComponent(25); }
template<> Pocket::TouchableCanceller* Pocket::GameObject::ReplaceComponent<Pocket::TouchableCanceller>(GameObject* source) { ReplaceComponent(25, source); return (Pocket::TouchableCanceller*) GetComponent(25); }
template<> void Pocket::GameObject::EnableComponent<Pocket::TouchableCanceller>(bool enable) { EnableComponent(25, enable); }
template<> Pocket::Cloner* Pocket::GameObject::GetComponent<Pocket::Cloner>() { return (Pocket::Cloner*) GetComponent(27); }
template<> Pocket::Cloner* Pocket::GameObject::AddComponent<Pocket::Cloner>() { AddComponent(27); return (Pocket::Cloner*) GetComponent(27); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Cloner>() { RemoveComponent(27); }
template<> Pocket::Cloner* Pocket::GameObject::CloneComponent<Pocket::Cloner>(GameObject* source) { CloneComponent(27, source); return (Pocket::Cloner*) GetComponent(27); }
template<> Pocket::Cloner* Pocket::GameObject::ReplaceComponent<Pocket::Cloner>(GameObject* source) { ReplaceComponent(27, source); return (Pocket::Cloner*) GetComponent(27); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Cloner>(bool enable) { EnableComponent(27, enable); }
template<> Pocket::CloneVariable* Pocket::GameObject::GetComponent<Pocket::CloneVariable>() { return (Pocket::CloneVariable*) GetComponent(28); }
template<> Pocket::CloneVariable* Pocket::GameObject::AddComponent<Pocket::CloneVariable>() { AddComponent(28); return (Pocket::CloneVariable*) GetComponent(28); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::CloneVariable>() { RemoveComponent(28); }
template<> Pocket::CloneVariable* Pocket::GameObject::CloneComponent<Pocket::CloneVariable>(GameObject* source) { CloneComponent(28, source); return (Pocket::CloneVariable*) GetComponent(28); }
template<> Pocket::CloneVariable* Pocket::GameObject::ReplaceComponent<Pocket::CloneVariable>(GameObject* source) { ReplaceComponent(28, source); return (Pocket::CloneVariable*) GetComponent(28); }
template<> void Pocket::GameObject::EnableComponent<Pocket::CloneVariable>(bool enable) { EnableComponent(28, enable); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::GetComponent<Pocket::EditorDropTarget>() { return (Pocket::EditorDropTarget*) GetComponent(29); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::AddComponent<Pocket::EditorDropTarget>() { AddComponent(29); return (Pocket::EditorDropTarget*) GetComponent(29); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::EditorDropTarget>() { RemoveComponent(29); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::CloneComponent<Pocket::EditorDropTarget>(GameObject* source) { CloneComponent(29, source); return (Pocket::EditorDropTarget*) GetComponent(29); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::ReplaceComponent<Pocket::EditorDropTarget>(GameObject* source) { ReplaceComponent(29, source); return (Pocket::EditorDropTarget*) GetComponent(29); }
template<> void Pocket::GameObject::EnableComponent<Pocket::EditorDropTarget>(bool enable) { EnableComponent(29, enable); }
template<> Pocket::ProjectSettings* Pocket::GameObject::GetComponent<Pocket::ProjectSettings>() { return (Pocket::ProjectSettings*) GetComponent(30); }
template<> Pocket::ProjectSettings* Pocket::GameObject::AddComponent<Pocket::ProjectSettings>() { AddComponent(30); return (Pocket::ProjectSettings*) GetComponent(30); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ProjectSettings>() { RemoveComponent(30); }
template<> Pocket::ProjectSettings* Pocket::GameObject::CloneComponent<Pocket::ProjectSettings>(GameObject* source) { CloneComponent(30, source); return (Pocket::ProjectSettings*) GetComponent(30); }
template<> Pocket::ProjectSettings* Pocket::GameObject::ReplaceComponent<Pocket::ProjectSettings>(GameObject* source) { ReplaceComponent(30, source); return (Pocket::ProjectSettings*) GetComponent(30); }
template<> void Pocket::GameObject::EnableComponent<Pocket::ProjectSettings>(bool enable) { EnableComponent(30, enable); }
template<> Pocket::FilePath* Pocket::GameObject::GetComponent<Pocket::FilePath>() { return (Pocket::FilePath*) GetComponent(31); }
template<> Pocket::FilePath* Pocket::GameObject::AddComponent<Pocket::FilePath>() { AddComponent(31); return (Pocket::FilePath*) GetComponent(31); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FilePath>() { RemoveComponent(31); }
template<> Pocket::FilePath* Pocket::GameObject::CloneComponent<Pocket::FilePath>(GameObject* source) { CloneComponent(31, source); return (Pocket::FilePath*) GetComponent(31); }
template<> Pocket::FilePath* Pocket::GameObject::ReplaceComponent<Pocket::FilePath>(GameObject* source) { ReplaceComponent(31, source); return (Pocket::FilePath*) GetComponent(31); }
template<> void Pocket::GameObject::EnableComponent<Pocket::FilePath>(bool enable) { EnableComponent(31, enable); }
template<> Pocket::Spawner* Pocket::GameObject::GetComponent<Pocket::Spawner>() { return (Pocket::Spawner*) GetComponent(32); }
template<> Pocket::Spawner* Pocket::GameObject::AddComponent<Pocket::Spawner>() { AddComponent(32); return (Pocket::Spawner*) GetComponent(32); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Spawner>() { RemoveComponent(32); }
template<> Pocket::Spawner* Pocket::GameObject::CloneComponent<Pocket::Spawner>(GameObject* source) { CloneComponent(32, source); return (Pocket::Spawner*) GetComponent(32); }
template<> Pocket::Spawner* Pocket::GameObject::ReplaceComponent<Pocket::Spawner>(GameObject* source) { ReplaceComponent(32, source); return (Pocket::Spawner*) GetComponent(32); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Spawner>(bool enable) { EnableComponent(32, enable); }
template<> Pocket::Animation* Pocket::GameObject::GetComponent<Pocket::Animation>() { return (Pocket::Animation*) GetComponent(33); }
template<> Pocket::Animation* Pocket::GameObject::AddComponent<Pocket::Animation>() { AddComponent(33); return (Pocket::Animation*) GetComponent(33); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Animation>() { RemoveComponent(33); }
template<> Pocket::Animation* Pocket::GameObject::CloneComponent<Pocket::Animation>(GameObject* source) { CloneComponent(33, source); return (Pocket::Animation*) GetComponent(33); }
template<> Pocket::Animation* Pocket::GameObject::ReplaceComponent<Pocket::Animation>(GameObject* source) { ReplaceComponent(33, source); return (Pocket::Animation*) GetComponent(33); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Animation>(bool enable) { EnableComponent(33, enable); }
template<> Pocket::Animator* Pocket::GameObject::GetComponent<Pocket::Animator>() { return (Pocket::Animator*) GetComponent(34); }
template<> Pocket::Animator* Pocket::GameObject::AddComponent<Pocket::Animator>() { AddComponent(34); return (Pocket::Animator*) GetComponent(34); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Animator>() { RemoveComponent(34); }
template<> Pocket::Animator* Pocket::GameObject::CloneComponent<Pocket::Animator>(GameObject* source) { CloneComponent(34, source); return (Pocket::Animator*) GetComponent(34); }
template<> Pocket::Animator* Pocket::GameObject::ReplaceComponent<Pocket::Animator>(GameObject* source) { ReplaceComponent(34, source); return (Pocket::Animator*) GetComponent(34); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Animator>(bool enable) { EnableComponent(34, enable); }
template<> Pocket::AssetLoader* Pocket::GameObject::GetComponent<Pocket::AssetLoader>() { return (Pocket::AssetLoader*) GetComponent(35); }
template<> Pocket::AssetLoader* Pocket::GameObject::AddComponent<Pocket::AssetLoader>() { AddComponent(35); return (Pocket::AssetLoader*) GetComponent(35); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::AssetLoader>() { RemoveComponent(35); }
template<> Pocket::AssetLoader* Pocket::GameObject::CloneComponent<Pocket::AssetLoader>(GameObject* source) { CloneComponent(35, source); return (Pocket::AssetLoader*) GetComponent(35); }
template<> Pocket::AssetLoader* Pocket::GameObject::ReplaceComponent<Pocket::AssetLoader>(GameObject* source) { ReplaceComponent(35, source); return (Pocket::AssetLoader*) GetComponent(35); }
template<> void Pocket::GameObject::EnableComponent<Pocket::AssetLoader>(bool enable) { EnableComponent(35, enable); }
template<> Pocket::ShaderComponent* Pocket::GameObject::GetComponent<Pocket::ShaderComponent>() { return (Pocket::ShaderComponent*) GetComponent(36); }
template<> Pocket::ShaderComponent* Pocket::GameObject::AddComponent<Pocket::ShaderComponent>() { AddComponent(36); return (Pocket::ShaderComponent*) GetComponent(36); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ShaderComponent>() { RemoveComponent(36); }
template<> Pocket::ShaderComponent* Pocket::GameObject::CloneComponent<Pocket::ShaderComponent>(GameObject* source) { CloneComponent(36, source); return (Pocket::ShaderComponent*) GetComponent(36); }
template<> Pocket::ShaderComponent* Pocket::GameObject::ReplaceComponent<Pocket::ShaderComponent>(GameObject* source) { ReplaceComponent(36, source); return (Pocket::ShaderComponent*) GetComponent(36); }
template<> void Pocket::GameObject::EnableComponent<Pocket::ShaderComponent>(bool enable) { EnableComponent(36, enable); }
template<> Pocket::Sound* Pocket::GameObject::GetComponent<Pocket::Sound>() { return (Pocket::Sound*) GetComponent(37); }
template<> Pocket::Sound* Pocket::GameObject::AddComponent<Pocket::Sound>() { AddComponent(37); return (Pocket::Sound*) GetComponent(37); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sound>() { RemoveComponent(37); }
template<> Pocket::Sound* Pocket::GameObject::CloneComponent<Pocket::Sound>(GameObject* source) { CloneComponent(37, source); return (Pocket::Sound*) GetComponent(37); }
template<> Pocket::Sound* Pocket::GameObject::ReplaceComponent<Pocket::Sound>(GameObject* source) { ReplaceComponent(37, source); return (Pocket::Sound*) GetComponent(37); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Sound>(bool enable) { EnableComponent(37, enable); }
template<> Pocket::SoundEmitter* Pocket::GameObject::GetComponent<Pocket::SoundEmitter>() { return (Pocket::SoundEmitter*) GetComponent(38); }
template<> Pocket::SoundEmitter* Pocket::GameObject::AddComponent<Pocket::SoundEmitter>() { AddComponent(38); return (Pocket::SoundEmitter*) GetComponent(38); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SoundEmitter>() { RemoveComponent(38); }
template<> Pocket::SoundEmitter* Pocket::GameObject::CloneComponent<Pocket::SoundEmitter>(GameObject* source) { CloneComponent(38, source); return (Pocket::SoundEmitter*) GetComponent(38); }
template<> Pocket::SoundEmitter* Pocket::GameObject::ReplaceComponent<Pocket::SoundEmitter>(GameObject* source) { ReplaceComponent(38, source); return (Pocket::SoundEmitter*) GetComponent(38); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SoundEmitter>(bool enable) { EnableComponent(38, enable); }
template<> Pocket::SoundListener* Pocket::GameObject::GetComponent<Pocket::SoundListener>() { return (Pocket::SoundListener*) GetComponent(39); }
template<> Pocket::SoundListener* Pocket::GameObject::AddComponent<Pocket::SoundListener>() { AddComponent(39); return (Pocket::SoundListener*) GetComponent(39); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SoundListener>() { RemoveComponent(39); }
template<> Pocket::SoundListener* Pocket::GameObject::CloneComponent<Pocket::SoundListener>(GameObject* source) { CloneComponent(39, source); return (Pocket::SoundListener*) GetComponent(39); }
template<> Pocket::SoundListener* Pocket::GameObject::ReplaceComponent<Pocket::SoundListener>(GameObject* source) { ReplaceComponent(39, source); return (Pocket::SoundListener*) GetComponent(39); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SoundListener>(bool enable) { EnableComponent(39, enable); }
template<> Pocket::ParticleEmitter* Pocket::GameObject::GetComponent<Pocket::ParticleEmitter>() { return (Pocket::ParticleEmitter*) GetComponent(40); }
template<> Pocket::ParticleEmitter* Pocket::GameObject::AddComponent<Pocket::ParticleEmitter>() { AddComponent(40); return (Pocket::ParticleEmitter*) GetComponent(40); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ParticleEmitter>() { RemoveComponent(40); }
template<> Pocket::ParticleEmitter* Pocket::GameObject::CloneComponent<Pocket::ParticleEmitter>(GameObject* source) { CloneComponent(40, source); return (Pocket::ParticleEmitter*) GetComponent(40); }
template<> Pocket::ParticleEmitter* Pocket::GameObject::ReplaceComponent<Pocket::ParticleEmitter>(GameObject* source) { ReplaceComponent(40, source); return (Pocket::ParticleEmitter*) GetComponent(40); }
template<> void Pocket::GameObject::EnableComponent<Pocket::ParticleEmitter>(bool enable) { EnableComponent(40, enable); }
template<> Pocket::ParticleEffect* Pocket::GameObject::GetComponent<Pocket::ParticleEffect>() { return (Pocket::ParticleEffect*) GetComponent(41); }
template<> Pocket::ParticleEffect* Pocket::GameObject::AddComponent<Pocket::ParticleEffect>() { AddComponent(41); return (Pocket::ParticleEffect*) GetComponent(41); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ParticleEffect>() { RemoveComponent(41); }
template<> Pocket::ParticleEffect* Pocket::GameObject::CloneComponent<Pocket::ParticleEffect>(GameObject* source) { CloneComponent(41, source); return (Pocket::ParticleEffect*) GetComponent(41); }
template<> Pocket::ParticleEffect* Pocket::GameObject::ReplaceComponent<Pocket::ParticleEffect>(GameObject* source) { ReplaceComponent(41, source); return (Pocket::ParticleEffect*) GetComponent(41); }
template<> void Pocket::GameObject::EnableComponent<Pocket::ParticleEffect>(bool enable) { EnableComponent(41, enable); }
template<> Pocket::SlicedQuad* Pocket::GameObject::GetComponent<Pocket::SlicedQuad>() { return (Pocket::SlicedQuad*) GetComponent(42); }
template<> Pocket::SlicedQuad* Pocket::GameObject::AddComponent<Pocket::SlicedQuad>() { AddComponent(42); return (Pocket::SlicedQuad*) GetComponent(42); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SlicedQuad>() { RemoveComponent(42); }
template<> Pocket::SlicedQuad* Pocket::GameObject::CloneComponent<Pocket::SlicedQuad>(GameObject* source) { CloneComponent(42, source); return (Pocket::SlicedQuad*) GetComponent(42); }
template<> Pocket::SlicedQuad* Pocket::GameObject::ReplaceComponent<Pocket::SlicedQuad>(GameObject* source) { ReplaceComponent(42, source); return (Pocket::SlicedQuad*) GetComponent(42); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SlicedQuad>(bool enable) { EnableComponent(42, enable); }
template<> Pocket::SlicedTexture* Pocket::GameObject::GetComponent<Pocket::SlicedTexture>() { return (Pocket::SlicedTexture*) GetComponent(43); }
template<> Pocket::SlicedTexture* Pocket::GameObject::AddComponent<Pocket::SlicedTexture>() { AddComponent(43); return (Pocket::SlicedTexture*) GetComponent(43); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SlicedTexture>() { RemoveComponent(43); }
template<> Pocket::SlicedTexture* Pocket::GameObject::CloneComponent<Pocket::SlicedTexture>(GameObject* source) { CloneComponent(43, source); return (Pocket::SlicedTexture*) GetComponent(43); }
template<> Pocket::SlicedTexture* Pocket::GameObject::ReplaceComponent<Pocket::SlicedTexture>(GameObject* source) { ReplaceComponent(43, source); return (Pocket::SlicedTexture*) GetComponent(43); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SlicedTexture>(bool enable) { EnableComponent(43, enable); }
template<> Pocket::InputMapper* Pocket::GameObject::GetComponent<Pocket::InputMapper>() { return (Pocket::InputMapper*) GetComponent(44); }
template<> Pocket::InputMapper* Pocket::GameObject::AddComponent<Pocket::InputMapper>() { AddComponent(44); return (Pocket::InputMapper*) GetComponent(44); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::InputMapper>() { RemoveComponent(44); }
template<> Pocket::InputMapper* Pocket::GameObject::CloneComponent<Pocket::InputMapper>(GameObject* source) { CloneComponent(44, source); return (Pocket::InputMapper*) GetComponent(44); }
template<> Pocket::InputMapper* Pocket::GameObject::ReplaceComponent<Pocket::InputMapper>(GameObject* source) { ReplaceComponent(44, source); return (Pocket::InputMapper*) GetComponent(44); }
template<> void Pocket::GameObject::EnableComponent<Pocket::InputMapper>(bool enable) { EnableComponent(44, enable); }
template<> Pocket::InputController* Pocket::GameObject::GetComponent<Pocket::InputController>() { return (Pocket::InputController*) GetComponent(45); }
template<> Pocket::InputController* Pocket::GameObject::AddComponent<Pocket::InputController>() { AddComponent(45); return (Pocket::InputController*) GetComponent(45); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::InputController>() { RemoveComponent(45); }
template<> Pocket::InputController* Pocket::GameObject::CloneComponent<Pocket::InputController>(GameObject* source) { CloneComponent(45, source); return (Pocket::InputController*) GetComponent(45); }
template<> Pocket::InputController* Pocket::GameObject::ReplaceComponent<Pocket::InputController>(GameObject* source) { ReplaceComponent(45, source); return (Pocket::InputController*) GetComponent(45); }
template<> void Pocket::GameObject::EnableComponent<Pocket::InputController>(bool enable) { EnableComponent(45, enable); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::GetComponent<Pocket::FirstPersonMover>() { return (Pocket::FirstPersonMover*) GetComponent(46); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::AddComponent<Pocket::FirstPersonMover>() { AddComponent(46); return (Pocket::FirstPersonMover*) GetComponent(46); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FirstPersonMover>() { RemoveComponent(46); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::CloneComponent<Pocket::FirstPersonMover>(GameObject* source) { CloneComponent(46, source); return (Pocket::FirstPersonMover*) GetComponent(46); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::ReplaceComponent<Pocket::FirstPersonMover>(GameObject* source) { ReplaceComponent(46, source); return (Pocket::FirstPersonMover*) GetComponent(46); }
template<> void Pocket::GameObject::EnableComponent<Pocket::FirstPersonMover>(bool enable) { EnableComponent(46, enable); }
template<> Pocket::RigidBody* Pocket::GameObject::GetComponent<Pocket::RigidBody>() { return (Pocket::RigidBody*) GetComponent(47); }
template<> Pocket::RigidBody* Pocket::GameObject::AddComponent<Pocket::RigidBody>() { AddComponent(47); return (Pocket::RigidBody*) GetComponent(47); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::RigidBody>() { RemoveComponent(47); }
template<> Pocket::RigidBody* Pocket::GameObject::CloneComponent<Pocket::RigidBody>(GameObject* source) { CloneComponent(47, source); return (Pocket::RigidBody*) GetComponent(47); }
template<> Pocket::RigidBody* Pocket::GameObject::ReplaceComponent<Pocket::RigidBody>(GameObject* source) { ReplaceComponent(47, source); return (Pocket::RigidBody*) GetComponent(47); }
template<> void Pocket::GameObject::EnableComponent<Pocket::RigidBody>(bool enable) { EnableComponent(47, enable); }
template<> Pocket::RigidBody2d* Pocket::GameObject::GetComponent<Pocket::RigidBody2d>() { return (Pocket::RigidBody2d*) GetComponent(48); }
template<> Pocket::RigidBody2d* Pocket::GameObject::AddComponent<Pocket::RigidBody2d>() { AddComponent(48); return (Pocket::RigidBody2d*) GetComponent(48); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::RigidBody2d>() { RemoveComponent(48); }
template<> Pocket::RigidBody2d* Pocket::GameObject::CloneComponent<Pocket::RigidBody2d>(GameObject* source) { CloneComponent(48, source); return (Pocket::RigidBody2d*) GetComponent(48); }
template<> Pocket::RigidBody2d* Pocket::GameObject::ReplaceComponent<Pocket::RigidBody2d>(GameObject* source) { ReplaceComponent(48, source); return (Pocket::RigidBody2d*) GetComponent(48); }
template<> void Pocket::GameObject::EnableComponent<Pocket::RigidBody2d>(bool enable) { EnableComponent(48, enable); }
template<> Pocket::Joint2d* Pocket::GameObject::GetComponent<Pocket::Joint2d>() { return (Pocket::Joint2d*) GetComponent(49); }
template<> Pocket::Joint2d* Pocket::GameObject::AddComponent<Pocket::Joint2d>() { AddComponent(49); return (Pocket::Joint2d*) GetComponent(49); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Joint2d>() { RemoveComponent(49); }
template<> Pocket::Joint2d* Pocket::GameObject::CloneComponent<Pocket::Joint2d>(GameObject* source) { CloneComponent(49, source); return (Pocket::Joint2d*) GetComponent(49); }
template<> Pocket::Joint2d* Pocket::GameObject::ReplaceComponent<Pocket::Joint2d>(GameObject* source) { ReplaceComponent(49, source); return (Pocket::Joint2d*) GetComponent(49); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Joint2d>(bool enable) { EnableComponent(49, enable); }
template<> Pocket::DistanceScaler* Pocket::GameObject::GetComponent<Pocket::DistanceScaler>() { return (Pocket::DistanceScaler*) GetComponent(50); }
template<> Pocket::DistanceScaler* Pocket::GameObject::AddComponent<Pocket::DistanceScaler>() { AddComponent(50); return (Pocket::DistanceScaler*) GetComponent(50); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::DistanceScaler>() { RemoveComponent(50); }
template<> Pocket::DistanceScaler* Pocket::GameObject::CloneComponent<Pocket::DistanceScaler>(GameObject* source) { CloneComponent(50, source); return (Pocket::DistanceScaler*) GetComponent(50); }
template<> Pocket::DistanceScaler* Pocket::GameObject::ReplaceComponent<Pocket::DistanceScaler>(GameObject* source) { ReplaceComponent(50, source); return (Pocket::DistanceScaler*) GetComponent(50); }
template<> void Pocket::GameObject::EnableComponent<Pocket::DistanceScaler>(bool enable) { EnableComponent(50, enable); }
template<> Pocket::LineRenderer* Pocket::GameObject::GetComponent<Pocket::LineRenderer>() { return (Pocket::LineRenderer*) GetComponent(51); }
template<> Pocket::LineRenderer* Pocket::GameObject::AddComponent<Pocket::LineRenderer>() { AddComponent(51); return (Pocket::LineRenderer*) GetComponent(51); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::LineRenderer>() { RemoveComponent(51); }
template<> Pocket::LineRenderer* Pocket::GameObject::CloneComponent<Pocket::LineRenderer>(GameObject* source) { CloneComponent(51, source); return (Pocket::LineRenderer*) GetComponent(51); }
template<> Pocket::LineRenderer* Pocket::GameObject::ReplaceComponent<Pocket::LineRenderer>(GameObject* source) { ReplaceComponent(51, source); return (Pocket::LineRenderer*) GetComponent(51); }
template<> void Pocket::GameObject::EnableComponent<Pocket::LineRenderer>(bool enable) { EnableComponent(51, enable); }
template<> Pocket::SceneManager* Pocket::GameObject::GetComponent<Pocket::SceneManager>() { return (Pocket::SceneManager*) GetComponent(52); }
template<> Pocket::SceneManager* Pocket::GameObject::AddComponent<Pocket::SceneManager>() { AddComponent(52); return (Pocket::SceneManager*) GetComponent(52); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SceneManager>() { RemoveComponent(52); }
template<> Pocket::SceneManager* Pocket::GameObject::CloneComponent<Pocket::SceneManager>(GameObject* source) { CloneComponent(52, source); return (Pocket::SceneManager*) GetComponent(52); }
template<> Pocket::SceneManager* Pocket::GameObject::ReplaceComponent<Pocket::SceneManager>(GameObject* source) { ReplaceComponent(52, source); return (Pocket::SceneManager*) GetComponent(52); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SceneManager>(bool enable) { EnableComponent(52, enable); }
template<> Pocket::Trigger* Pocket::GameObject::GetComponent<Pocket::Trigger>() { return (Pocket::Trigger*) GetComponent(53); }
template<> Pocket::Trigger* Pocket::GameObject::AddComponent<Pocket::Trigger>() { AddComponent(53); return (Pocket::Trigger*) GetComponent(53); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Trigger>() { RemoveComponent(53); }
template<> Pocket::Trigger* Pocket::GameObject::CloneComponent<Pocket::Trigger>(GameObject* source) { CloneComponent(53, source); return (Pocket::Trigger*) GetComponent(53); }
template<> Pocket::Trigger* Pocket::GameObject::ReplaceComponent<Pocket::Trigger>(GameObject* source) { ReplaceComponent(53, source); return (Pocket::Trigger*) GetComponent(53); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Trigger>(bool enable) { EnableComponent(53, enable); }
template<> Pocket::LogMessage* Pocket::GameObject::GetComponent<Pocket::LogMessage>() { return (Pocket::LogMessage*) GetComponent(56); }
template<> Pocket::LogMessage* Pocket::GameObject::AddComponent<Pocket::LogMessage>() { AddComponent(56); return (Pocket::LogMessage*) GetComponent(56); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::LogMessage>() { RemoveComponent(56); }
template<> Pocket::LogMessage* Pocket::GameObject::CloneComponent<Pocket::LogMessage>(GameObject* source) { CloneComponent(56, source); return (Pocket::LogMessage*) GetComponent(56); }
template<> Pocket::LogMessage* Pocket::GameObject::ReplaceComponent<Pocket::LogMessage>(GameObject* source) { ReplaceComponent(56, source); return (Pocket::LogMessage*) GetComponent(56); }
template<> void Pocket::GameObject::EnableComponent<Pocket::LogMessage>(bool enable) { EnableComponent(56, enable); }
template<> Pocket::FileSystemListener* Pocket::GameObject::GetComponent<Pocket::FileSystemListener>() { return (Pocket::FileSystemListener*) GetComponent(57); }
template<> Pocket::FileSystemListener* Pocket::GameObject::AddComponent<Pocket::FileSystemListener>() { AddComponent(57); return (Pocket::FileSystemListener*) GetComponent(57); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FileSystemListener>() { RemoveComponent(57); }
template<> Pocket::FileSystemListener* Pocket::GameObject::CloneComponent<Pocket::FileSystemListener>(GameObject* source) { CloneComponent(57, source); return (Pocket::FileSystemListener*) GetComponent(57); }
template<> Pocket::FileSystemListener* Pocket::GameObject::ReplaceComponent<Pocket::FileSystemListener>(GameObject* source) { ReplaceComponent(57, source); return (Pocket::FileSystemListener*) GetComponent(57); }
template<> void Pocket::GameObject::EnableComponent<Pocket::FileSystemListener>(bool enable) { EnableComponent(57, enable); }
template<> Pocket::AssetImporter* Pocket::GameObject::GetComponent<Pocket::AssetImporter>() { return (Pocket::AssetImporter*) GetComponent(62); }
template<> Pocket::AssetImporter* Pocket::GameObject::AddComponent<Pocket::AssetImporter>() { AddComponent(62); return (Pocket::AssetImporter*) GetComponent(62); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::AssetImporter>() { RemoveComponent(62); }
template<> Pocket::AssetImporter* Pocket::GameObject::CloneComponent<Pocket::AssetImporter>(GameObject* source) { CloneComponent(62, source); return (Pocket::AssetImporter*) GetComponent(62); }
template<> Pocket::AssetImporter* Pocket::GameObject::ReplaceComponent<Pocket::AssetImporter>(GameObject* source) { ReplaceComponent(62, source); return (Pocket::AssetImporter*) GetComponent(62); }
template<> void Pocket::GameObject::EnableComponent<Pocket::AssetImporter>(bool enable) { EnableComponent(62, enable); }
template<> KillTrigger* Pocket::GameObject::GetComponent<KillTrigger>() { return (KillTrigger*) GetComponent(63); }
template<> KillTrigger* Pocket::GameObject::AddComponent<KillTrigger>() { AddComponent(63); return (KillTrigger*) GetComponent(63); }
template<> void Pocket::GameObject::RemoveComponent<KillTrigger>() { RemoveComponent(63); }
template<> KillTrigger* Pocket::GameObject::CloneComponent<KillTrigger>(GameObject* source) { CloneComponent(63, source); return (KillTrigger*) GetComponent(63); }
template<> KillTrigger* Pocket::GameObject::ReplaceComponent<KillTrigger>(GameObject* source) { ReplaceComponent(63, source); return (KillTrigger*) GetComponent(63); }
template<> void Pocket::GameObject::EnableComponent<KillTrigger>(bool enable) { EnableComponent(63, enable); }
template<> RectCollider* Pocket::GameObject::GetComponent<RectCollider>() { return (RectCollider*) GetComponent(64); }
template<> RectCollider* Pocket::GameObject::AddComponent<RectCollider>() { AddComponent(64); return (RectCollider*) GetComponent(64); }
template<> void Pocket::GameObject::RemoveComponent<RectCollider>() { RemoveComponent(64); }
template<> RectCollider* Pocket::GameObject::CloneComponent<RectCollider>(GameObject* source) { CloneComponent(64, source); return (RectCollider*) GetComponent(64); }
template<> RectCollider* Pocket::GameObject::ReplaceComponent<RectCollider>(GameObject* source) { ReplaceComponent(64, source); return (RectCollider*) GetComponent(64); }
template<> void Pocket::GameObject::EnableComponent<RectCollider>(bool enable) { EnableComponent(64, enable); }
template<> Rotator* Pocket::GameObject::GetComponent<Rotator>() { return (Rotator*) GetComponent(65); }
template<> Rotator* Pocket::GameObject::AddComponent<Rotator>() { AddComponent(65); return (Rotator*) GetComponent(65); }
template<> void Pocket::GameObject::RemoveComponent<Rotator>() { RemoveComponent(65); }
template<> Rotator* Pocket::GameObject::CloneComponent<Rotator>(GameObject* source) { CloneComponent(65, source); return (Rotator*) GetComponent(65); }
template<> Rotator* Pocket::GameObject::ReplaceComponent<Rotator>(GameObject* source) { ReplaceComponent(65, source); return (Rotator*) GetComponent(65); }
template<> void Pocket::GameObject::EnableComponent<Rotator>(bool enable) { EnableComponent(65, enable); }
template<> Score* Pocket::GameObject::GetComponent<Score>() { return (Score*) GetComponent(66); }
template<> Score* Pocket::GameObject::AddComponent<Score>() { AddComponent(66); return (Score*) GetComponent(66); }
template<> void Pocket::GameObject::RemoveComponent<Score>() { RemoveComponent(66); }
template<> Score* Pocket::GameObject::CloneComponent<Score>(GameObject* source) { CloneComponent(66, source); return (Score*) GetComponent(66); }
template<> Score* Pocket::GameObject::ReplaceComponent<Score>(GameObject* source) { ReplaceComponent(66, source); return (Score*) GetComponent(66); }
template<> void Pocket::GameObject::EnableComponent<Score>(bool enable) { EnableComponent(66, enable); }
template<> ScoreTrigger* Pocket::GameObject::GetComponent<ScoreTrigger>() { return (ScoreTrigger*) GetComponent(67); }
template<> ScoreTrigger* Pocket::GameObject::AddComponent<ScoreTrigger>() { AddComponent(67); return (ScoreTrigger*) GetComponent(67); }
template<> void Pocket::GameObject::RemoveComponent<ScoreTrigger>() { RemoveComponent(67); }
template<> ScoreTrigger* Pocket::GameObject::CloneComponent<ScoreTrigger>(GameObject* source) { CloneComponent(67, source); return (ScoreTrigger*) GetComponent(67); }
template<> ScoreTrigger* Pocket::GameObject::ReplaceComponent<ScoreTrigger>(GameObject* source) { ReplaceComponent(67, source); return (ScoreTrigger*) GetComponent(67); }
template<> void Pocket::GameObject::EnableComponent<ScoreTrigger>(bool enable) { EnableComponent(67, enable); }
template<> TransformController* Pocket::GameObject::GetComponent<TransformController>() { return (TransformController*) GetComponent(68); }
template<> TransformController* Pocket::GameObject::AddComponent<TransformController>() { AddComponent(68); return (TransformController*) GetComponent(68); }
template<> void Pocket::GameObject::RemoveComponent<TransformController>() { RemoveComponent(68); }
template<> TransformController* Pocket::GameObject::CloneComponent<TransformController>(GameObject* source) { CloneComponent(68, source); return (TransformController*) GetComponent(68); }
template<> TransformController* Pocket::GameObject::ReplaceComponent<TransformController>(GameObject* source) { ReplaceComponent(68, source); return (TransformController*) GetComponent(68); }
template<> void Pocket::GameObject::EnableComponent<TransformController>(bool enable) { EnableComponent(68, enable); }
#include "TypeInfo.hpp"
#include "Property.hpp"
#include "Transform.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Mesh.hpp"
#include "Renderable.hpp"
#include "Sizeable.hpp"
#include "VertexMesh.hpp"
#include "TextureAtlas.hpp"
#include "Colour.hpp"
#include "Touchable.hpp"
#include "InputController.hpp"
#include "Velocity.hpp"
#include "KillTrigger.hpp"
#include "RectCollider.hpp"
#include "Rotator.hpp"
#include "Score.hpp"
#include "ScoreTrigger.hpp"
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
   return 7;
}
extern "C" IGameSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 75: return new KillTriggerSystem();
      case 76: return new RectCollisionSystem();
      case 77: return new RotatorSystem();
      case 78: return new ScoreSystem();
      case 79: return new ScoreTriggerSystem();
      case 80: return new TransformControllerSystem();
      case 81: return new VelocityCollider();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IGameSystem* scriptSystem) {
   delete scriptSystem; 
}
extern "C" int CountComponents() {
   return 6;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 63: return new KillTrigger();
      case 64: return new RectCollider();
      case 65: return new Rotator();
      case 66: return new Score();
      case 67: return new ScoreTrigger();
      case 68: return new TransformController();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 63: { delete ((KillTrigger*)component); break; }
      case 64: { delete ((RectCollider*)component); break; }
      case 65: { delete ((Rotator*)component); break; }
      case 66: { delete ((Score*)component); break; }
      case 67: { delete ((ScoreTrigger*)component); break; }
      case 68: { delete ((TransformController*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 63: { KillTrigger* co = (KillTrigger*)c; 
      KillTrigger* so = ((KillTrigger*)s);
        co->operator=(*so);             break; }
      case 64: { RectCollider* co = (RectCollider*)c; 
      RectCollider* so = ((RectCollider*)s);
        co->operator=(*so);             break; }
      case 65: { Rotator* co = (Rotator*)c; 
      Rotator* so = ((Rotator*)s);
        co->operator=(*so);             break; }
      case 66: { Score* co = (Score*)c; 
      Score* so = ((Score*)s);
        co->operator=(*so);             break; }
      case 67: { ScoreTrigger* co = (ScoreTrigger*)c; 
      ScoreTrigger* so = ((ScoreTrigger*)s);
        co->operator=(*so);             break; }
      case 68: { TransformController* co = (TransformController*)c; 
      TransformController* so = ((TransformController*)s);
        co->operator=(*so);             break; }
   }
}

extern "C" Pocket::TypeInfo* GetTypeInfo(int componentID, void* componentPtr) {
   switch (componentID) { 
      case 63: {
      KillTrigger* component = (KillTrigger*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "KillTrigger";
	      info->AddField(component->variable, "variable");
      return info;
      break; }
      case 64: {
      RectCollider* component = (RectCollider*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "RectCollider";
	      info->AddField(component->Enter, "Enter");
	      info->AddField(component->Exit, "Exit");
	      info->AddField(component->collisions, "collisions");
	      info->AddField(component->previousCollisions, "previousCollisions");
      return info;
      break; }
      case 65: {
      Rotator* component = (Rotator*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Rotator";
	      info->AddField(component->speed, "speed");
      return info;
      break; }
      case 66: {
      Score* component = (Score*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Score";
	      info->AddField(component->score, "score");
      return info;
      break; }
      case 67: {
      ScoreTrigger* component = (ScoreTrigger*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "ScoreTrigger";
	      info->AddField(component->scoreToAdd, "scoreToAdd");
      return info;
      break; }
      case 68: {
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
extern "C" const char* GetComponentName(int componentID) {
   switch (componentID) { 
      case 63: 
         return "KillTrigger";
      case 64: 
         return "RectCollider";
      case 65: 
         return "Rotator";
      case 66: 
         return "Score";
      case 67: 
         return "ScoreTrigger";
      case 68: 
         return "TransformController";
      default: return "";
   }
}
extern "C" int* GetSystems() {
 return new int[26] {-1,63,64,-1,1,64,10,-1,65,1,-1,66,20,-1,67,64,66,-1,1,68,45,-1,14,64,10,-2,};}
extern "C" void DeleteGetSystems(int* indicies) {
delete indicies;
}
