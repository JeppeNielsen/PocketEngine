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
    
    playButton = gui.CreateLabelControl(window, "TextBox", 0, {100,30},0, "Play", 20);
    stopButton = gui.CreateLabelControl(window, "TextBox", {100,0}, {100,30}, 0, "Stop", 20);
    
    playButton->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    stopButton->Children()[0]->GetComponent<Colorable>()->Color = Colour::Black();
    
    playButton->GetComponent<Touchable>()->Click.Bind(this, &PlayButtons::PlayClicked);
    stopButton->GetComponent<Touchable>()->Click.Bind(this, &PlayButtons::StopClicked);
    
    playButton->Enabled = false;
    stopButton->Enabled = false;
    
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
        UpdateButtons(current);
    } else {
        playButton->Enabled = false;
        stopButton->Enabled = false;
    }
}

void PlayButtons::UpdateButtons(OpenWorld *openWorld) {
    playButton->Enabled = !openWorld->IsPlaying();
    stopButton->Enabled = !playButton->Enabled();
}

void PlayButtons::PlayClicked(Pocket::TouchData d) {
    if (!currentWorld) return;
    context->preActions.emplace_back([this] {
        currentWorld->Play();
        UpdateButtons(currentWorld);
    });
}

void PlayButtons::StopClicked(Pocket::TouchData d) {
    if (!currentWorld) return;
    context->preActions.emplace_back([this] {
        currentWorld->Stop();
        UpdateButtons(currentWorld);
    });
}
