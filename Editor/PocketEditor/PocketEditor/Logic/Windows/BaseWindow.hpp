//
//  BaseWindow.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "EditorContext.hpp"
#include "OpenWorld.hpp"
#include <string>

class BaseWindow {
public:
    virtual ~BaseWindow();
    void Initialize(EditorContext* context);
    virtual void Create();
    
private:
    void ActiveWorldChangedInternal();

protected:
    EditorContext* context;
    GameObject* window;
    OpenWorld* currentWorld;
    
    virtual std::string Name() = 0;
    virtual void OnInitialize();
    virtual void OnCreate();
    virtual void ActiveWorldChanged(OpenWorld* old, OpenWorld* current);
    virtual bool CreateBar();
    virtual void PostCompile();
    
    GameObject* CreateButton(GameObject* parent, const Vector2& position, const Vector2& size, const std::string& text);
};