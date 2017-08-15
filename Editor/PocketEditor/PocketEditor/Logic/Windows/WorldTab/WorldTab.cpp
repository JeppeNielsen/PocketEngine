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

std::string WorldTab::Name() { return "WorldTab"; }

void WorldTab::OnInitialize() {
    context->Project().Worlds.WorldLoaded.Bind(this, &WorldTab::WorldLoaded);
    context->Project().Worlds.WorldClosed.Bind(this, &WorldTab::WorldClosed);
}

void WorldTab::OnCreate() {
    window->Parent()->GetComponent<Transform>()->Position = {300,100};
    window->Parent()->RemoveComponent<Renderable>();
    window->Parent()->GetComponent<Touchable>()->ClickThrough = true;

    renderArea = context->Gui().CreatePivot(window);
    renderArea->AddComponent<GameWorldViewport>()->World = &context->World();
    renderArea->GetComponent<GameWorldViewport>()->RenderSize = {2048, 1152 };
    
    renderArea->AddComponent<Touchable>();
    renderArea->AddComponent<TextureComponent>();
    renderArea->AddComponent<Sizeable>();
    renderArea->AddComponent<Mesh>();
    renderArea->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    context->Gui().AddLayouter(renderArea, 25, 2000, 2000);
    
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

