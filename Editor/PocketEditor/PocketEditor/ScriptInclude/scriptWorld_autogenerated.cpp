namespace Pocket {
struct Hierarchy;
struct Atlas;
struct Spawner;
struct Animation;
struct Animator;
struct AssetLoader;
struct TextureComponent;
struct ShaderComponent;
struct Font;
struct Sound;
struct SoundEmitter;
struct SoundListener;
struct Transform;
struct Cloner;
struct CloneVariable;
struct Orderable;
struct Mesh;
struct ParticleEmitter;
struct ParticleEffect;
struct Renderable;
struct Camera;
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
struct Label;
struct PanelDropper;
struct Panel;
struct PanelArea;
struct PanelSplitter;
struct TouchableCanceller;
struct Hoverable;
struct ScrollWheelMover;
struct VirtualTreeList;
struct VirtualTreeListSpawner;
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
struct EditorObject;
struct AssetImporter;
struct FieldEditor;
struct EditorDropTarget;
struct ProjectSettings;
struct FilePath;
struct GameWorldViewport;
struct GameObjectEditor;
struct FileSystemListener;
struct GameObjectFieldEditor;
struct LogMessage;
}
struct Road;
struct RoadGenerator;
struct RoadMovement;
struct RoadOrigin;
struct RoadPosition;
struct RoadRenderer;
struct Rotatable;
struct TransformFollower;

#include "GameSystem.hpp"
template<> Pocket::Hierarchy* Pocket::GameObject::GetComponent<Pocket::Hierarchy>() { return (Pocket::Hierarchy*) GetComponent(0); }
template<> Pocket::Hierarchy* Pocket::GameObject::AddComponent<Pocket::Hierarchy>() { AddComponent(0); return (Pocket::Hierarchy*) GetComponent(0); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Hierarchy>() { RemoveComponent(0); }
template<> Pocket::Hierarchy* Pocket::GameObject::CloneComponent<Pocket::Hierarchy>(GameObject* source) { CloneComponent(0, source); return (Pocket::Hierarchy*) GetComponent(0); }
template<> Pocket::Hierarchy* Pocket::GameObject::ReplaceComponent<Pocket::Hierarchy>(GameObject* source) { ReplaceComponent(0, source); return (Pocket::Hierarchy*) GetComponent(0); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Hierarchy>(bool enable) { EnableComponent(0, enable); }
template<> Pocket::Atlas* Pocket::GameObject::GetComponent<Pocket::Atlas>() { return (Pocket::Atlas*) GetComponent(1); }
template<> Pocket::Atlas* Pocket::GameObject::AddComponent<Pocket::Atlas>() { AddComponent(1); return (Pocket::Atlas*) GetComponent(1); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Atlas>() { RemoveComponent(1); }
template<> Pocket::Atlas* Pocket::GameObject::CloneComponent<Pocket::Atlas>(GameObject* source) { CloneComponent(1, source); return (Pocket::Atlas*) GetComponent(1); }
template<> Pocket::Atlas* Pocket::GameObject::ReplaceComponent<Pocket::Atlas>(GameObject* source) { ReplaceComponent(1, source); return (Pocket::Atlas*) GetComponent(1); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Atlas>(bool enable) { EnableComponent(1, enable); }
template<> Pocket::Spawner* Pocket::GameObject::GetComponent<Pocket::Spawner>() { return (Pocket::Spawner*) GetComponent(2); }
template<> Pocket::Spawner* Pocket::GameObject::AddComponent<Pocket::Spawner>() { AddComponent(2); return (Pocket::Spawner*) GetComponent(2); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Spawner>() { RemoveComponent(2); }
template<> Pocket::Spawner* Pocket::GameObject::CloneComponent<Pocket::Spawner>(GameObject* source) { CloneComponent(2, source); return (Pocket::Spawner*) GetComponent(2); }
template<> Pocket::Spawner* Pocket::GameObject::ReplaceComponent<Pocket::Spawner>(GameObject* source) { ReplaceComponent(2, source); return (Pocket::Spawner*) GetComponent(2); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Spawner>(bool enable) { EnableComponent(2, enable); }
template<> Pocket::Animation* Pocket::GameObject::GetComponent<Pocket::Animation>() { return (Pocket::Animation*) GetComponent(3); }
template<> Pocket::Animation* Pocket::GameObject::AddComponent<Pocket::Animation>() { AddComponent(3); return (Pocket::Animation*) GetComponent(3); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Animation>() { RemoveComponent(3); }
template<> Pocket::Animation* Pocket::GameObject::CloneComponent<Pocket::Animation>(GameObject* source) { CloneComponent(3, source); return (Pocket::Animation*) GetComponent(3); }
template<> Pocket::Animation* Pocket::GameObject::ReplaceComponent<Pocket::Animation>(GameObject* source) { ReplaceComponent(3, source); return (Pocket::Animation*) GetComponent(3); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Animation>(bool enable) { EnableComponent(3, enable); }
template<> Pocket::Animator* Pocket::GameObject::GetComponent<Pocket::Animator>() { return (Pocket::Animator*) GetComponent(4); }
template<> Pocket::Animator* Pocket::GameObject::AddComponent<Pocket::Animator>() { AddComponent(4); return (Pocket::Animator*) GetComponent(4); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Animator>() { RemoveComponent(4); }
template<> Pocket::Animator* Pocket::GameObject::CloneComponent<Pocket::Animator>(GameObject* source) { CloneComponent(4, source); return (Pocket::Animator*) GetComponent(4); }
template<> Pocket::Animator* Pocket::GameObject::ReplaceComponent<Pocket::Animator>(GameObject* source) { ReplaceComponent(4, source); return (Pocket::Animator*) GetComponent(4); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Animator>(bool enable) { EnableComponent(4, enable); }
template<> Pocket::AssetLoader* Pocket::GameObject::GetComponent<Pocket::AssetLoader>() { return (Pocket::AssetLoader*) GetComponent(5); }
template<> Pocket::AssetLoader* Pocket::GameObject::AddComponent<Pocket::AssetLoader>() { AddComponent(5); return (Pocket::AssetLoader*) GetComponent(5); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::AssetLoader>() { RemoveComponent(5); }
template<> Pocket::AssetLoader* Pocket::GameObject::CloneComponent<Pocket::AssetLoader>(GameObject* source) { CloneComponent(5, source); return (Pocket::AssetLoader*) GetComponent(5); }
template<> Pocket::AssetLoader* Pocket::GameObject::ReplaceComponent<Pocket::AssetLoader>(GameObject* source) { ReplaceComponent(5, source); return (Pocket::AssetLoader*) GetComponent(5); }
template<> void Pocket::GameObject::EnableComponent<Pocket::AssetLoader>(bool enable) { EnableComponent(5, enable); }
template<> Pocket::TextureComponent* Pocket::GameObject::GetComponent<Pocket::TextureComponent>() { return (Pocket::TextureComponent*) GetComponent(6); }
template<> Pocket::TextureComponent* Pocket::GameObject::AddComponent<Pocket::TextureComponent>() { AddComponent(6); return (Pocket::TextureComponent*) GetComponent(6); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::TextureComponent>() { RemoveComponent(6); }
template<> Pocket::TextureComponent* Pocket::GameObject::CloneComponent<Pocket::TextureComponent>(GameObject* source) { CloneComponent(6, source); return (Pocket::TextureComponent*) GetComponent(6); }
template<> Pocket::TextureComponent* Pocket::GameObject::ReplaceComponent<Pocket::TextureComponent>(GameObject* source) { ReplaceComponent(6, source); return (Pocket::TextureComponent*) GetComponent(6); }
template<> void Pocket::GameObject::EnableComponent<Pocket::TextureComponent>(bool enable) { EnableComponent(6, enable); }
template<> Pocket::ShaderComponent* Pocket::GameObject::GetComponent<Pocket::ShaderComponent>() { return (Pocket::ShaderComponent*) GetComponent(7); }
template<> Pocket::ShaderComponent* Pocket::GameObject::AddComponent<Pocket::ShaderComponent>() { AddComponent(7); return (Pocket::ShaderComponent*) GetComponent(7); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ShaderComponent>() { RemoveComponent(7); }
template<> Pocket::ShaderComponent* Pocket::GameObject::CloneComponent<Pocket::ShaderComponent>(GameObject* source) { CloneComponent(7, source); return (Pocket::ShaderComponent*) GetComponent(7); }
template<> Pocket::ShaderComponent* Pocket::GameObject::ReplaceComponent<Pocket::ShaderComponent>(GameObject* source) { ReplaceComponent(7, source); return (Pocket::ShaderComponent*) GetComponent(7); }
template<> void Pocket::GameObject::EnableComponent<Pocket::ShaderComponent>(bool enable) { EnableComponent(7, enable); }
template<> Pocket::Font* Pocket::GameObject::GetComponent<Pocket::Font>() { return (Pocket::Font*) GetComponent(8); }
template<> Pocket::Font* Pocket::GameObject::AddComponent<Pocket::Font>() { AddComponent(8); return (Pocket::Font*) GetComponent(8); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Font>() { RemoveComponent(8); }
template<> Pocket::Font* Pocket::GameObject::CloneComponent<Pocket::Font>(GameObject* source) { CloneComponent(8, source); return (Pocket::Font*) GetComponent(8); }
template<> Pocket::Font* Pocket::GameObject::ReplaceComponent<Pocket::Font>(GameObject* source) { ReplaceComponent(8, source); return (Pocket::Font*) GetComponent(8); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Font>(bool enable) { EnableComponent(8, enable); }
template<> Pocket::Sound* Pocket::GameObject::GetComponent<Pocket::Sound>() { return (Pocket::Sound*) GetComponent(9); }
template<> Pocket::Sound* Pocket::GameObject::AddComponent<Pocket::Sound>() { AddComponent(9); return (Pocket::Sound*) GetComponent(9); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sound>() { RemoveComponent(9); }
template<> Pocket::Sound* Pocket::GameObject::CloneComponent<Pocket::Sound>(GameObject* source) { CloneComponent(9, source); return (Pocket::Sound*) GetComponent(9); }
template<> Pocket::Sound* Pocket::GameObject::ReplaceComponent<Pocket::Sound>(GameObject* source) { ReplaceComponent(9, source); return (Pocket::Sound*) GetComponent(9); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Sound>(bool enable) { EnableComponent(9, enable); }
template<> Pocket::SoundEmitter* Pocket::GameObject::GetComponent<Pocket::SoundEmitter>() { return (Pocket::SoundEmitter*) GetComponent(10); }
template<> Pocket::SoundEmitter* Pocket::GameObject::AddComponent<Pocket::SoundEmitter>() { AddComponent(10); return (Pocket::SoundEmitter*) GetComponent(10); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SoundEmitter>() { RemoveComponent(10); }
template<> Pocket::SoundEmitter* Pocket::GameObject::CloneComponent<Pocket::SoundEmitter>(GameObject* source) { CloneComponent(10, source); return (Pocket::SoundEmitter*) GetComponent(10); }
template<> Pocket::SoundEmitter* Pocket::GameObject::ReplaceComponent<Pocket::SoundEmitter>(GameObject* source) { ReplaceComponent(10, source); return (Pocket::SoundEmitter*) GetComponent(10); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SoundEmitter>(bool enable) { EnableComponent(10, enable); }
template<> Pocket::SoundListener* Pocket::GameObject::GetComponent<Pocket::SoundListener>() { return (Pocket::SoundListener*) GetComponent(11); }
template<> Pocket::SoundListener* Pocket::GameObject::AddComponent<Pocket::SoundListener>() { AddComponent(11); return (Pocket::SoundListener*) GetComponent(11); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SoundListener>() { RemoveComponent(11); }
template<> Pocket::SoundListener* Pocket::GameObject::CloneComponent<Pocket::SoundListener>(GameObject* source) { CloneComponent(11, source); return (Pocket::SoundListener*) GetComponent(11); }
template<> Pocket::SoundListener* Pocket::GameObject::ReplaceComponent<Pocket::SoundListener>(GameObject* source) { ReplaceComponent(11, source); return (Pocket::SoundListener*) GetComponent(11); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SoundListener>(bool enable) { EnableComponent(11, enable); }
template<> Pocket::Transform* Pocket::GameObject::GetComponent<Pocket::Transform>() { return (Pocket::Transform*) GetComponent(12); }
template<> Pocket::Transform* Pocket::GameObject::AddComponent<Pocket::Transform>() { AddComponent(12); return (Pocket::Transform*) GetComponent(12); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Transform>() { RemoveComponent(12); }
template<> Pocket::Transform* Pocket::GameObject::CloneComponent<Pocket::Transform>(GameObject* source) { CloneComponent(12, source); return (Pocket::Transform*) GetComponent(12); }
template<> Pocket::Transform* Pocket::GameObject::ReplaceComponent<Pocket::Transform>(GameObject* source) { ReplaceComponent(12, source); return (Pocket::Transform*) GetComponent(12); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Transform>(bool enable) { EnableComponent(12, enable); }
template<> Pocket::Cloner* Pocket::GameObject::GetComponent<Pocket::Cloner>() { return (Pocket::Cloner*) GetComponent(13); }
template<> Pocket::Cloner* Pocket::GameObject::AddComponent<Pocket::Cloner>() { AddComponent(13); return (Pocket::Cloner*) GetComponent(13); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Cloner>() { RemoveComponent(13); }
template<> Pocket::Cloner* Pocket::GameObject::CloneComponent<Pocket::Cloner>(GameObject* source) { CloneComponent(13, source); return (Pocket::Cloner*) GetComponent(13); }
template<> Pocket::Cloner* Pocket::GameObject::ReplaceComponent<Pocket::Cloner>(GameObject* source) { ReplaceComponent(13, source); return (Pocket::Cloner*) GetComponent(13); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Cloner>(bool enable) { EnableComponent(13, enable); }
template<> Pocket::CloneVariable* Pocket::GameObject::GetComponent<Pocket::CloneVariable>() { return (Pocket::CloneVariable*) GetComponent(14); }
template<> Pocket::CloneVariable* Pocket::GameObject::AddComponent<Pocket::CloneVariable>() { AddComponent(14); return (Pocket::CloneVariable*) GetComponent(14); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::CloneVariable>() { RemoveComponent(14); }
template<> Pocket::CloneVariable* Pocket::GameObject::CloneComponent<Pocket::CloneVariable>(GameObject* source) { CloneComponent(14, source); return (Pocket::CloneVariable*) GetComponent(14); }
template<> Pocket::CloneVariable* Pocket::GameObject::ReplaceComponent<Pocket::CloneVariable>(GameObject* source) { ReplaceComponent(14, source); return (Pocket::CloneVariable*) GetComponent(14); }
template<> void Pocket::GameObject::EnableComponent<Pocket::CloneVariable>(bool enable) { EnableComponent(14, enable); }
template<> Pocket::Orderable* Pocket::GameObject::GetComponent<Pocket::Orderable>() { return (Pocket::Orderable*) GetComponent(15); }
template<> Pocket::Orderable* Pocket::GameObject::AddComponent<Pocket::Orderable>() { AddComponent(15); return (Pocket::Orderable*) GetComponent(15); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Orderable>() { RemoveComponent(15); }
template<> Pocket::Orderable* Pocket::GameObject::CloneComponent<Pocket::Orderable>(GameObject* source) { CloneComponent(15, source); return (Pocket::Orderable*) GetComponent(15); }
template<> Pocket::Orderable* Pocket::GameObject::ReplaceComponent<Pocket::Orderable>(GameObject* source) { ReplaceComponent(15, source); return (Pocket::Orderable*) GetComponent(15); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Orderable>(bool enable) { EnableComponent(15, enable); }
template<> Pocket::Mesh* Pocket::GameObject::GetComponent<Pocket::Mesh>() { return (Pocket::Mesh*) GetComponent(16); }
template<> Pocket::Mesh* Pocket::GameObject::AddComponent<Pocket::Mesh>() { AddComponent(16); return (Pocket::Mesh*) GetComponent(16); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Mesh>() { RemoveComponent(16); }
template<> Pocket::Mesh* Pocket::GameObject::CloneComponent<Pocket::Mesh>(GameObject* source) { CloneComponent(16, source); return (Pocket::Mesh*) GetComponent(16); }
template<> Pocket::Mesh* Pocket::GameObject::ReplaceComponent<Pocket::Mesh>(GameObject* source) { ReplaceComponent(16, source); return (Pocket::Mesh*) GetComponent(16); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Mesh>(bool enable) { EnableComponent(16, enable); }
template<> Pocket::ParticleEmitter* Pocket::GameObject::GetComponent<Pocket::ParticleEmitter>() { return (Pocket::ParticleEmitter*) GetComponent(17); }
template<> Pocket::ParticleEmitter* Pocket::GameObject::AddComponent<Pocket::ParticleEmitter>() { AddComponent(17); return (Pocket::ParticleEmitter*) GetComponent(17); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ParticleEmitter>() { RemoveComponent(17); }
template<> Pocket::ParticleEmitter* Pocket::GameObject::CloneComponent<Pocket::ParticleEmitter>(GameObject* source) { CloneComponent(17, source); return (Pocket::ParticleEmitter*) GetComponent(17); }
template<> Pocket::ParticleEmitter* Pocket::GameObject::ReplaceComponent<Pocket::ParticleEmitter>(GameObject* source) { ReplaceComponent(17, source); return (Pocket::ParticleEmitter*) GetComponent(17); }
template<> void Pocket::GameObject::EnableComponent<Pocket::ParticleEmitter>(bool enable) { EnableComponent(17, enable); }
template<> Pocket::ParticleEffect* Pocket::GameObject::GetComponent<Pocket::ParticleEffect>() { return (Pocket::ParticleEffect*) GetComponent(18); }
template<> Pocket::ParticleEffect* Pocket::GameObject::AddComponent<Pocket::ParticleEffect>() { AddComponent(18); return (Pocket::ParticleEffect*) GetComponent(18); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ParticleEffect>() { RemoveComponent(18); }
template<> Pocket::ParticleEffect* Pocket::GameObject::CloneComponent<Pocket::ParticleEffect>(GameObject* source) { CloneComponent(18, source); return (Pocket::ParticleEffect*) GetComponent(18); }
template<> Pocket::ParticleEffect* Pocket::GameObject::ReplaceComponent<Pocket::ParticleEffect>(GameObject* source) { ReplaceComponent(18, source); return (Pocket::ParticleEffect*) GetComponent(18); }
template<> void Pocket::GameObject::EnableComponent<Pocket::ParticleEffect>(bool enable) { EnableComponent(18, enable); }
template<> Pocket::Renderable* Pocket::GameObject::GetComponent<Pocket::Renderable>() { return (Pocket::Renderable*) GetComponent(19); }
template<> Pocket::Renderable* Pocket::GameObject::AddComponent<Pocket::Renderable>() { AddComponent(19); return (Pocket::Renderable*) GetComponent(19); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Renderable>() { RemoveComponent(19); }
template<> Pocket::Renderable* Pocket::GameObject::CloneComponent<Pocket::Renderable>(GameObject* source) { CloneComponent(19, source); return (Pocket::Renderable*) GetComponent(19); }
template<> Pocket::Renderable* Pocket::GameObject::ReplaceComponent<Pocket::Renderable>(GameObject* source) { ReplaceComponent(19, source); return (Pocket::Renderable*) GetComponent(19); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Renderable>(bool enable) { EnableComponent(19, enable); }
template<> Pocket::Camera* Pocket::GameObject::GetComponent<Pocket::Camera>() { return (Pocket::Camera*) GetComponent(20); }
template<> Pocket::Camera* Pocket::GameObject::AddComponent<Pocket::Camera>() { AddComponent(20); return (Pocket::Camera*) GetComponent(20); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Camera>() { RemoveComponent(20); }
template<> Pocket::Camera* Pocket::GameObject::CloneComponent<Pocket::Camera>(GameObject* source) { CloneComponent(20, source); return (Pocket::Camera*) GetComponent(20); }
template<> Pocket::Camera* Pocket::GameObject::ReplaceComponent<Pocket::Camera>(GameObject* source) { ReplaceComponent(20, source); return (Pocket::Camera*) GetComponent(20); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Camera>(bool enable) { EnableComponent(20, enable); }
template<> Pocket::TextBox* Pocket::GameObject::GetComponent<Pocket::TextBox>() { return (Pocket::TextBox*) GetComponent(21); }
template<> Pocket::TextBox* Pocket::GameObject::AddComponent<Pocket::TextBox>() { AddComponent(21); return (Pocket::TextBox*) GetComponent(21); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::TextBox>() { RemoveComponent(21); }
template<> Pocket::TextBox* Pocket::GameObject::CloneComponent<Pocket::TextBox>(GameObject* source) { CloneComponent(21, source); return (Pocket::TextBox*) GetComponent(21); }
template<> Pocket::TextBox* Pocket::GameObject::ReplaceComponent<Pocket::TextBox>(GameObject* source) { ReplaceComponent(21, source); return (Pocket::TextBox*) GetComponent(21); }
template<> void Pocket::GameObject::EnableComponent<Pocket::TextBox>(bool enable) { EnableComponent(21, enable); }
template<> Pocket::Touchable* Pocket::GameObject::GetComponent<Pocket::Touchable>() { return (Pocket::Touchable*) GetComponent(22); }
template<> Pocket::Touchable* Pocket::GameObject::AddComponent<Pocket::Touchable>() { AddComponent(22); return (Pocket::Touchable*) GetComponent(22); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Touchable>() { RemoveComponent(22); }
template<> Pocket::Touchable* Pocket::GameObject::CloneComponent<Pocket::Touchable>(GameObject* source) { CloneComponent(22, source); return (Pocket::Touchable*) GetComponent(22); }
template<> Pocket::Touchable* Pocket::GameObject::ReplaceComponent<Pocket::Touchable>(GameObject* source) { ReplaceComponent(22, source); return (Pocket::Touchable*) GetComponent(22); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Touchable>(bool enable) { EnableComponent(22, enable); }
template<> Pocket::Sprite* Pocket::GameObject::GetComponent<Pocket::Sprite>() { return (Pocket::Sprite*) GetComponent(23); }
template<> Pocket::Sprite* Pocket::GameObject::AddComponent<Pocket::Sprite>() { AddComponent(23); return (Pocket::Sprite*) GetComponent(23); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sprite>() { RemoveComponent(23); }
template<> Pocket::Sprite* Pocket::GameObject::CloneComponent<Pocket::Sprite>(GameObject* source) { CloneComponent(23, source); return (Pocket::Sprite*) GetComponent(23); }
template<> Pocket::Sprite* Pocket::GameObject::ReplaceComponent<Pocket::Sprite>(GameObject* source) { ReplaceComponent(23, source); return (Pocket::Sprite*) GetComponent(23); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Sprite>(bool enable) { EnableComponent(23, enable); }
template<> Pocket::Sizeable* Pocket::GameObject::GetComponent<Pocket::Sizeable>() { return (Pocket::Sizeable*) GetComponent(24); }
template<> Pocket::Sizeable* Pocket::GameObject::AddComponent<Pocket::Sizeable>() { AddComponent(24); return (Pocket::Sizeable*) GetComponent(24); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Sizeable>() { RemoveComponent(24); }
template<> Pocket::Sizeable* Pocket::GameObject::CloneComponent<Pocket::Sizeable>(GameObject* source) { CloneComponent(24, source); return (Pocket::Sizeable*) GetComponent(24); }
template<> Pocket::Sizeable* Pocket::GameObject::ReplaceComponent<Pocket::Sizeable>(GameObject* source) { ReplaceComponent(24, source); return (Pocket::Sizeable*) GetComponent(24); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Sizeable>(bool enable) { EnableComponent(24, enable); }
template<> Pocket::Draggable* Pocket::GameObject::GetComponent<Pocket::Draggable>() { return (Pocket::Draggable*) GetComponent(25); }
template<> Pocket::Draggable* Pocket::GameObject::AddComponent<Pocket::Draggable>() { AddComponent(25); return (Pocket::Draggable*) GetComponent(25); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Draggable>() { RemoveComponent(25); }
template<> Pocket::Draggable* Pocket::GameObject::CloneComponent<Pocket::Draggable>(GameObject* source) { CloneComponent(25, source); return (Pocket::Draggable*) GetComponent(25); }
template<> Pocket::Draggable* Pocket::GameObject::ReplaceComponent<Pocket::Draggable>(GameObject* source) { ReplaceComponent(25, source); return (Pocket::Draggable*) GetComponent(25); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Draggable>(bool enable) { EnableComponent(25, enable); }
template<> Pocket::Layouter* Pocket::GameObject::GetComponent<Pocket::Layouter>() { return (Pocket::Layouter*) GetComponent(26); }
template<> Pocket::Layouter* Pocket::GameObject::AddComponent<Pocket::Layouter>() { AddComponent(26); return (Pocket::Layouter*) GetComponent(26); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Layouter>() { RemoveComponent(26); }
template<> Pocket::Layouter* Pocket::GameObject::CloneComponent<Pocket::Layouter>(GameObject* source) { CloneComponent(26, source); return (Pocket::Layouter*) GetComponent(26); }
template<> Pocket::Layouter* Pocket::GameObject::ReplaceComponent<Pocket::Layouter>(GameObject* source) { ReplaceComponent(26, source); return (Pocket::Layouter*) GetComponent(26); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Layouter>(bool enable) { EnableComponent(26, enable); }
template<> Pocket::Colorable* Pocket::GameObject::GetComponent<Pocket::Colorable>() { return (Pocket::Colorable*) GetComponent(27); }
template<> Pocket::Colorable* Pocket::GameObject::AddComponent<Pocket::Colorable>() { AddComponent(27); return (Pocket::Colorable*) GetComponent(27); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Colorable>() { RemoveComponent(27); }
template<> Pocket::Colorable* Pocket::GameObject::CloneComponent<Pocket::Colorable>(GameObject* source) { CloneComponent(27, source); return (Pocket::Colorable*) GetComponent(27); }
template<> Pocket::Colorable* Pocket::GameObject::ReplaceComponent<Pocket::Colorable>(GameObject* source) { ReplaceComponent(27, source); return (Pocket::Colorable*) GetComponent(27); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Colorable>(bool enable) { EnableComponent(27, enable); }
template<> Pocket::Velocity* Pocket::GameObject::GetComponent<Pocket::Velocity>() { return (Pocket::Velocity*) GetComponent(28); }
template<> Pocket::Velocity* Pocket::GameObject::AddComponent<Pocket::Velocity>() { AddComponent(28); return (Pocket::Velocity*) GetComponent(28); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Velocity>() { RemoveComponent(28); }
template<> Pocket::Velocity* Pocket::GameObject::CloneComponent<Pocket::Velocity>(GameObject* source) { CloneComponent(28, source); return (Pocket::Velocity*) GetComponent(28); }
template<> Pocket::Velocity* Pocket::GameObject::ReplaceComponent<Pocket::Velocity>(GameObject* source) { ReplaceComponent(28, source); return (Pocket::Velocity*) GetComponent(28); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Velocity>(bool enable) { EnableComponent(28, enable); }
template<> Pocket::Limitable* Pocket::GameObject::GetComponent<Pocket::Limitable>() { return (Pocket::Limitable*) GetComponent(29); }
template<> Pocket::Limitable* Pocket::GameObject::AddComponent<Pocket::Limitable>() { AddComponent(29); return (Pocket::Limitable*) GetComponent(29); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Limitable>() { RemoveComponent(29); }
template<> Pocket::Limitable* Pocket::GameObject::CloneComponent<Pocket::Limitable>(GameObject* source) { CloneComponent(29, source); return (Pocket::Limitable*) GetComponent(29); }
template<> Pocket::Limitable* Pocket::GameObject::ReplaceComponent<Pocket::Limitable>(GameObject* source) { ReplaceComponent(29, source); return (Pocket::Limitable*) GetComponent(29); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Limitable>(bool enable) { EnableComponent(29, enable); }
template<> Pocket::SelectedColorer* Pocket::GameObject::GetComponent<Pocket::SelectedColorer>() { return (Pocket::SelectedColorer*) GetComponent(30); }
template<> Pocket::SelectedColorer* Pocket::GameObject::AddComponent<Pocket::SelectedColorer>() { AddComponent(30); return (Pocket::SelectedColorer*) GetComponent(30); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SelectedColorer>() { RemoveComponent(30); }
template<> Pocket::SelectedColorer* Pocket::GameObject::CloneComponent<Pocket::SelectedColorer>(GameObject* source) { CloneComponent(30, source); return (Pocket::SelectedColorer*) GetComponent(30); }
template<> Pocket::SelectedColorer* Pocket::GameObject::ReplaceComponent<Pocket::SelectedColorer>(GameObject* source) { ReplaceComponent(30, source); return (Pocket::SelectedColorer*) GetComponent(30); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SelectedColorer>(bool enable) { EnableComponent(30, enable); }
template<> Pocket::Selectable* Pocket::GameObject::GetComponent<Pocket::Selectable>() { return (Pocket::Selectable*) GetComponent(31); }
template<> Pocket::Selectable* Pocket::GameObject::AddComponent<Pocket::Selectable>() { AddComponent(31); return (Pocket::Selectable*) GetComponent(31); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Selectable>() { RemoveComponent(31); }
template<> Pocket::Selectable* Pocket::GameObject::CloneComponent<Pocket::Selectable>(GameObject* source) { CloneComponent(31, source); return (Pocket::Selectable*) GetComponent(31); }
template<> Pocket::Selectable* Pocket::GameObject::ReplaceComponent<Pocket::Selectable>(GameObject* source) { ReplaceComponent(31, source); return (Pocket::Selectable*) GetComponent(31); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Selectable>(bool enable) { EnableComponent(31, enable); }
template<> Pocket::Droppable* Pocket::GameObject::GetComponent<Pocket::Droppable>() { return (Pocket::Droppable*) GetComponent(32); }
template<> Pocket::Droppable* Pocket::GameObject::AddComponent<Pocket::Droppable>() { AddComponent(32); return (Pocket::Droppable*) GetComponent(32); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Droppable>() { RemoveComponent(32); }
template<> Pocket::Droppable* Pocket::GameObject::CloneComponent<Pocket::Droppable>(GameObject* source) { CloneComponent(32, source); return (Pocket::Droppable*) GetComponent(32); }
template<> Pocket::Droppable* Pocket::GameObject::ReplaceComponent<Pocket::Droppable>(GameObject* source) { ReplaceComponent(32, source); return (Pocket::Droppable*) GetComponent(32); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Droppable>(bool enable) { EnableComponent(32, enable); }
template<> Pocket::Label* Pocket::GameObject::GetComponent<Pocket::Label>() { return (Pocket::Label*) GetComponent(33); }
template<> Pocket::Label* Pocket::GameObject::AddComponent<Pocket::Label>() { AddComponent(33); return (Pocket::Label*) GetComponent(33); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Label>() { RemoveComponent(33); }
template<> Pocket::Label* Pocket::GameObject::CloneComponent<Pocket::Label>(GameObject* source) { CloneComponent(33, source); return (Pocket::Label*) GetComponent(33); }
template<> Pocket::Label* Pocket::GameObject::ReplaceComponent<Pocket::Label>(GameObject* source) { ReplaceComponent(33, source); return (Pocket::Label*) GetComponent(33); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Label>(bool enable) { EnableComponent(33, enable); }
template<> Pocket::PanelDropper* Pocket::GameObject::GetComponent<Pocket::PanelDropper>() { return (Pocket::PanelDropper*) GetComponent(34); }
template<> Pocket::PanelDropper* Pocket::GameObject::AddComponent<Pocket::PanelDropper>() { AddComponent(34); return (Pocket::PanelDropper*) GetComponent(34); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::PanelDropper>() { RemoveComponent(34); }
template<> Pocket::PanelDropper* Pocket::GameObject::CloneComponent<Pocket::PanelDropper>(GameObject* source) { CloneComponent(34, source); return (Pocket::PanelDropper*) GetComponent(34); }
template<> Pocket::PanelDropper* Pocket::GameObject::ReplaceComponent<Pocket::PanelDropper>(GameObject* source) { ReplaceComponent(34, source); return (Pocket::PanelDropper*) GetComponent(34); }
template<> void Pocket::GameObject::EnableComponent<Pocket::PanelDropper>(bool enable) { EnableComponent(34, enable); }
template<> Pocket::Panel* Pocket::GameObject::GetComponent<Pocket::Panel>() { return (Pocket::Panel*) GetComponent(35); }
template<> Pocket::Panel* Pocket::GameObject::AddComponent<Pocket::Panel>() { AddComponent(35); return (Pocket::Panel*) GetComponent(35); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Panel>() { RemoveComponent(35); }
template<> Pocket::Panel* Pocket::GameObject::CloneComponent<Pocket::Panel>(GameObject* source) { CloneComponent(35, source); return (Pocket::Panel*) GetComponent(35); }
template<> Pocket::Panel* Pocket::GameObject::ReplaceComponent<Pocket::Panel>(GameObject* source) { ReplaceComponent(35, source); return (Pocket::Panel*) GetComponent(35); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Panel>(bool enable) { EnableComponent(35, enable); }
template<> Pocket::PanelArea* Pocket::GameObject::GetComponent<Pocket::PanelArea>() { return (Pocket::PanelArea*) GetComponent(36); }
template<> Pocket::PanelArea* Pocket::GameObject::AddComponent<Pocket::PanelArea>() { AddComponent(36); return (Pocket::PanelArea*) GetComponent(36); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::PanelArea>() { RemoveComponent(36); }
template<> Pocket::PanelArea* Pocket::GameObject::CloneComponent<Pocket::PanelArea>(GameObject* source) { CloneComponent(36, source); return (Pocket::PanelArea*) GetComponent(36); }
template<> Pocket::PanelArea* Pocket::GameObject::ReplaceComponent<Pocket::PanelArea>(GameObject* source) { ReplaceComponent(36, source); return (Pocket::PanelArea*) GetComponent(36); }
template<> void Pocket::GameObject::EnableComponent<Pocket::PanelArea>(bool enable) { EnableComponent(36, enable); }
template<> Pocket::PanelSplitter* Pocket::GameObject::GetComponent<Pocket::PanelSplitter>() { return (Pocket::PanelSplitter*) GetComponent(37); }
template<> Pocket::PanelSplitter* Pocket::GameObject::AddComponent<Pocket::PanelSplitter>() { AddComponent(37); return (Pocket::PanelSplitter*) GetComponent(37); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::PanelSplitter>() { RemoveComponent(37); }
template<> Pocket::PanelSplitter* Pocket::GameObject::CloneComponent<Pocket::PanelSplitter>(GameObject* source) { CloneComponent(37, source); return (Pocket::PanelSplitter*) GetComponent(37); }
template<> Pocket::PanelSplitter* Pocket::GameObject::ReplaceComponent<Pocket::PanelSplitter>(GameObject* source) { ReplaceComponent(37, source); return (Pocket::PanelSplitter*) GetComponent(37); }
template<> void Pocket::GameObject::EnableComponent<Pocket::PanelSplitter>(bool enable) { EnableComponent(37, enable); }
template<> Pocket::TouchableCanceller* Pocket::GameObject::GetComponent<Pocket::TouchableCanceller>() { return (Pocket::TouchableCanceller*) GetComponent(38); }
template<> Pocket::TouchableCanceller* Pocket::GameObject::AddComponent<Pocket::TouchableCanceller>() { AddComponent(38); return (Pocket::TouchableCanceller*) GetComponent(38); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::TouchableCanceller>() { RemoveComponent(38); }
template<> Pocket::TouchableCanceller* Pocket::GameObject::CloneComponent<Pocket::TouchableCanceller>(GameObject* source) { CloneComponent(38, source); return (Pocket::TouchableCanceller*) GetComponent(38); }
template<> Pocket::TouchableCanceller* Pocket::GameObject::ReplaceComponent<Pocket::TouchableCanceller>(GameObject* source) { ReplaceComponent(38, source); return (Pocket::TouchableCanceller*) GetComponent(38); }
template<> void Pocket::GameObject::EnableComponent<Pocket::TouchableCanceller>(bool enable) { EnableComponent(38, enable); }
template<> Pocket::Hoverable* Pocket::GameObject::GetComponent<Pocket::Hoverable>() { return (Pocket::Hoverable*) GetComponent(39); }
template<> Pocket::Hoverable* Pocket::GameObject::AddComponent<Pocket::Hoverable>() { AddComponent(39); return (Pocket::Hoverable*) GetComponent(39); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Hoverable>() { RemoveComponent(39); }
template<> Pocket::Hoverable* Pocket::GameObject::CloneComponent<Pocket::Hoverable>(GameObject* source) { CloneComponent(39, source); return (Pocket::Hoverable*) GetComponent(39); }
template<> Pocket::Hoverable* Pocket::GameObject::ReplaceComponent<Pocket::Hoverable>(GameObject* source) { ReplaceComponent(39, source); return (Pocket::Hoverable*) GetComponent(39); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Hoverable>(bool enable) { EnableComponent(39, enable); }
template<> Pocket::ScrollWheelMover* Pocket::GameObject::GetComponent<Pocket::ScrollWheelMover>() { return (Pocket::ScrollWheelMover*) GetComponent(40); }
template<> Pocket::ScrollWheelMover* Pocket::GameObject::AddComponent<Pocket::ScrollWheelMover>() { AddComponent(40); return (Pocket::ScrollWheelMover*) GetComponent(40); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ScrollWheelMover>() { RemoveComponent(40); }
template<> Pocket::ScrollWheelMover* Pocket::GameObject::CloneComponent<Pocket::ScrollWheelMover>(GameObject* source) { CloneComponent(40, source); return (Pocket::ScrollWheelMover*) GetComponent(40); }
template<> Pocket::ScrollWheelMover* Pocket::GameObject::ReplaceComponent<Pocket::ScrollWheelMover>(GameObject* source) { ReplaceComponent(40, source); return (Pocket::ScrollWheelMover*) GetComponent(40); }
template<> void Pocket::GameObject::EnableComponent<Pocket::ScrollWheelMover>(bool enable) { EnableComponent(40, enable); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::GetComponent<Pocket::VirtualTreeList>() { return (Pocket::VirtualTreeList*) GetComponent(41); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::AddComponent<Pocket::VirtualTreeList>() { AddComponent(41); return (Pocket::VirtualTreeList*) GetComponent(41); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::VirtualTreeList>() { RemoveComponent(41); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::CloneComponent<Pocket::VirtualTreeList>(GameObject* source) { CloneComponent(41, source); return (Pocket::VirtualTreeList*) GetComponent(41); }
template<> Pocket::VirtualTreeList* Pocket::GameObject::ReplaceComponent<Pocket::VirtualTreeList>(GameObject* source) { ReplaceComponent(41, source); return (Pocket::VirtualTreeList*) GetComponent(41); }
template<> void Pocket::GameObject::EnableComponent<Pocket::VirtualTreeList>(bool enable) { EnableComponent(41, enable); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::GetComponent<Pocket::VirtualTreeListSpawner>() { return (Pocket::VirtualTreeListSpawner*) GetComponent(42); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::AddComponent<Pocket::VirtualTreeListSpawner>() { AddComponent(42); return (Pocket::VirtualTreeListSpawner*) GetComponent(42); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::VirtualTreeListSpawner>() { RemoveComponent(42); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::CloneComponent<Pocket::VirtualTreeListSpawner>(GameObject* source) { CloneComponent(42, source); return (Pocket::VirtualTreeListSpawner*) GetComponent(42); }
template<> Pocket::VirtualTreeListSpawner* Pocket::GameObject::ReplaceComponent<Pocket::VirtualTreeListSpawner>(GameObject* source) { ReplaceComponent(42, source); return (Pocket::VirtualTreeListSpawner*) GetComponent(42); }
template<> void Pocket::GameObject::EnableComponent<Pocket::VirtualTreeListSpawner>(bool enable) { EnableComponent(42, enable); }
template<> Pocket::SlicedQuad* Pocket::GameObject::GetComponent<Pocket::SlicedQuad>() { return (Pocket::SlicedQuad*) GetComponent(43); }
template<> Pocket::SlicedQuad* Pocket::GameObject::AddComponent<Pocket::SlicedQuad>() { AddComponent(43); return (Pocket::SlicedQuad*) GetComponent(43); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SlicedQuad>() { RemoveComponent(43); }
template<> Pocket::SlicedQuad* Pocket::GameObject::CloneComponent<Pocket::SlicedQuad>(GameObject* source) { CloneComponent(43, source); return (Pocket::SlicedQuad*) GetComponent(43); }
template<> Pocket::SlicedQuad* Pocket::GameObject::ReplaceComponent<Pocket::SlicedQuad>(GameObject* source) { ReplaceComponent(43, source); return (Pocket::SlicedQuad*) GetComponent(43); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SlicedQuad>(bool enable) { EnableComponent(43, enable); }
template<> Pocket::SlicedTexture* Pocket::GameObject::GetComponent<Pocket::SlicedTexture>() { return (Pocket::SlicedTexture*) GetComponent(44); }
template<> Pocket::SlicedTexture* Pocket::GameObject::AddComponent<Pocket::SlicedTexture>() { AddComponent(44); return (Pocket::SlicedTexture*) GetComponent(44); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SlicedTexture>() { RemoveComponent(44); }
template<> Pocket::SlicedTexture* Pocket::GameObject::CloneComponent<Pocket::SlicedTexture>(GameObject* source) { CloneComponent(44, source); return (Pocket::SlicedTexture*) GetComponent(44); }
template<> Pocket::SlicedTexture* Pocket::GameObject::ReplaceComponent<Pocket::SlicedTexture>(GameObject* source) { ReplaceComponent(44, source); return (Pocket::SlicedTexture*) GetComponent(44); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SlicedTexture>(bool enable) { EnableComponent(44, enable); }
template<> Pocket::InputMapper* Pocket::GameObject::GetComponent<Pocket::InputMapper>() { return (Pocket::InputMapper*) GetComponent(45); }
template<> Pocket::InputMapper* Pocket::GameObject::AddComponent<Pocket::InputMapper>() { AddComponent(45); return (Pocket::InputMapper*) GetComponent(45); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::InputMapper>() { RemoveComponent(45); }
template<> Pocket::InputMapper* Pocket::GameObject::CloneComponent<Pocket::InputMapper>(GameObject* source) { CloneComponent(45, source); return (Pocket::InputMapper*) GetComponent(45); }
template<> Pocket::InputMapper* Pocket::GameObject::ReplaceComponent<Pocket::InputMapper>(GameObject* source) { ReplaceComponent(45, source); return (Pocket::InputMapper*) GetComponent(45); }
template<> void Pocket::GameObject::EnableComponent<Pocket::InputMapper>(bool enable) { EnableComponent(45, enable); }
template<> Pocket::InputController* Pocket::GameObject::GetComponent<Pocket::InputController>() { return (Pocket::InputController*) GetComponent(46); }
template<> Pocket::InputController* Pocket::GameObject::AddComponent<Pocket::InputController>() { AddComponent(46); return (Pocket::InputController*) GetComponent(46); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::InputController>() { RemoveComponent(46); }
template<> Pocket::InputController* Pocket::GameObject::CloneComponent<Pocket::InputController>(GameObject* source) { CloneComponent(46, source); return (Pocket::InputController*) GetComponent(46); }
template<> Pocket::InputController* Pocket::GameObject::ReplaceComponent<Pocket::InputController>(GameObject* source) { ReplaceComponent(46, source); return (Pocket::InputController*) GetComponent(46); }
template<> void Pocket::GameObject::EnableComponent<Pocket::InputController>(bool enable) { EnableComponent(46, enable); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::GetComponent<Pocket::FirstPersonMover>() { return (Pocket::FirstPersonMover*) GetComponent(47); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::AddComponent<Pocket::FirstPersonMover>() { AddComponent(47); return (Pocket::FirstPersonMover*) GetComponent(47); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FirstPersonMover>() { RemoveComponent(47); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::CloneComponent<Pocket::FirstPersonMover>(GameObject* source) { CloneComponent(47, source); return (Pocket::FirstPersonMover*) GetComponent(47); }
template<> Pocket::FirstPersonMover* Pocket::GameObject::ReplaceComponent<Pocket::FirstPersonMover>(GameObject* source) { ReplaceComponent(47, source); return (Pocket::FirstPersonMover*) GetComponent(47); }
template<> void Pocket::GameObject::EnableComponent<Pocket::FirstPersonMover>(bool enable) { EnableComponent(47, enable); }
template<> Pocket::RigidBody* Pocket::GameObject::GetComponent<Pocket::RigidBody>() { return (Pocket::RigidBody*) GetComponent(48); }
template<> Pocket::RigidBody* Pocket::GameObject::AddComponent<Pocket::RigidBody>() { AddComponent(48); return (Pocket::RigidBody*) GetComponent(48); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::RigidBody>() { RemoveComponent(48); }
template<> Pocket::RigidBody* Pocket::GameObject::CloneComponent<Pocket::RigidBody>(GameObject* source) { CloneComponent(48, source); return (Pocket::RigidBody*) GetComponent(48); }
template<> Pocket::RigidBody* Pocket::GameObject::ReplaceComponent<Pocket::RigidBody>(GameObject* source) { ReplaceComponent(48, source); return (Pocket::RigidBody*) GetComponent(48); }
template<> void Pocket::GameObject::EnableComponent<Pocket::RigidBody>(bool enable) { EnableComponent(48, enable); }
template<> Pocket::RigidBody2d* Pocket::GameObject::GetComponent<Pocket::RigidBody2d>() { return (Pocket::RigidBody2d*) GetComponent(49); }
template<> Pocket::RigidBody2d* Pocket::GameObject::AddComponent<Pocket::RigidBody2d>() { AddComponent(49); return (Pocket::RigidBody2d*) GetComponent(49); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::RigidBody2d>() { RemoveComponent(49); }
template<> Pocket::RigidBody2d* Pocket::GameObject::CloneComponent<Pocket::RigidBody2d>(GameObject* source) { CloneComponent(49, source); return (Pocket::RigidBody2d*) GetComponent(49); }
template<> Pocket::RigidBody2d* Pocket::GameObject::ReplaceComponent<Pocket::RigidBody2d>(GameObject* source) { ReplaceComponent(49, source); return (Pocket::RigidBody2d*) GetComponent(49); }
template<> void Pocket::GameObject::EnableComponent<Pocket::RigidBody2d>(bool enable) { EnableComponent(49, enable); }
template<> Pocket::Joint2d* Pocket::GameObject::GetComponent<Pocket::Joint2d>() { return (Pocket::Joint2d*) GetComponent(50); }
template<> Pocket::Joint2d* Pocket::GameObject::AddComponent<Pocket::Joint2d>() { AddComponent(50); return (Pocket::Joint2d*) GetComponent(50); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Joint2d>() { RemoveComponent(50); }
template<> Pocket::Joint2d* Pocket::GameObject::CloneComponent<Pocket::Joint2d>(GameObject* source) { CloneComponent(50, source); return (Pocket::Joint2d*) GetComponent(50); }
template<> Pocket::Joint2d* Pocket::GameObject::ReplaceComponent<Pocket::Joint2d>(GameObject* source) { ReplaceComponent(50, source); return (Pocket::Joint2d*) GetComponent(50); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Joint2d>(bool enable) { EnableComponent(50, enable); }
template<> Pocket::DistanceScaler* Pocket::GameObject::GetComponent<Pocket::DistanceScaler>() { return (Pocket::DistanceScaler*) GetComponent(51); }
template<> Pocket::DistanceScaler* Pocket::GameObject::AddComponent<Pocket::DistanceScaler>() { AddComponent(51); return (Pocket::DistanceScaler*) GetComponent(51); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::DistanceScaler>() { RemoveComponent(51); }
template<> Pocket::DistanceScaler* Pocket::GameObject::CloneComponent<Pocket::DistanceScaler>(GameObject* source) { CloneComponent(51, source); return (Pocket::DistanceScaler*) GetComponent(51); }
template<> Pocket::DistanceScaler* Pocket::GameObject::ReplaceComponent<Pocket::DistanceScaler>(GameObject* source) { ReplaceComponent(51, source); return (Pocket::DistanceScaler*) GetComponent(51); }
template<> void Pocket::GameObject::EnableComponent<Pocket::DistanceScaler>(bool enable) { EnableComponent(51, enable); }
template<> Pocket::LineRenderer* Pocket::GameObject::GetComponent<Pocket::LineRenderer>() { return (Pocket::LineRenderer*) GetComponent(52); }
template<> Pocket::LineRenderer* Pocket::GameObject::AddComponent<Pocket::LineRenderer>() { AddComponent(52); return (Pocket::LineRenderer*) GetComponent(52); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::LineRenderer>() { RemoveComponent(52); }
template<> Pocket::LineRenderer* Pocket::GameObject::CloneComponent<Pocket::LineRenderer>(GameObject* source) { CloneComponent(52, source); return (Pocket::LineRenderer*) GetComponent(52); }
template<> Pocket::LineRenderer* Pocket::GameObject::ReplaceComponent<Pocket::LineRenderer>(GameObject* source) { ReplaceComponent(52, source); return (Pocket::LineRenderer*) GetComponent(52); }
template<> void Pocket::GameObject::EnableComponent<Pocket::LineRenderer>(bool enable) { EnableComponent(52, enable); }
template<> Pocket::SceneManager* Pocket::GameObject::GetComponent<Pocket::SceneManager>() { return (Pocket::SceneManager*) GetComponent(53); }
template<> Pocket::SceneManager* Pocket::GameObject::AddComponent<Pocket::SceneManager>() { AddComponent(53); return (Pocket::SceneManager*) GetComponent(53); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::SceneManager>() { RemoveComponent(53); }
template<> Pocket::SceneManager* Pocket::GameObject::CloneComponent<Pocket::SceneManager>(GameObject* source) { CloneComponent(53, source); return (Pocket::SceneManager*) GetComponent(53); }
template<> Pocket::SceneManager* Pocket::GameObject::ReplaceComponent<Pocket::SceneManager>(GameObject* source) { ReplaceComponent(53, source); return (Pocket::SceneManager*) GetComponent(53); }
template<> void Pocket::GameObject::EnableComponent<Pocket::SceneManager>(bool enable) { EnableComponent(53, enable); }
template<> Pocket::Trigger* Pocket::GameObject::GetComponent<Pocket::Trigger>() { return (Pocket::Trigger*) GetComponent(54); }
template<> Pocket::Trigger* Pocket::GameObject::AddComponent<Pocket::Trigger>() { AddComponent(54); return (Pocket::Trigger*) GetComponent(54); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::Trigger>() { RemoveComponent(54); }
template<> Pocket::Trigger* Pocket::GameObject::CloneComponent<Pocket::Trigger>(GameObject* source) { CloneComponent(54, source); return (Pocket::Trigger*) GetComponent(54); }
template<> Pocket::Trigger* Pocket::GameObject::ReplaceComponent<Pocket::Trigger>(GameObject* source) { ReplaceComponent(54, source); return (Pocket::Trigger*) GetComponent(54); }
template<> void Pocket::GameObject::EnableComponent<Pocket::Trigger>(bool enable) { EnableComponent(54, enable); }
template<> Pocket::EditorObject* Pocket::GameObject::GetComponent<Pocket::EditorObject>() { return (Pocket::EditorObject*) GetComponent(55); }
template<> Pocket::EditorObject* Pocket::GameObject::AddComponent<Pocket::EditorObject>() { AddComponent(55); return (Pocket::EditorObject*) GetComponent(55); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::EditorObject>() { RemoveComponent(55); }
template<> Pocket::EditorObject* Pocket::GameObject::CloneComponent<Pocket::EditorObject>(GameObject* source) { CloneComponent(55, source); return (Pocket::EditorObject*) GetComponent(55); }
template<> Pocket::EditorObject* Pocket::GameObject::ReplaceComponent<Pocket::EditorObject>(GameObject* source) { ReplaceComponent(55, source); return (Pocket::EditorObject*) GetComponent(55); }
template<> void Pocket::GameObject::EnableComponent<Pocket::EditorObject>(bool enable) { EnableComponent(55, enable); }
template<> Pocket::AssetImporter* Pocket::GameObject::GetComponent<Pocket::AssetImporter>() { return (Pocket::AssetImporter*) GetComponent(57); }
template<> Pocket::AssetImporter* Pocket::GameObject::AddComponent<Pocket::AssetImporter>() { AddComponent(57); return (Pocket::AssetImporter*) GetComponent(57); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::AssetImporter>() { RemoveComponent(57); }
template<> Pocket::AssetImporter* Pocket::GameObject::CloneComponent<Pocket::AssetImporter>(GameObject* source) { CloneComponent(57, source); return (Pocket::AssetImporter*) GetComponent(57); }
template<> Pocket::AssetImporter* Pocket::GameObject::ReplaceComponent<Pocket::AssetImporter>(GameObject* source) { ReplaceComponent(57, source); return (Pocket::AssetImporter*) GetComponent(57); }
template<> void Pocket::GameObject::EnableComponent<Pocket::AssetImporter>(bool enable) { EnableComponent(57, enable); }
template<> Pocket::FieldEditor* Pocket::GameObject::GetComponent<Pocket::FieldEditor>() { return (Pocket::FieldEditor*) GetComponent(58); }
template<> Pocket::FieldEditor* Pocket::GameObject::AddComponent<Pocket::FieldEditor>() { AddComponent(58); return (Pocket::FieldEditor*) GetComponent(58); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FieldEditor>() { RemoveComponent(58); }
template<> Pocket::FieldEditor* Pocket::GameObject::CloneComponent<Pocket::FieldEditor>(GameObject* source) { CloneComponent(58, source); return (Pocket::FieldEditor*) GetComponent(58); }
template<> Pocket::FieldEditor* Pocket::GameObject::ReplaceComponent<Pocket::FieldEditor>(GameObject* source) { ReplaceComponent(58, source); return (Pocket::FieldEditor*) GetComponent(58); }
template<> void Pocket::GameObject::EnableComponent<Pocket::FieldEditor>(bool enable) { EnableComponent(58, enable); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::GetComponent<Pocket::EditorDropTarget>() { return (Pocket::EditorDropTarget*) GetComponent(59); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::AddComponent<Pocket::EditorDropTarget>() { AddComponent(59); return (Pocket::EditorDropTarget*) GetComponent(59); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::EditorDropTarget>() { RemoveComponent(59); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::CloneComponent<Pocket::EditorDropTarget>(GameObject* source) { CloneComponent(59, source); return (Pocket::EditorDropTarget*) GetComponent(59); }
template<> Pocket::EditorDropTarget* Pocket::GameObject::ReplaceComponent<Pocket::EditorDropTarget>(GameObject* source) { ReplaceComponent(59, source); return (Pocket::EditorDropTarget*) GetComponent(59); }
template<> void Pocket::GameObject::EnableComponent<Pocket::EditorDropTarget>(bool enable) { EnableComponent(59, enable); }
template<> Pocket::ProjectSettings* Pocket::GameObject::GetComponent<Pocket::ProjectSettings>() { return (Pocket::ProjectSettings*) GetComponent(60); }
template<> Pocket::ProjectSettings* Pocket::GameObject::AddComponent<Pocket::ProjectSettings>() { AddComponent(60); return (Pocket::ProjectSettings*) GetComponent(60); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::ProjectSettings>() { RemoveComponent(60); }
template<> Pocket::ProjectSettings* Pocket::GameObject::CloneComponent<Pocket::ProjectSettings>(GameObject* source) { CloneComponent(60, source); return (Pocket::ProjectSettings*) GetComponent(60); }
template<> Pocket::ProjectSettings* Pocket::GameObject::ReplaceComponent<Pocket::ProjectSettings>(GameObject* source) { ReplaceComponent(60, source); return (Pocket::ProjectSettings*) GetComponent(60); }
template<> void Pocket::GameObject::EnableComponent<Pocket::ProjectSettings>(bool enable) { EnableComponent(60, enable); }
template<> Pocket::FilePath* Pocket::GameObject::GetComponent<Pocket::FilePath>() { return (Pocket::FilePath*) GetComponent(61); }
template<> Pocket::FilePath* Pocket::GameObject::AddComponent<Pocket::FilePath>() { AddComponent(61); return (Pocket::FilePath*) GetComponent(61); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FilePath>() { RemoveComponent(61); }
template<> Pocket::FilePath* Pocket::GameObject::CloneComponent<Pocket::FilePath>(GameObject* source) { CloneComponent(61, source); return (Pocket::FilePath*) GetComponent(61); }
template<> Pocket::FilePath* Pocket::GameObject::ReplaceComponent<Pocket::FilePath>(GameObject* source) { ReplaceComponent(61, source); return (Pocket::FilePath*) GetComponent(61); }
template<> void Pocket::GameObject::EnableComponent<Pocket::FilePath>(bool enable) { EnableComponent(61, enable); }
template<> Pocket::GameWorldViewport* Pocket::GameObject::GetComponent<Pocket::GameWorldViewport>() { return (Pocket::GameWorldViewport*) GetComponent(62); }
template<> Pocket::GameWorldViewport* Pocket::GameObject::AddComponent<Pocket::GameWorldViewport>() { AddComponent(62); return (Pocket::GameWorldViewport*) GetComponent(62); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::GameWorldViewport>() { RemoveComponent(62); }
template<> Pocket::GameWorldViewport* Pocket::GameObject::CloneComponent<Pocket::GameWorldViewport>(GameObject* source) { CloneComponent(62, source); return (Pocket::GameWorldViewport*) GetComponent(62); }
template<> Pocket::GameWorldViewport* Pocket::GameObject::ReplaceComponent<Pocket::GameWorldViewport>(GameObject* source) { ReplaceComponent(62, source); return (Pocket::GameWorldViewport*) GetComponent(62); }
template<> void Pocket::GameObject::EnableComponent<Pocket::GameWorldViewport>(bool enable) { EnableComponent(62, enable); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::GetComponent<Pocket::GameObjectEditor>() { return (Pocket::GameObjectEditor*) GetComponent(63); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::AddComponent<Pocket::GameObjectEditor>() { AddComponent(63); return (Pocket::GameObjectEditor*) GetComponent(63); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::GameObjectEditor>() { RemoveComponent(63); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::CloneComponent<Pocket::GameObjectEditor>(GameObject* source) { CloneComponent(63, source); return (Pocket::GameObjectEditor*) GetComponent(63); }
template<> Pocket::GameObjectEditor* Pocket::GameObject::ReplaceComponent<Pocket::GameObjectEditor>(GameObject* source) { ReplaceComponent(63, source); return (Pocket::GameObjectEditor*) GetComponent(63); }
template<> void Pocket::GameObject::EnableComponent<Pocket::GameObjectEditor>(bool enable) { EnableComponent(63, enable); }
template<> Pocket::FileSystemListener* Pocket::GameObject::GetComponent<Pocket::FileSystemListener>() { return (Pocket::FileSystemListener*) GetComponent(64); }
template<> Pocket::FileSystemListener* Pocket::GameObject::AddComponent<Pocket::FileSystemListener>() { AddComponent(64); return (Pocket::FileSystemListener*) GetComponent(64); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::FileSystemListener>() { RemoveComponent(64); }
template<> Pocket::FileSystemListener* Pocket::GameObject::CloneComponent<Pocket::FileSystemListener>(GameObject* source) { CloneComponent(64, source); return (Pocket::FileSystemListener*) GetComponent(64); }
template<> Pocket::FileSystemListener* Pocket::GameObject::ReplaceComponent<Pocket::FileSystemListener>(GameObject* source) { ReplaceComponent(64, source); return (Pocket::FileSystemListener*) GetComponent(64); }
template<> void Pocket::GameObject::EnableComponent<Pocket::FileSystemListener>(bool enable) { EnableComponent(64, enable); }
template<> Pocket::GameObjectFieldEditor* Pocket::GameObject::GetComponent<Pocket::GameObjectFieldEditor>() { return (Pocket::GameObjectFieldEditor*) GetComponent(65); }
template<> Pocket::GameObjectFieldEditor* Pocket::GameObject::AddComponent<Pocket::GameObjectFieldEditor>() { AddComponent(65); return (Pocket::GameObjectFieldEditor*) GetComponent(65); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::GameObjectFieldEditor>() { RemoveComponent(65); }
template<> Pocket::GameObjectFieldEditor* Pocket::GameObject::CloneComponent<Pocket::GameObjectFieldEditor>(GameObject* source) { CloneComponent(65, source); return (Pocket::GameObjectFieldEditor*) GetComponent(65); }
template<> Pocket::GameObjectFieldEditor* Pocket::GameObject::ReplaceComponent<Pocket::GameObjectFieldEditor>(GameObject* source) { ReplaceComponent(65, source); return (Pocket::GameObjectFieldEditor*) GetComponent(65); }
template<> void Pocket::GameObject::EnableComponent<Pocket::GameObjectFieldEditor>(bool enable) { EnableComponent(65, enable); }
template<> Pocket::LogMessage* Pocket::GameObject::GetComponent<Pocket::LogMessage>() { return (Pocket::LogMessage*) GetComponent(66); }
template<> Pocket::LogMessage* Pocket::GameObject::AddComponent<Pocket::LogMessage>() { AddComponent(66); return (Pocket::LogMessage*) GetComponent(66); }
template<> void Pocket::GameObject::RemoveComponent<Pocket::LogMessage>() { RemoveComponent(66); }
template<> Pocket::LogMessage* Pocket::GameObject::CloneComponent<Pocket::LogMessage>(GameObject* source) { CloneComponent(66, source); return (Pocket::LogMessage*) GetComponent(66); }
template<> Pocket::LogMessage* Pocket::GameObject::ReplaceComponent<Pocket::LogMessage>(GameObject* source) { ReplaceComponent(66, source); return (Pocket::LogMessage*) GetComponent(66); }
template<> void Pocket::GameObject::EnableComponent<Pocket::LogMessage>(bool enable) { EnableComponent(66, enable); }
template<> Road* Pocket::GameObject::GetComponent<Road>() { return (Road*) GetComponent(67); }
template<> Road* Pocket::GameObject::AddComponent<Road>() { AddComponent(67); return (Road*) GetComponent(67); }
template<> void Pocket::GameObject::RemoveComponent<Road>() { RemoveComponent(67); }
template<> Road* Pocket::GameObject::CloneComponent<Road>(GameObject* source) { CloneComponent(67, source); return (Road*) GetComponent(67); }
template<> Road* Pocket::GameObject::ReplaceComponent<Road>(GameObject* source) { ReplaceComponent(67, source); return (Road*) GetComponent(67); }
template<> void Pocket::GameObject::EnableComponent<Road>(bool enable) { EnableComponent(67, enable); }
template<> RoadGenerator* Pocket::GameObject::GetComponent<RoadGenerator>() { return (RoadGenerator*) GetComponent(68); }
template<> RoadGenerator* Pocket::GameObject::AddComponent<RoadGenerator>() { AddComponent(68); return (RoadGenerator*) GetComponent(68); }
template<> void Pocket::GameObject::RemoveComponent<RoadGenerator>() { RemoveComponent(68); }
template<> RoadGenerator* Pocket::GameObject::CloneComponent<RoadGenerator>(GameObject* source) { CloneComponent(68, source); return (RoadGenerator*) GetComponent(68); }
template<> RoadGenerator* Pocket::GameObject::ReplaceComponent<RoadGenerator>(GameObject* source) { ReplaceComponent(68, source); return (RoadGenerator*) GetComponent(68); }
template<> void Pocket::GameObject::EnableComponent<RoadGenerator>(bool enable) { EnableComponent(68, enable); }
template<> RoadMovement* Pocket::GameObject::GetComponent<RoadMovement>() { return (RoadMovement*) GetComponent(69); }
template<> RoadMovement* Pocket::GameObject::AddComponent<RoadMovement>() { AddComponent(69); return (RoadMovement*) GetComponent(69); }
template<> void Pocket::GameObject::RemoveComponent<RoadMovement>() { RemoveComponent(69); }
template<> RoadMovement* Pocket::GameObject::CloneComponent<RoadMovement>(GameObject* source) { CloneComponent(69, source); return (RoadMovement*) GetComponent(69); }
template<> RoadMovement* Pocket::GameObject::ReplaceComponent<RoadMovement>(GameObject* source) { ReplaceComponent(69, source); return (RoadMovement*) GetComponent(69); }
template<> void Pocket::GameObject::EnableComponent<RoadMovement>(bool enable) { EnableComponent(69, enable); }
template<> RoadOrigin* Pocket::GameObject::GetComponent<RoadOrigin>() { return (RoadOrigin*) GetComponent(70); }
template<> RoadOrigin* Pocket::GameObject::AddComponent<RoadOrigin>() { AddComponent(70); return (RoadOrigin*) GetComponent(70); }
template<> void Pocket::GameObject::RemoveComponent<RoadOrigin>() { RemoveComponent(70); }
template<> RoadOrigin* Pocket::GameObject::CloneComponent<RoadOrigin>(GameObject* source) { CloneComponent(70, source); return (RoadOrigin*) GetComponent(70); }
template<> RoadOrigin* Pocket::GameObject::ReplaceComponent<RoadOrigin>(GameObject* source) { ReplaceComponent(70, source); return (RoadOrigin*) GetComponent(70); }
template<> void Pocket::GameObject::EnableComponent<RoadOrigin>(bool enable) { EnableComponent(70, enable); }
template<> RoadPosition* Pocket::GameObject::GetComponent<RoadPosition>() { return (RoadPosition*) GetComponent(71); }
template<> RoadPosition* Pocket::GameObject::AddComponent<RoadPosition>() { AddComponent(71); return (RoadPosition*) GetComponent(71); }
template<> void Pocket::GameObject::RemoveComponent<RoadPosition>() { RemoveComponent(71); }
template<> RoadPosition* Pocket::GameObject::CloneComponent<RoadPosition>(GameObject* source) { CloneComponent(71, source); return (RoadPosition*) GetComponent(71); }
template<> RoadPosition* Pocket::GameObject::ReplaceComponent<RoadPosition>(GameObject* source) { ReplaceComponent(71, source); return (RoadPosition*) GetComponent(71); }
template<> void Pocket::GameObject::EnableComponent<RoadPosition>(bool enable) { EnableComponent(71, enable); }
template<> RoadRenderer* Pocket::GameObject::GetComponent<RoadRenderer>() { return (RoadRenderer*) GetComponent(72); }
template<> RoadRenderer* Pocket::GameObject::AddComponent<RoadRenderer>() { AddComponent(72); return (RoadRenderer*) GetComponent(72); }
template<> void Pocket::GameObject::RemoveComponent<RoadRenderer>() { RemoveComponent(72); }
template<> RoadRenderer* Pocket::GameObject::CloneComponent<RoadRenderer>(GameObject* source) { CloneComponent(72, source); return (RoadRenderer*) GetComponent(72); }
template<> RoadRenderer* Pocket::GameObject::ReplaceComponent<RoadRenderer>(GameObject* source) { ReplaceComponent(72, source); return (RoadRenderer*) GetComponent(72); }
template<> void Pocket::GameObject::EnableComponent<RoadRenderer>(bool enable) { EnableComponent(72, enable); }
template<> Rotatable* Pocket::GameObject::GetComponent<Rotatable>() { return (Rotatable*) GetComponent(73); }
template<> Rotatable* Pocket::GameObject::AddComponent<Rotatable>() { AddComponent(73); return (Rotatable*) GetComponent(73); }
template<> void Pocket::GameObject::RemoveComponent<Rotatable>() { RemoveComponent(73); }
template<> Rotatable* Pocket::GameObject::CloneComponent<Rotatable>(GameObject* source) { CloneComponent(73, source); return (Rotatable*) GetComponent(73); }
template<> Rotatable* Pocket::GameObject::ReplaceComponent<Rotatable>(GameObject* source) { ReplaceComponent(73, source); return (Rotatable*) GetComponent(73); }
template<> void Pocket::GameObject::EnableComponent<Rotatable>(bool enable) { EnableComponent(73, enable); }
template<> TransformFollower* Pocket::GameObject::GetComponent<TransformFollower>() { return (TransformFollower*) GetComponent(74); }
template<> TransformFollower* Pocket::GameObject::AddComponent<TransformFollower>() { AddComponent(74); return (TransformFollower*) GetComponent(74); }
template<> void Pocket::GameObject::RemoveComponent<TransformFollower>() { RemoveComponent(74); }
template<> TransformFollower* Pocket::GameObject::CloneComponent<TransformFollower>(GameObject* source) { CloneComponent(74, source); return (TransformFollower*) GetComponent(74); }
template<> TransformFollower* Pocket::GameObject::ReplaceComponent<TransformFollower>(GameObject* source) { ReplaceComponent(74, source); return (TransformFollower*) GetComponent(74); }
template<> void Pocket::GameObject::EnableComponent<TransformFollower>(bool enable) { EnableComponent(74, enable); }
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
#include "MathHelper.hpp"
#include "Road.hpp"
#include "RoadGenerator.hpp"
#include "RoadMovement.hpp"
#include "RoadOrigin.hpp"
#include "RoadPosition.hpp"
#include "RoadRenderer.hpp"
#include "Rotatable.hpp"
#include "TransformFollower.hpp"

#include <string>
#include <vector>
#include "Property.hpp"
template<> struct Pocket::FieldInfoIndexer<bool> { static int Index() { return 0; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<bool>> { static int Index() { return 1; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<bool>> { static int Index() { return 2; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<bool>> { static int Index() { return 3; } };
template<> struct Pocket::FieldInfoIndexer<int> { static int Index() { return 4; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<int>> { static int Index() { return 5; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<int>> { static int Index() { return 6; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<int>> { static int Index() { return 7; } };
template<> struct Pocket::FieldInfoIndexer<float> { static int Index() { return 8; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<float>> { static int Index() { return 9; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<float>> { static int Index() { return 10; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<float>> { static int Index() { return 11; } };
template<> struct Pocket::FieldInfoIndexer<double> { static int Index() { return 12; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<double>> { static int Index() { return 13; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<double>> { static int Index() { return 14; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<double>> { static int Index() { return 15; } };
template<> struct Pocket::FieldInfoIndexer<std::__1::basic_string<char>> { static int Index() { return 16; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<std::__1::basic_string<char>>> { static int Index() { return 17; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<std::__1::basic_string<char> >> { static int Index() { return 18; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<std::__1::basic_string<char> >> { static int Index() { return 19; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::GameObjectHandle> { static int Index() { return 20; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<Pocket::GameObjectHandle>> { static int Index() { return 21; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<Pocket::GameObjectHandle>> { static int Index() { return 22; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<Pocket::GameObjectHandle>> { static int Index() { return 23; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Vector3> { static int Index() { return 27; } };
template<> struct Pocket::FieldInfoIndexer<std::vector<Pocket::Vector3>> { static int Index() { return 28; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Property<Pocket::Vector3>> { static int Index() { return 29; } };
template<> struct Pocket::FieldInfoIndexer<Pocket::Timeline<Pocket::Vector3>> { static int Index() { return 30; } };

extern "C" int CountSystems() {
   return 8;
}
extern "C" IGameSystem* CreateSystem(int systemID) {
   switch (systemID) { 
      case 78: return new RoadGeneratorSystem();
      case 79: return new RoadMovementSystem();
      case 80: return new RoadOriginSystem();
      case 81: return new RoadPositionSystem();
      case 82: return new RoadRendererSystem();
      case 83: return new RoadSystem();
      case 84: return new RotatableSystem();
      case 85: return new TransformFollowerSystem();
      default: return 0;
   }
}
extern "C" void DeleteSystem(IGameSystem* scriptSystem) {
   delete scriptSystem; 
}
extern "C" int CountComponents() {
   return 8;
}
extern "C" void* CreateComponent(int componentID) {
   switch (componentID) { 
      case 67: return new Road();
      case 68: return new RoadGenerator();
      case 69: return new RoadMovement();
      case 70: return new RoadOrigin();
      case 71: return new RoadPosition();
      case 72: return new RoadRenderer();
      case 73: return new Rotatable();
      case 74: return new TransformFollower();
      default: return 0;
   }
}
extern "C" void DeleteComponent(int componentID, void* component) {
   switch (componentID) { 
      case 67: { delete ((Road*)component); break; }
      case 68: { delete ((RoadGenerator*)component); break; }
      case 69: { delete ((RoadMovement*)component); break; }
      case 70: { delete ((RoadOrigin*)component); break; }
      case 71: { delete ((RoadPosition*)component); break; }
      case 72: { delete ((RoadRenderer*)component); break; }
      case 73: { delete ((Rotatable*)component); break; }
      case 74: { delete ((TransformFollower*)component); break; }
   }
}
extern "C" void ResetComponent(int componentID, void* c, void* s) {
   switch (componentID) { 
      case 67: { Road* co = (Road*)c; 
      Road* so = ((Road*)s);
        co->operator=(*so);             break; }
      case 68: { RoadGenerator* co = (RoadGenerator*)c; 
      RoadGenerator* so = ((RoadGenerator*)s);
        co->operator=(*so);             break; }
      case 69: { RoadMovement* co = (RoadMovement*)c; 
      RoadMovement* so = ((RoadMovement*)s);
        co->operator=(*so);             break; }
      case 70: { RoadOrigin* co = (RoadOrigin*)c; 
      RoadOrigin* so = ((RoadOrigin*)s);
        co->operator=(*so);             break; }
      case 71: { RoadPosition* co = (RoadPosition*)c; 
      RoadPosition* so = ((RoadPosition*)s);
        co->operator=(*so);             break; }
      case 72: { RoadRenderer* co = (RoadRenderer*)c; 
      RoadRenderer* so = ((RoadRenderer*)s);
        co->operator=(*so);             break; }
      case 73: { Rotatable* co = (Rotatable*)c; 
      Rotatable* so = ((Rotatable*)s);
        co->operator=(*so);             break; }
      case 74: { TransformFollower* co = (TransformFollower*)c; 
      TransformFollower* so = ((TransformFollower*)s);
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
      case 67: {
      Road* component = (Road*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Road";
	      info->AddScriptField(component->Origin, "Origin");
	      info->AddScriptField(component->positions, "positions");
	      info->AddScriptField(component->rotations, "rotations");
	      info->AddScriptField(component->widths, "widths");
      return info;
      break; }
      case 68: {
      RoadGenerator* component = (RoadGenerator*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "RoadGenerator";
	      info->AddScriptField(component->frequency, "frequency");
	      info->AddScriptField(component->time, "time");
      return info;
      break; }
      case 69: {
      RoadMovement* component = (RoadMovement*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "RoadMovement";
	      info->AddScriptField(component->speed, "speed");
      return info;
      break; }
      case 70: {
      RoadOrigin* component = (RoadOrigin*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "RoadOrigin";
	      info->AddScriptField(component->variable, "variable");
      return info;
      break; }
      case 71: {
      RoadPosition* component = (RoadPosition*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "RoadPosition";
	      info->AddScriptField(component->Position, "Position");
      return info;
      break; }
      case 72: {
      RoadRenderer* component = (RoadRenderer*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "RoadRenderer";
	      info->AddScriptField(component->Depth, "Depth");
	      info->AddScriptField(component->origin, "origin");
	      info->AddScriptField(component->renderOffset, "renderOffset");
	      info->AddScriptField(component->segments, "segments");
	      info->AddScriptField(component->vScale, "vScale");
      return info;
      break; }
      case 73: {
      Rotatable* component = (Rotatable*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "Rotatable";
	      info->AddScriptField(component->angularVelocity, "angularVelocity");
	      info->AddScriptField(component->name, "name");
      return info;
      break; }
      case 74: {
      TransformFollower* component = (TransformFollower*)componentPtr;
	      Pocket::TypeInfo* info = new Pocket::TypeInfo();
	      info->name = "TransformFollower";
	      info->AddScriptField(component->lookAt, "lookAt");
	      info->AddScriptField(component->position, "position");
	      info->AddScriptField(component->smoothness, "smoothness");
	      info->AddScriptField(component->target, "target");
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
      case 67: 
         return "Road";
      case 68: 
         return "RoadGenerator";
      case 69: 
         return "RoadMovement";
      case 70: 
         return "RoadOrigin";
      case 71: 
         return "RoadPosition";
      case 72: 
         return "RoadRenderer";
      case 73: 
         return "Rotatable";
      case 74: 
         return "TransformFollower";
      default: return "";
   }
}
extern "C" int* GetSystems() {
 return new int[28] {-1,68,67,-1,69,71,-1,70,71,67,-1,71,67,12,-1,72,67,16,71,-1,67,-1,73,12,-1,74,12,-2,};}
extern "C" void DeleteGetSystems(int* indicies) {
delete indicies;
}
