//
//  ProjectWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "WorldTab.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"
#include "FileSystemListenerSystem.hpp"
#include "GameWorldViewport.hpp"
#include "FileHelper.hpp"

std::string WorldTab::Name() { return "WorldTab"; }

void WorldTab::OnInitialize() {
    context->Project().Worlds.WorldLoaded.Bind(this, &WorldTab::WorldLoaded);
    context->Project().Worlds.WorldClosed.Bind(this, &WorldTab::WorldClosed);
}

void WorldTab::OnCreate() {
    window->Hierarchy().Parent()->GetComponent<Transform>()->Position = {300,100};
    window->Hierarchy().Parent()->RemoveComponent<Renderable>();
    window->Hierarchy().Parent()->GetComponent<Touchable>()->ClickThrough = true;

    renderArea = context->Gui().CreatePivot(window);
   // renderArea->AddComponent<GameWorldViewport>()->World = &context->World();
    renderArea->AddComponent<GameWorldViewport>()->RenderTextureSize = {2880, 1800};
    
    renderArea->AddComponent<Touchable>();
    renderArea->AddComponent<TextureComponent>();
    renderArea->AddComponent<Sizeable>();
    renderArea->AddComponent<Mesh>();
    renderArea->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    context->Gui().AddLayouter(renderArea, 25, 2000, 2000);
    
    CreatePlayButtons();
    
    tabArea = context->Gui().CreateLayoutControl(window, "TextBox", 25, {2000,25}, {2000,25}, Layouter::LayoutMode::Horizontal);
}

bool WorldTab::CreateBar() {
    return true;
}

void WorldTab::Clicked(TouchData d, OpenWorld* clickedWorld) {
    context->Project().Worlds.ActiveWorld = clickedWorld;
}

void WorldTab::ActiveWorldChanged(OpenWorld *old, OpenWorld *current) {
    for(auto t : openedTabs) {
        bool active = t.first == current;
        t.second->GetComponent<Colorable>()->Color =
            active ?
        Colour(1.0f,1.0f,1.0f, 1.0f) :
        Colour(0.6f,0.6f,0.6f, 1.0f);
    }
    UpdatePlayButtons(current);
    
    if (old) {
        old->IsPlaying.Changed.Unbind(this, &WorldTab::IsPlayingChanged, current);
        if (old->GetRunningWorld()) {
            old->GetRunningWorld()->World().SceneCreated.Unbind(this, &WorldTab::RootCreated, old);
            old->GetRunningWorld()->World().SceneRemoved.Unbind(this, &WorldTab::RootRemoved, old);
            old->GetRunningWorld()->ActiveScene.Changed.Unbind(this, &WorldTab::ActiveSceneChanged, old->GetRunningWorld());
        }
        ClearTabs();
    }
    
    if (current) {
        renderArea->GetComponent<GameWorldViewport>()->World = current->World();
        current->IsPlaying.Changed.Bind(this, &WorldTab::IsPlayingChanged, current);
        if (current->GetRunningWorld()) {
            current->GetRunningWorld()->World().SceneCreated.Bind(this, &WorldTab::RootCreated, current);
            current->GetRunningWorld()->World().SceneRemoved.Bind(this, &WorldTab::RootRemoved, current);
            current->GetRunningWorld()->ActiveScene.Changed.Bind(this, &WorldTab::ActiveSceneChanged, current->GetRunningWorld());
        }
        UpdateWorld(current);
    } else {
        renderArea->GetComponent<GameWorldViewport>()->World = nullptr;
    }
}

void WorldTab::WorldLoaded(OpenWorld *world) {
    Gui& gui = context->Gui();
    GameObject* button = gui.CreateLayoutControl(tabArea, "Box", {200,25}, {200,25}, {200,25}, Layouter::LayoutMode::Horizontal);
    GameObject* label = gui.CreateLabel(button, 0, {200,30}, 0, world->Filename, 12);
    label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
    label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    label->GetComponent<Colorable>()->Color = Colour::Black();
    
    gui.AddLayouter(label, 25, {175,25}, {175,25});
    
    button->GetComponent<Touchable>()->Click.Bind(this, &WorldTab::Clicked, world);
    
    {
        GameObject* closeButton = gui.CreateLayoutControl(button, "Box", 25,25,25);
        closeButton->GetComponent<Touchable>()->Click.Bind([world, this] (TouchData d) {
            context->Project().Worlds.CloseWorld(world);
        });
        
        GameObject* label = gui.CreateLabel(closeButton, 0, {20,20}, 0, "X", 12);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
        label->ReplaceComponent<Sizeable>(closeButton);
    }
    
    openedTabs[world] = button;
}

void WorldTab::WorldClosed(OpenWorld *world) {
    openedTabs[world]->Remove();
    openedTabs.erase(openedTabs.find(world));
}

void WorldTab::CreatePlayButtons() {
    Gui& gui = context->Gui();
    
    editModePivot = gui.CreatePivot(window);
    editModePivot->AddComponent<Sizeable>();
    gui.AddLayouter(editModePivot, 30, {2000,30}, {2000,30}, Layouter::LayoutMode::Horizontal);
    {
        GameObject* testButton = gui.CreateLabelControl(editModePivot, "TextBox", 0, {100,30},0, "Test", 20);
        gui.AddLayouter(testButton, 30, {2000,30}, {2000,30});
        testButton->Hierarchy().Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
        testButton->GetComponent<Touchable>()->Click.Bind([this](TouchData d) {
            if (!currentWorld) return;
            context->preActions.emplace_back([this] {
                currentWorld->Play();
                UpdatePlayButtons(currentWorld);
            });
        });
    }

    gameModePivot = gui.CreatePivot(window);
    gui.AddLayouter(gameModePivot, 30, {2000,30}, {2000,30}, Layouter::LayoutMode::Vertical);
    
    
    GameObject* buttons = gui.CreatePivot(gameModePivot);
    gui.AddLayouter(buttons, 30, {2000,30}, {2000,30}, Layouter::LayoutMode::Horizontal);
    {
        GameObject* stopButton = gui.CreateLabelControl(buttons, "TextBox", {100,0}, {100,30}, 0, "Stop", 20);
        gui.AddLayouter(stopButton, 30, {2000,30}, {2000,30});
        
        stopButton->Hierarchy().Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
        stopButton->GetComponent<Touchable>()->Click.Bind([this] (TouchData d) {
            if (!currentWorld) return;
            context->preActions.emplace_back([this] {
                currentWorld->Stop();
                UpdatePlayButtons(currentWorld);
            });
        });
    
        GameObject* pauseButton = gui.CreateLabelControl(buttons, "TextBox", {0,0}, {100,30}, 0, "Pause", 20);
        gui.AddLayouter(pauseButton, 30, {2000,30}, {2000,30});
        
        pauseButton->Hierarchy().Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
        pauseButton->GetComponent<Touchable>()->Click.Bind([this](TouchData d) {
            if (!currentWorld) return;
            currentWorld->IsPaused = !currentWorld->IsPaused();
        });
    }
    
    rootPanel = gui.CreatePivot(gameModePivot);
    gui.AddLayouter(rootPanel, 30, {2000,30}, {2000,30}, Layouter::LayoutMode::Horizontal);
    {
        
    }
    
    
    editModePivot->Hierarchy().Enabled = false;
    gameModePivot->Hierarchy().Enabled = false;
}

void WorldTab::UpdatePlayButtons(OpenWorld *openWorld) {
    if (openWorld) {
        editModePivot->Hierarchy().Enabled = !openWorld->IsPlaying();
        gameModePivot->Hierarchy().Enabled = openWorld->IsPlaying();
    } else {
        editModePivot->Hierarchy().Enabled = false;
        gameModePivot->Hierarchy().Enabled = false;
    }
    
   /* if (openWorld) {
        renderArea->GetComponent<GameWorldViewport>()->World = !openWorld->IsPlaying() ? &context->World() : &openWorld->GetRunningWorld()->World();
    } else {
        renderArea->GetComponent<GameWorldViewport>()->World = &context->World();
    }
    */
}

void WorldTab::IsPlayingChanged(OpenWorld* world) {
    if (world->IsPlaying) {
        if (world->GetRunningWorld()) {
            world->GetRunningWorld()->World().SceneCreated.Bind(this, &WorldTab::RootCreated, world);
            world->GetRunningWorld()->World().SceneRemoved.Bind(this, &WorldTab::RootRemoved, world);
            world->GetRunningWorld()->ActiveScene.Changed.Bind(this, &WorldTab::ActiveSceneChanged, world->GetRunningWorld());
        }
    } else {
        if (world->GetRunningWorld()) {
            world->GetRunningWorld()->World().SceneCreated.Unbind(this, &WorldTab::RootCreated, world);
            world->GetRunningWorld()->World().SceneRemoved.Unbind(this, &WorldTab::RootRemoved, world);
            world->GetRunningWorld()->ActiveScene.Changed.Unbind(this, &WorldTab::ActiveSceneChanged, world->GetRunningWorld());
        }
    }
    UpdateWorld(world);
}

void WorldTab::UpdateWorld(OpenWorld *world) {
    ClearTabs();
    if (!world->IsPlaying) return;
    auto& runningWorld = world->GetRunningWorld()->World();
    
    for (auto r : runningWorld.Scenes()) {
        if (r!=world->GetRunningWorld()->EditorRoot()) {
            AddTab(world, r);
        }
    }
    ActiveSceneChanged(world->GetRunningWorld());
}

void WorldTab::AddTab(OpenWorld* world, GameObject* scene) {

    Gui& gui = context->Gui();

    GameObject* button = gui.CreateLayoutControl(rootPanel, "Box", {200,25}, {200,25}, {200,25}, Layouter::LayoutMode::Horizontal);
    GameObject* label = gui.CreateLabel(button, 0, {200,30}, 0, "SCENE, FIX",12);// FileHelper::GetFileNameFromPath(scene->TryGetRootPath()), 12);
    label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
    label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    label->GetComponent<Colorable>()->Color = Colour::Black();

    gui.AddLayouter(label, 25, {175,25}, {175,25});

    button->GetComponent<Touchable>()->Click.Bind([world, scene] (TouchData d){
        world->GetRunningWorld()->ActiveScene = scene;
    });

    {
        GameObject* closeButton = gui.CreateLayoutControl(button, "Box", 25,25,25);
        closeButton->GetComponent<Touchable>()->Click.Bind([world, this] (TouchData d) {
            world->GetRunningWorld()->ActiveScene = nullptr;
        });
        
        GameObject* label = gui.CreateLabel(closeButton, 0, {20,20}, 0, "X", 12);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
        label->ReplaceComponent<Sizeable>(closeButton);
    }
}

void WorldTab::ClearTabs() {
    for (auto o : rootPanel->Hierarchy().Children()) {
        o->Remove();
    }
}

void WorldTab::ActiveSceneChanged(RunningWorld *world) {
//    for(auto t : rootPanel->Children()) {
//        bool active = t.first == world->ActiveScene;
//        t->GetComponent<Colorable>()->Color =
//            active ?
//        Colour(1.0f,1.0f,1.0f, 1.0f) :
//        Colour(0.6f,0.6f,0.6f, 1.0f);
//    }
}

void WorldTab::RootCreated(GameObject* root, OpenWorld* world) {
    UpdateWorld(world);
}

void WorldTab::RootRemoved(GameObject* root, OpenWorld* world) {
    UpdateWorld(world);
}
