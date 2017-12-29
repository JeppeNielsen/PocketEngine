#include "Engine.hpp"
#include "File.hpp"
#include "FileArchive.hpp"
#include "RenderSystem.hpp"
#include "TransformHierarchy.hpp"
#include "TouchSystem.hpp"
#include "ClonerSystem.hpp"
#include "InputMapperSystem.hpp"
#include "VelocitySystem.hpp"
#include "Gui.hpp"
#include "SwitchSystem.hpp"
#include "SwitchEnablerSystem.hpp"
#include "TouchSwitchSystem.hpp"
#include "SlicedQuadMeshSystem.hpp"
#include "AssetManager.hpp"
#include "TriggerSystem.hpp"
#include "TriggerTouchSystem.hpp"
#include "SceneManagerSystem.hpp"
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
void CreateDefaultSystems(Pocket::GameObject &world) { world.CreateSystem<Pocket::RenderSystem>(); world.CreateSystem<Pocket::TransformHierarchy>(); world.CreateSystem<Pocket::TouchSystem>()->TouchDepth = 0; world.CreateSystem<Pocket::ClonerSystem>(); world.CreateSystem<Pocket::InputMapperSystem>(); world.CreateSystem<Pocket::VelocitySystem>(); world.CreateSystem<Pocket::Gui>(); world.CreateSystem<Pocket::SwitchSystem>(); world.CreateSystem<Pocket::SwitchEnablerSystem>(); world.CreateSystem<Pocket::TouchSwitchSystem>(); world.CreateSystem<Pocket::SlicedQuadMeshSystem>(); world.CreateSystem<Pocket::AssetManager>(); world.CreateSystem<Pocket::TriggerSystem>(); world.CreateSystem<Pocket::TriggerTouchSystem>(); world.CreateSystem<Pocket::SceneManagerSystem>();
}
void CreateScriptSystems(Pocket::GameObject &world) {
world.World()->AddComponentTypeWithGetType<RectCollider>([] (const Pocket::GameObject* object) -> Pocket::TypeInfo {
RectCollider* component = object->GetComponent<RectCollider>();
      Pocket::TypeInfo typeInfo;
	      typeInfo.name = "RectCollider";
	      typeInfo.AddField(component->collisions, "collisions");
	      typeInfo.AddField(component->previousCollisions, "previousCollisions");
return typeInfo;
});
world.World()->AddComponentTypeWithGetType<Rotator>([] (const Pocket::GameObject* object) -> Pocket::TypeInfo {
Rotator* component = object->GetComponent<Rotator>();
      Pocket::TypeInfo typeInfo;
	      typeInfo.name = "Rotator";
	      typeInfo.AddField(component->speed, "speed");
return typeInfo;
});
world.World()->AddComponentTypeWithGetType<TransformController>([] (const Pocket::GameObject* object) -> Pocket::TypeInfo {
TransformController* component = object->GetComponent<TransformController>();
      Pocket::TypeInfo typeInfo;
	      typeInfo.name = "TransformController";
	      typeInfo.AddField(component->speed, "speed");
return typeInfo;
});
world.CreateSystem<RectCollisionSystem>();
world.CreateSystem<RotatorSystem>();
world.CreateSystem<TransformControllerSystem>();
world.CreateSystem<VelocityCollider>();
}
class GameCode : public Pocket::GameState<GameCode> { public: Pocket::GameWorld world; Pocket::FileArchive fileArchive; void Initialize() { std::string zipFile = "resources"; fileArchive.Initialize(zipFile); File::SetArchive(fileArchive); world.GuidToRoot = [this] (const std::string& guid) { GameObject* root = 0; if (!fileArchive.TryLoadData(guid, [&root, this] (void* data, size_t size) { std::stringstream ss; ss.write((const char*)data, size); root = world.CreateRootFromJson(ss, [](GameObject* root) { CreateDefaultSystems(*root); CreateScriptSystems(*root); }); })) { std::cout << "unable to load: "<<guid <<std::endl; } return root; }; world.GuidToPath = [] (const std::string& guid) { return guid; };world.SetLayerScene(0, world.TryFindRoot("9KkPi0bmTS6f+W3nOFgVRw=="));

    std::cout << Context().ScreenSize << std::endl;

} void Update(float dt) { Context().InputDevice().UpdateInputManager(&world.Input()); world.Update(dt); } void Render() { world.Render(); } }; int main() { Pocket::Engine e; e.Start<GameCode>(); return 0; }
