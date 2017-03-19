//
//  SystemHelper.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 19/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "SystemHelper.hpp"
#include "RenderSystem.hpp"
#include "TouchSystem.hpp"
#include "TransformHierarchy.hpp"
#include "FileHelper.hpp"
#include "InputManager.hpp"

#include "DraggableSystem.hpp"
#include "EditorTransformSystem.hpp"
#include "EditorMeshSystem.hpp"
#include "EditorSizeableSystem.hpp"
#include "ClickSelectorSystem.hpp"
#include "DragSelector.hpp"
#include "SelectableDragSystem.hpp"
#include "SelectedColorerSystem.hpp"
#include "ClonerSystem.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "ScriptWorld.hpp"
#include "InputMapperSystem.hpp"
#include "CloneVariable.hpp"
#include "VelocitySystem.hpp"
#include "EditorContext.hpp"
#include "AssetManager.hpp"
#include "SwitchSystem.hpp"
#include "TouchSwitchSystem.hpp"
#include "SwitchEnablerSystem.hpp"
#include "SlicedQuadMeshSystem.hpp"

#include "TriggerSystem.hpp"
#include "TriggerTouchSystem.hpp"
#include "SceneManagerSystem.hpp"

#include "LineRendererSystem.hpp"

#include "EditorProxyComponent.hpp"
#include "Camera.hpp"

#include "EditorCameraSelection.hpp"
#include "DistanceScalerSystem.hpp"

using namespace Pocket;

void SystemHelper::AddGameSystems(Pocket::GameObject &world) {
    world.CreateSystem<RenderSystem>();
    world.CreateSystem<TransformHierarchy>();
    world.CreateSystem<TouchSystem>()->TouchDepth = 0;
    world.CreateSystem<ClonerSystem>();
    world.CreateSystem<InputMapperSystem>();
    world.CreateSystem<VelocitySystem>();
    world.CreateSystem<Gui>();
    world.CreateSystem<SwitchSystem>();
    world.CreateSystem<SwitchEnablerSystem>();
    world.CreateSystem<TouchSwitchSystem>();
    world.CreateSystem<SlicedQuadMeshSystem>();
    world.CreateSystem<TriggerSystem>();
    world.CreateSystem<TriggerTouchSystem>();
    world.CreateSystem<SceneManagerSystem>();
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
}

bool SystemHelper::IsComponentEditorSpecific(int componentType) {
    if (componentType == GameIdHelper::GetComponentID<EditorObject>()) return true;
    if (componentType == GameIdHelper::GetComponentID<EditorProxyComponent<Camera>>()) return true;
    return false;
}
