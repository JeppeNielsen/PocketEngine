//
//  ProjectWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "SceneTab.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"
#include "FileSystemListenerSystem.hpp"
#include "FileHelper.hpp"

std::string SceneTab::Name() { return "SceneTab"; }

void SceneTab::OnInitialize() {
    context->EngineContext().ScreenSize.Changed.Bind(this, &SceneTab::ScreenSizeChanged);
}

void SceneTab::OnCreate() {
    //window->GetComponent<Colorable>()->Color = Colour(1.0f,1.0f,1.0f,0.5f);
    
    renderArea = context->Gui().CreateLayoutControl(window, "TextBox", 25, 2000, 2000);
    tabArea = context->Gui().CreateLayoutControl(window, "Box", 25, {25,2000}, {25,2000}, Layouter::LayoutMode::Horizontal);
    
    ScreenSizeChanged();
}

bool SceneTab::CreateBar() {
    return true;
}

void SceneTab::ScreenSizeChanged() {
//    Vector2 size = context->EngineContext().ScreenSize;
//    window->GetComponent<Sizeable>()->Size = {size.x-200, 30};
//    window->GetComponent<Transform>()->Position = {200,size.y - window->GetComponent<Sizeable>()->Size().y - 30 };
}

void SceneTab::Clicked(TouchData d, OpenWorld* clickedWorld) {
    context->Project().Worlds.ActiveWorld = clickedWorld;
}

void SceneTab::ActiveWorldChanged(OpenWorld *old, OpenWorld *current) {

    if (old) {
        old->IsPlaying.Changed.Unbind(this, &SceneTab::IsPlayingChanged, current);
        if (old->GetRunningWorld()) {
            old->GetRunningWorld()->World().LayersChanged.Unbind(this, &SceneTab::LayersChanged, old);
            old->GetRunningWorld()->ActiveScene.Changed.Unbind(this, &SceneTab::ActiveSceneChanged, old->GetRunningWorld());
        }
        ClearTabs();
    }
    
    if (current) {
        current->IsPlaying.Changed.Bind(this, &SceneTab::IsPlayingChanged, current);
        if (current->GetRunningWorld()) {
            current->GetRunningWorld()->World().LayersChanged.Bind(this, &SceneTab::LayersChanged, current);
            current->GetRunningWorld()->ActiveScene.Changed.Bind(this, &SceneTab::ActiveSceneChanged, current->GetRunningWorld());
        }
        UpdateWorld(current);
    }
}

void SceneTab::IsPlayingChanged(OpenWorld* world) {
    if (world->IsPlaying) {
        if (world->GetRunningWorld()) {
            world->GetRunningWorld()->World().LayersChanged.Bind(this, &SceneTab::LayersChanged, world);
            world->GetRunningWorld()->ActiveScene.Changed.Bind(this, &SceneTab::ActiveSceneChanged, world->GetRunningWorld());
        }
    } else {
        if (world->GetRunningWorld()) {
            world->GetRunningWorld()->World().LayersChanged.Unbind(this, &SceneTab::LayersChanged, world);
            world->GetRunningWorld()->ActiveScene.Changed.Unbind(this, &SceneTab::ActiveSceneChanged, world->GetRunningWorld());
        }
    }
    UpdateWorld(world);
}

void SceneTab::LayersChanged(OpenWorld* world) {
    UpdateWorld(world);
}

void SceneTab::UpdateWorld(OpenWorld *world) {
    ClearTabs();
    if (!world->IsPlaying) return;
    auto& r = world->GetRunningWorld()->World();
    
    auto& layers = r.GetSceneLayers();
    for(auto& layer : layers) {
        AddTab(world, layer.second);
    }
    
    /*
    for(auto r : r.Roots()) {
        AddTab(world, r);
    }
    */
    ActiveSceneChanged(world->GetRunningWorld());
}

void SceneTab::AddTab(OpenWorld* world, GameObject* scene) {
    Gui& gui = context->Gui();
    GameObject* button = gui.CreateLayoutControl(tabArea, "Box", 25, {200,25}, {200,25});
    GameObject* label = gui.CreateLabel(button, 0, {200,30}, 0, FileHelper::GetFileNameFromPath(scene->TryGetRootPath()), 12);
    label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
    label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    label->GetComponent<Colorable>()->Color = Colour::Black();
    label->ReplaceComponent<Sizeable>(button);
    
    button->GetComponent<Touchable>()->Click.Bind([world, scene, this] (TouchData d) {
        world->GetRunningWorld()->ActiveScene = scene;
    });
    
    {
        GameObject* closeButton = gui.CreateControl(button, "Box", {200-25, 5}, {20,20});
        closeButton->GetComponent<Touchable>()->Click.Bind([world, this] (TouchData d) {
            world->GetRunningWorld()->ActiveScene = 0;
        });
        
        GameObject* label = gui.CreateLabel(closeButton, 0, {20,20}, 0, "X", 12);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
    }
    
    openedTabs[scene] = button;
}

void SceneTab::AlignTabs() {
//    Vector2 pos{0,0};
//    for(auto t : openedTabs) {
//        t.second->GetComponent<Transform>()->Position = pos;
//        pos += { t.second->GetComponent<Sizeable>()->Size().x, 0};
//    }
}

void SceneTab::ClearTabs() {
    for(auto t : openedTabs) {
        t.second->Remove();
    }
    openedTabs.clear();
}

void SceneTab::ActiveSceneChanged(RunningWorld *world) {
    for(auto t : openedTabs) {
        bool active = t.first == world->ActiveScene;
        t.second->GetComponent<Colorable>()->Color =
            active ?
        Colour(1.0f,1.0f,1.0f, 1.0f) :
        Colour(0.6f,0.6f,0.6f, 1.0f);
    }
}
