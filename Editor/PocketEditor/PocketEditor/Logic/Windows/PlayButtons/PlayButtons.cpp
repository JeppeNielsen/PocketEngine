//
//  ProjectWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "PlayButtons.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"
#include "FileSystemListenerSystem.hpp"

std::string PlayButtons::Name() { return "PlayButtons"; }

void PlayButtons::OnInitialize() {
    context->EngineContext().ScreenSize.Changed.Bind(this, &PlayButtons::ScreenSizeChanged);
}

void PlayButtons::OnCreate() {
    
    Gui& gui = context->Gui();
    
    editModePivot = gui.CreatePivot(window);
    {
        GameObject* testButton = gui.CreateLabelControl(editModePivot, "TextBox", 0, {100,30},0, "Test", 20);
        testButton->Hierarchy().Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
        testButton->GetComponent<Touchable>()->Click.Bind([this](TouchData d) {
            if (!currentWorld) return;
            context->preActions.emplace_back([this] {
                currentWorld->Play();
                UpdateStates(currentWorld);
            });
        });
    }

    gameModePivot = gui.CreatePivot(window);
    {
        GameObject* stopButton = gui.CreateLabelControl(gameModePivot, "TextBox", {100,0}, {100,30}, 0, "Stop", 20);
        stopButton->Hierarchy().Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
        stopButton->GetComponent<Touchable>()->Click.Bind([this] (TouchData d) {
            if (!currentWorld) return;
            context->preActions.emplace_back([this] {
                currentWorld->Stop();
                UpdateStates(currentWorld);
            });
        });
    
        GameObject* pauseButton = gui.CreateLabelControl(gameModePivot, "TextBox", {0,0}, {100,30}, 0, "Pause", 20);
        pauseButton->Hierarchy().Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
        pauseButton->GetComponent<Touchable>()->Click.Bind([this](TouchData d) {
            if (!currentWorld) return;
            currentWorld->IsPaused = !currentWorld->IsPaused();
        });
    }
    
    editModePivot->Hierarchy().Enabled = false;
    gameModePivot->Hierarchy().Enabled = false;
    
    window->RemoveComponent<Renderable>();
    ScreenSizeChanged();
}

bool PlayButtons::CreateBar() {
    return false;
}

void PlayButtons::ScreenSizeChanged() {
    Vector2 size = context->EngineContext().ScreenSize;
    window->GetComponent<Sizeable>()->Size = {100, 30};
    window->GetComponent<Transform>()->Position = {size.x/2-window->GetComponent<Sizeable>()->Size().x/2,size.y - window->GetComponent<Sizeable>()->Size().y-30 };
}

void PlayButtons::ActiveWorldChanged(OpenWorld *old, OpenWorld *current) {
    if (current) {
        UpdateStates(current);
    } else {
        editModePivot->Hierarchy().Enabled = false;
        gameModePivot->Hierarchy().Enabled = false;
    }
}

void PlayButtons::UpdateStates(OpenWorld *openWorld) {
    editModePivot->Hierarchy().Enabled = !openWorld->IsPlaying();
    gameModePivot->Hierarchy().Enabled = openWorld->IsPlaying();
}
