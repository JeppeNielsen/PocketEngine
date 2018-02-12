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

void SystemHelper::AddGameSystems(Pocket::GameStorage& storage) {
    
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
    
    
    //Gui
    


/*
    world.AddSystemType<RenderSystem>();
    world.AddSystemType<TransformHierarchy>();
    world.AddSystemType<TouchSystem>()->TouchDepth = 0;
    world.AddSystemType<ClonerSystem>();
    world.AddSystemType<InputMapperSystem>();
    world.AddSystemType<VelocitySystem>();
    world.AddSystemType<Gui>();
//    world.AddSystemType<SwitchSystem>();
//    world.AddSystemType<SwitchEnablerSystem>();
//    world.AddSystemType<TouchSwitchSystem>();
    world.AddSystemType<SlicedQuadMeshSystem>();
    world.AddSystemType<TriggerSystem>();
    world.AddSystemType<TriggerTouchSystem>();
    world.AddSystemType<SceneManagerSystem>();
    world.AddSystemType<AssetManager>();
    
    */
}

void SystemHelper::AddEditorSystems(Pocket::GameStorage& storage) {
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

bool SystemHelper::IsComponentEditorSpecific(int componentType) {
    if (componentType == GameIdHelper::GetComponentID<EditorObject>()) return true;
    if (componentType == GameIdHelper::GetComponentID<EditorProxyComponent<Camera>>()) return true;
    return false;
}
