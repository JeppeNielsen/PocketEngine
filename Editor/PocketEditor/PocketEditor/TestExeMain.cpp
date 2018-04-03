#include "Engine.hpp"
#include "File.hpp"
#include "FileArchive.hpp"
#include "AnimatorSystem.hpp"
#include "AssetManager.hpp"
#include "SoundSystem.hpp"
#include "ClonerSystem.hpp"
#include "HierarchyOrder.hpp"
#include "ParticleMeshUpdater.hpp"
#include "ParticleUpdaterSystem.hpp"
#include "Gui.hpp"
#include "DroppableSystem.hpp"
#include "FontTextureSystem.hpp"
#include "LabelMeshSystem.hpp"
#include "LayoutSystem.hpp"
#include "SlicedQuadMeshSystem.hpp"
#include "SpriteMeshSystem.hpp"
#include "SpriteTextureSystem.hpp"
#include "TextBoxLabelSystem.hpp"
#include "TextBoxSystem.hpp"
#include "InputMapperSystem.hpp"
#include "HoverSystem.hpp"
#include "TouchCancelSystem.hpp"
#include "TouchSystem.hpp"
#include "DraggableMotionSystem.hpp"
#include "DraggableSystem.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "LimitableSystem.hpp"
#include "VelocitySystem.hpp"
#include "ScrollWheelMoverSystem.hpp"
#include "ColorSystem.hpp"
#include "DistanceScalerSystem.hpp"
#include "LineRendererSystem.hpp"
#include "RenderSystem.hpp"
#include "SceneManagerSystem.hpp"
#include "ClickSelectorSystem.hpp"
#include "DragSelector.hpp"
#include "SelectableCollection.hpp"
#include "SelectableDragSystem.hpp"
#include "SelectedColorerSystem.hpp"
#include "TransformHierarchy.hpp"
#include "SpawnerSystem.hpp"
#include "TriggerSystem.hpp"
#include "TriggerTouchSystem.hpp"
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
#include "GameObjectJsonSerializer.hpp"

using namespace Pocket;

void InitializeStorage(GameStorage & storage) {
    storage.AddSystemType < SpawnerSystem > ();
    storage.AddSystemType < AnimatorSystem > ();
    storage.AddSystemType < AssetManager > ();
    storage.AddSystemType < SoundSystem > ();
    storage.AddSystemType < ClonerSystem > ();
    storage.AddSystemType < HierarchyOrder > ();
    storage.AddSystemType < ParticleMeshUpdater > ();
    storage.AddSystemType < ParticleUpdaterSystem > ();
    storage.AddSystemType < Gui > ();
    storage.AddSystemType < SlicedQuadMeshSystem > ();
    storage.AddSystemType < InputMapperSystem > ();
    storage.AddSystemType < TouchCancelSystem > ();
    storage.AddSystemType < TouchSystem > ();
    storage.AddSystemType < DraggableMotionSystem > ();
    storage.AddSystemType < DraggableSystem > ();
    storage.AddSystemType < FirstPersonMoverSystem > ();
    storage.AddSystemType < LimitableSystem > ();
    storage.AddSystemType < VelocitySystem > ();
    storage.AddSystemType < ColorSystem > ();
    storage.AddSystemType < DistanceScalerSystem > ();
    storage.AddSystemType < LineRendererSystem > ();
    storage.AddSystemType < RenderSystem > ();
    storage.AddSystemType < SceneManagerSystem > ();
    storage.AddSystemType < ClickSelectorSystem > ();
    storage.AddSystemType < DragSelector > ();
    storage.AddSystemType < SelectableDragSystem > ();
    storage.AddSystemType < SelectedColorerSystem > ();
    storage.AddSystemType < TransformHierarchy > ();
    storage.AddSystemType < TriggerSystem > ();
    storage.AddSystemType < TriggerTouchSystem > ();
}
void CreateScriptSystems(Pocket::GameObject & scene);
void CreateDefaultSystems(Pocket::GameObject & scene) {
    scene.CreateSystem < SpawnerSystem > ();
    scene.CreateSystem < AnimatorSystem > ();
    scene.CreateSystem < AssetManager > ();
    scene.CreateSystem < SoundSystem > ();
    scene.CreateSystem < ClonerSystem > ();
    scene.CreateSystem < HierarchyOrder > ();
    scene.CreateSystem < ParticleMeshUpdater > ();
    scene.CreateSystem < ParticleUpdaterSystem > ();
    scene.CreateSystem < Gui > ();
    scene.CreateSystem < SlicedQuadMeshSystem > ();
    scene.CreateSystem < InputMapperSystem > ();
    scene.CreateSystem < TouchCancelSystem > ();
    scene.CreateSystem < TouchSystem > ();
    scene.CreateSystem < DraggableMotionSystem > ();
    scene.CreateSystem < DraggableSystem > ();
    scene.CreateSystem < FirstPersonMoverSystem > ();
    scene.CreateSystem < LimitableSystem > ();
    scene.CreateSystem < VelocitySystem > ();
    scene.CreateSystem < ColorSystem > ();
    scene.CreateSystem < DistanceScalerSystem > ();
    scene.CreateSystem < LineRendererSystem > ();
    scene.CreateSystem < RenderSystem > ();
    scene.CreateSystem < SceneManagerSystem > ();
    scene.CreateSystem < ClickSelectorSystem > ();
    scene.CreateSystem < DragSelector > ();
    scene.CreateSystem < SelectableDragSystem > ();
    scene.CreateSystem < SelectedColorerSystem > ();
    scene.CreateSystem < TransformHierarchy > ();
    scene.CreateSystem < TriggerSystem > ();
    scene.CreateSystem < TriggerTouchSystem > ();
}

class GameCode: public Pocket::GameState < GameCode > {
    public: Pocket::GameStorage storage;
    Pocket::GameWorld world;
    Pocket::FileArchive fileArchive;
    
    void Initialize() {
    
        storage.CreateSerializer < GameObjectJsonSerializer > ();
        storage.PrefabLoaded = [this] (GameObject* prefab) {
            prefab->CreateSystem<AssetManager>();
        };
    
        InitializeStorage(storage);
        world.Initialize(storage);
        std::string zipFile = "resources";
        fileArchive.Initialize(zipFile);
        File::SetArchive(fileArchive);
        storage.GuidToRoot = [this](const std::string & guid) {
            GameObject * root = nullptr;
            if (!fileArchive.TryLoadData(guid, [ & root, & guid, this](void * data, size_t size) {
                    std::stringstream ss;
                    ss.write((const char * ) data, size);
                    root = storage.LoadPrefab(guid, ss);
                })) {
                std::cout << "unable to load: " << guid << std::endl;
            }
            return root;
        };
        storage.GuidToPath = [](const std::string & guid) {
            return guid;
        };
        auto startScenePrefab = storage.TryGetPrefab("8zJP6GddQayowtpjUDt3ug==");
        auto startScene = world.CreateScene(startScenePrefab);
        CreateDefaultSystems( * startScene);
    }
    void Update(float dt) {
        Context().InputDevice().UpdateInputManager( & world.Input());
        world.Update(dt);
    }
    void Render() {
        world.Render();
    }
};
int main_testExe() {
    Pocket::Engine e;
    e.Start < GameCode > ();
    return 0;
}
