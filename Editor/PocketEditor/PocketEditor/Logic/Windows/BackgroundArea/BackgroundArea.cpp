//
//  BackgroundArea.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "BackgroundArea.hpp"

std::string BackgroundArea::Name() { return "Editor"; }

void BackgroundArea::OnInitialize() {
    context->EngineContext().ScreenSize.Changed.Bind(this, &BackgroundArea::ScreenSizeChanged);
}

void BackgroundArea::OnCreate() {
    window->RemoveComponent<Renderable>();
    window->AddComponent<PanelArea>();
    ScreenSizeChanged();
}

bool BackgroundArea::CreateBar() { return false; }

void BackgroundArea::ScreenSizeChanged() {
    Vector2 size = context->EngineContext().ScreenSize;
    window->GetComponent<Sizeable>()->Size = size;
    window->GetComponent<Transform>()->Position = 0;
}
