//
//  SystemHelper.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 19/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "SystemHelper.hpp"

//-------------------- Game Systems --------------------

//Animation
#include "AnimatorSystem.hpp"

//Assets
#include "AssetManager.hpp"


//Audio
#include "SoundSystem.hpp"

//Cloning
#include "ClonerSystem.hpp"

//Common
#include "HierarchyOrder.hpp"

//Effects
#include "ParticleMeshUpdater.hpp"
#include "ParticleUpdaterSystem.hpp"

//Gui
#include "Gui.hpp"
#include "SlicedQuadMeshSystem.hpp"

//Input
#include "InputMapperSystem.hpp"

//Interaction
#include "TouchCancelSystem.hpp"
#include "TouchSystem.hpp"

//Movement
#include "DraggableMotionSystem.hpp"
#include "DraggableSystem.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "LimitableSystem.hpp"
#include "VelocitySystem.hpp"

//Physics
#include "PhysicsSystem.hpp"
#include "PhysicsSystem2d.hpp"

//Rendering
#include "ColorSystem.hpp"
#include "DistanceScalerSystem.hpp"
#include "LineRendererSystem.hpp"
#include "RenderSystem.hpp"

//Scenes
#include "SceneManagerSystem.hpp"

//Selection
#include "ClickSelectorSystem.hpp"
#include "DragSelector.hpp"
#include "SelectableCollection.hpp"
#include "SelectableDragSystem.hpp"
#include "SelectedColorerSystem.hpp"

//Spatial
#include "TransformHierarchy.hpp"

//Triggering
#include "TriggerSystem.hpp"
#include "TriggerTouchSystem.hpp"

//Spawning
#include "SpawnerSystem.hpp"

//-------------------- Editor System --------------------

#include "EditorTransformSystem.hpp"
#include "EditorMeshSystem.hpp"
#include "EditorSizeableSystem.hpp"
#include "EditorObject.hpp"
#include "EditorCameraSelection.hpp"
#include "EditorSizeableSelection.hpp"
#include "AssetImporterSystem.hpp"
#include "FieldEditorSystem.hpp"
#include "EditorDropTarget.hpp"
#include "ProjectSettings.hpp"
#include "FilePath.hpp"
#include "GameWorldViewportSystem.hpp"
#include "GameObjectEditorSystem.hpp"
#include "FileSystemListenerSystem.hpp"
#include "EditorGui.hpp"
#include "GameObjectFieldEditor.hpp"

using namespace Pocket;

void SystemHelper::InitializeStorage(GameStorage& storage) {

    //spawning
    storage.AddSystemType<SpawnerSystem>();
    
    //Animations
    storage.AddSystemType<AnimatorSystem>();

    //Assets
    storage.AddSystemType<AssetManager>();

    //Audio
    storage.AddSystemType<SoundSystem>();

    //Cloning
    storage.AddSystemType<ClonerSystem>();

    //Common
    storage.AddSystemType<HierarchyOrder>();

    //Effects
    storage.AddSystemType<ParticleMeshUpdater>();
    storage.AddSystemType<ParticleUpdaterSystem>();

    //Gui
    storage.AddSystemType<Gui>();
    storage.AddSystemType<SlicedQuadMeshSystem>();
    
    //Input
    storage.AddSystemType<InputMapperSystem>();

    //Interaction
    storage.AddSystemType<TouchCancelSystem>();
    storage.AddSystemType<TouchSystem>();//->TouchDepth = 0;

    //Movement
    storage.AddSystemType<DraggableMotionSystem>();
    storage.AddSystemType<DraggableSystem>();
    storage.AddSystemType<FirstPersonMoverSystem>();
    storage.AddSystemType<LimitableSystem>();
    storage.AddSystemType<VelocitySystem>();

    //Physics
    storage.AddSystemType<PhysicsSystem>();
    storage.AddSystemType<PhysicsSystem2d>();

    //Rendering
    storage.AddSystemType<ColorSystem>();
    storage.AddSystemType<DistanceScalerSystem>();
    storage.AddSystemType<LineRendererSystem>();
    storage.AddSystemType<RenderSystem>();

    //Scenes
    storage.AddSystemType<SceneManagerSystem>();

    //Selection
    storage.AddSystemType<ClickSelectorSystem>();
    storage.AddSystemType<DragSelector>();
    storage.AddSystemType<SelectableDragSystem>();
    storage.AddSystemType<SelectedColorerSystem>();

    //Spatial
    storage.AddSystemType<TransformHierarchy>();

    //Triggering
    storage.AddSystemType<TriggerSystem>();
    storage.AddSystemType<TriggerTouchSystem>();


    //editor systems
    storage.AddSystemType<EditorTransformSystem>();
    storage.AddSystemType<EditorMeshSystem>();
    storage.AddSystemType<EditorSizeableSystem>();
    
    storage.AddSystemType<SelectableCollection<EditorObject>>();
    
    storage.AddSystemType<EditorCameraSelection>();
    
    storage.AddSystemType<EditorSizeableSelection>();
    
    storage.AddSystemType<AssetImporterSystem>();
    storage.AddSystemType<FieldEditorSystem>();
    
    storage.AddComponentType<EditorDropTarget>();
    storage.AddComponentType<ProjectSettings>();
    storage.AddComponentType<FilePath>();
    
    storage.AddSystemType<GameWorldViewportSystem>();
    
    storage.AddSystemType<GameObjectEditorSystem>();
    
    storage.AddSystemType<FileSystemListenerSystem>();
    storage.AddSystemType<EditorGui>();
    
    storage.AddComponentType<GameObjectFieldEditor>();

}

void SystemHelper::AddGameSystems(Pocket::GameObject& scene) {
    
    
    //spawning
    scene.CreateSystem<SpawnerSystem>();
    
    //Animations
    scene.CreateSystem<AnimatorSystem>();

    //Assets
    scene.CreateSystem<AssetManager>();

    //Audio
    scene.CreateSystem<SoundSystem>();

    //Cloning
    scene.CreateSystem<ClonerSystem>();

    //Common
    scene.CreateSystem<HierarchyOrder>();

    //Effects
    scene.CreateSystem<ParticleMeshUpdater>();
    scene.CreateSystem<ParticleUpdaterSystem>();

    //Gui
    scene.CreateSystem<Gui>();
    scene.CreateSystem<SlicedQuadMeshSystem>();
    
    //Input
    scene.CreateSystem<InputMapperSystem>();

    //Interaction
    scene.CreateSystem<TouchCancelSystem>();
    scene.CreateSystem<TouchSystem>();//->TouchDepth = 0;

    //Movement
    scene.CreateSystem<DraggableMotionSystem>();
    scene.CreateSystem<DraggableSystem>();
    scene.CreateSystem<FirstPersonMoverSystem>();
    scene.CreateSystem<LimitableSystem>();
    scene.CreateSystem<VelocitySystem>();

    //Physics
    scene.CreateSystem<PhysicsSystem>();
    scene.CreateSystem<PhysicsSystem2d>();

    //Rendering
    scene.CreateSystem<ColorSystem>();
    scene.CreateSystem<DistanceScalerSystem>();
    scene.CreateSystem<LineRendererSystem>();
    scene.CreateSystem<RenderSystem>();

    //Scenes
    scene.CreateSystem<SceneManagerSystem>();

    //Selection
    scene.CreateSystem<ClickSelectorSystem>();
    scene.CreateSystem<DragSelector>();
    scene.CreateSystem<SelectableDragSystem>();
    scene.CreateSystem<SelectedColorerSystem>();

    //Spatial
    scene.CreateSystem<TransformHierarchy>();

    //Triggering
    scene.CreateSystem<TriggerSystem>();
    scene.CreateSystem<TriggerTouchSystem>();
    
}

void SystemHelper::AddEditorSystems(Pocket::GameObject& scene) {
    scene.CreateSystem<EditorTransformSystem>();
    scene.CreateSystem<EditorMeshSystem>();
    scene.CreateSystem<EditorSizeableSystem>();
    
    scene.CreateSystem<SelectableCollection<EditorObject>>();
    
    scene.CreateSystem<EditorCameraSelection>();
    
    scene.CreateSystem<EditorSizeableSelection>();
    
    scene.CreateSystem<AssetImporterSystem>();
    scene.CreateSystem<FieldEditorSystem>();
    
    scene.CreateSystem<GameWorldViewportSystem>();
    
    scene.CreateSystem<GameObjectEditorSystem>();
    
    scene.CreateSystem<FileSystemListenerSystem>();
    scene.CreateSystem<EditorGui>();
    
    scene.CreateSystem<Gui>();

    scene.CreateSystem<ClickSelectorSystem>();
    scene.CreateSystem<DragSelector>();
    scene.CreateSystem<SelectableDragSystem>();
    scene.CreateSystem<SelectedColorerSystem>();
    
    scene.CreateSystem<FirstPersonMoverSystem>();
    
    scene.CreateSystem<ColorSystem>();
    scene.CreateSystem<DistanceScalerSystem>();
    scene.CreateSystem<LineRendererSystem>()->Order = 1000;
    scene.CreateSystem<RenderSystem>();
}

bool SystemHelper::IsComponentEditorSpecific(int componentType) {
    if (componentType == GameIdHelper::GetComponentID<EditorObject>()) return true;
    if (componentType == GameIdHelper::GetComponentID<EditorProxyComponent<Camera>>()) return true;
    return false;
}

void SystemHelper::SetScriptStorage(Pocket::GameStorage& storage, Pocket::ScriptWorld& scriptWorld) {
    scriptWorld.SetStorage(storage, [] (int componentId) {
        return componentId!=GameIdHelper::GetComponentID<EditorProxyComponent<Camera>>();
    });
}
