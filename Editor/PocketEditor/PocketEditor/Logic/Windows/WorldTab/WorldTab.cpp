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

std::string WorldTab::Name() { return "WorldTab"; }

void WorldTab::OnInitialize() {
    context->Project().Worlds.WorldLoaded.Bind(this, &WorldTab::WorldLoaded);
    context->Project().Worlds.WorldClosed.Bind(this, &WorldTab::WorldClosed);
    context->EngineContext().ScreenSize.Changed.Bind(this, &WorldTab::ScreenSizeChanged);
}

void WorldTab::OnCreate() {
    window->GetComponent<Colorable>()->Color = Colour(1.0f,1.0f,1.0f,0.5f);
    ScreenSizeChanged();
}

bool WorldTab::CreateBar() {
    return false;
}

void WorldTab::ScreenSizeChanged() {
    Vector2 size = context->EngineContext().ScreenSize;
    window->GetComponent<Sizeable>()->Size = {size.x-200, 30};
    window->GetComponent<Transform>()->Position = {200,size.y - window->GetComponent<Sizeable>()->Size().y };
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
}

void WorldTab::WorldLoaded(OpenWorld *world) {
    Gui& gui = context->Gui();
    GameObject* button = gui.CreateControl(window, "Box", 0, {200,30});
    GameObject* label = gui.CreateLabel(button, 0, {200,30}, 0, world->Filename, 12);
    label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
    label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
    
    label->GetComponent<Colorable>()->Color = Colour::Black();
    
    button->GetComponent<Touchable>()->Click.Bind(this, &WorldTab::Clicked, world);
    
    {
        GameObject* closeButton = gui.CreateControl(button, "Box", {200-25, 5}, {20,20});
        closeButton->GetComponent<Touchable>()->Click.Bind([world, this] (TouchData d) {
            context->Project().Worlds.CloseWorld(world);
        });
        
        GameObject* label = gui.CreateLabel(closeButton, 0, {20,20}, 0, "X", 12);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
    }
    
    openedTabs[world] = button;
    AlignTabs();
}

void WorldTab::WorldClosed(OpenWorld *world) {
    openedTabs[world]->Remove();
    openedTabs.erase(openedTabs.find(world));
    AlignTabs();
}

void WorldTab::AlignTabs() {
    Vector2 pos{0,0};
    for(auto t : openedTabs) {
        t.second->GetComponent<Transform>()->Position = pos;
        pos += { t.second->GetComponent<Sizeable>()->Size().x, 0};
    }
}
