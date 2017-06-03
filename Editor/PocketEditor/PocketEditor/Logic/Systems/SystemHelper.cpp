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
#include "TouchAnimatorSystem.hpp"
#include "TransformAnimatorSystem.hpp"

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
#include "DroppableSystem.hpp"
#include "FontTextureSystem.hpp"
#include "LabelMeshSystem.hpp"
#include "LayoutSystem.hpp"
#include "SlicedQuadMeshSystem.hpp"
#include "SpriteMeshSystem.hpp"
#include "SpriteTextureSystem.hpp"
#include "TextBoxLabelSystem.hpp"
#include "TextBoxSystem.hpp"

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

using namespace Pocket;

void SystemHelper::AddGameSystems(Pocket::GameObject &world) {
    
    //spawning
    world.CreateSystem<SpawnerSystem>();
    
    //Animations
    world.CreateSystem<TouchAnimatorSystem>();
    world.CreateSystem<TransformAnimatorSystem>();

    //Assets
    world.CreateSystem<AssetManager>();

    //Audio
    world.CreateSystem<SoundSystem>();

    //Cloning
    world.CreateSystem<ClonerSystem>();

    //Common
    world.CreateSystem<HierarchyOrder>();

    //Effects
    world.CreateSystem<ParticleMeshUpdater>();
    world.CreateSystem<ParticleUpdaterSystem>();

    //Gui
    world.CreateSystem<DroppableSystem>();
    world.CreateSystem<FontTextureSystem>();
    world.CreateSystem<LabelMeshSystem>();
    world.CreateSystem<LayoutSystem>();
    world.CreateSystem<SlicedQuadMeshSystem>();
    world.CreateSystem<SpriteMeshSystem>();
    world.CreateSystem<SpriteTextureSystem>();
    world.CreateSystem<TextBoxLabelSystem>();
    world.CreateSystem<TextBoxSystem>();

    //Input
    world.CreateSystem<InputMapperSystem>();

    //Interaction
    world.CreateSystem<TouchCancelSystem>();
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;

    //Movement
    world.CreateSystem<DraggableMotionSystem>();
    world.CreateSystem<DraggableSystem>();
    world.CreateSystem<FirstPersonMoverSystem>();
    world.CreateSystem<LimitableSystem>();
    world.CreateSystem<VelocitySystem>();

    //Physics
    world.CreateSystem<PhysicsSystem>();
    world.CreateSystem<PhysicsSystem2d>();

    //Rendering
    world.CreateSystem<ColorSystem>();
    world.CreateSystem<DistanceScalerSystem>();
    world.CreateSystem<LineRendererSystem>();
    world.CreateSystem<RenderSystem>();

    //Scenes
    world.CreateSystem<SceneManagerSystem>();

    //Selection
    world.CreateSystem<ClickSelectorSystem>();
    world.CreateSystem<DragSelector>();
    world.CreateSystem<SelectableDragSystem>();
    world.CreateSystem<SelectedColorerSystem>();

    //Spatial
    world.CreateSystem<TransformHierarchy>();

    //Triggering
    world.CreateSystem<TriggerSystem>();
    world.CreateSystem<TriggerTouchSystem>();
    
    
    


/*
    world.CreateSystem<RenderSystem>();
    world.CreateSystem<TransformHierarchy>();
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;
    world.CreateSystem<ClonerSystem>();
    world.CreateSystem<InputMapperSystem>();
    world.CreateSystem<VelocitySystem>();
    world.CreateSystem<Gui>();
//    world.CreateSystem<SwitchSystem>();
//    world.CreateSystem<SwitchEnablerSystem>();
//    world.CreateSystem<TouchSwitchSystem>();
    world.CreateSystem<SlicedQuadMeshSystem>();
    world.CreateSystem<TriggerSystem>();
    world.CreateSystem<TriggerTouchSystem>();
    world.CreateSystem<SceneManagerSystem>();
    world.CreateSystem<AssetManager>();
    
    */
}

void SystemHelper::AddEditorSystems(Pocket::GameObject& editorWorld) {
    editorWorld.CreateSystem<RenderSystem>();
    editorWorld.CreateSystem<TouchSystem>();
    editorWorld.CreateSystem<DraggableSystem>();
    editorWorld.CreateSystem<EditorTransformSystem>();
    editorWorld.CreateSystem<EditorMeshSystem>();
    editorWorld.CreateSystem<EditorSizeableSystem>();
    
    editorWorld.CreateSystem<TransformHierarchy>();
    
    editorWorld.CreateSystem<ClickSelectorSystem>();
    editorWorld.CreateSystem<DragSelector>()->Setup({2000,2000});
    editorWorld.CreateSystem<SelectableDragSystem>();
    editorWorld.CreateSystem<TouchSystem>()->TouchDepth = 5;
    editorWorld.CreateSystem<SelectedColorerSystem>();
    editorWorld.CreateSystem<FirstPersonMoverSystem>();
    editorWorld.CreateSystem<SelectableCollection<EditorObject>>();
    
    editorWorld.CreateSystem<LineRendererSystem>();
    
    editorWorld.CreateSystem<EditorCameraSelection>();
    
    editorWorld.CreateSystem<DistanceScalerSystem>();
    editorWorld.CreateSystem<EditorSizeableSelection>();
}

bool SystemHelper::IsComponentEditorSpecific(int componentType) {
    if (componentType == GameIdHelper::GetComponentID<EditorObject>()) return true;
    if (componentType == GameIdHelper::GetComponentID<EditorProxyComponent<Camera>>()) return true;
    return false;
}
