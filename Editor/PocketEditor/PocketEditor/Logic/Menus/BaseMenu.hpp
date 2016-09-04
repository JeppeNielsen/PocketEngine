//
//  BaseMenu.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "EditorContext.hpp"
#include "AppMenu.hpp"
#include "EngineContext.hpp"

class BaseMenu {
public:
    virtual ~BaseMenu();
    void Initialize(EngineContext* engineContext, EditorContext* context);
    void Create();
    
protected:
    EngineContext* engineContext;
    EditorContext* context;
    AppMenu* menu;
    
    virtual std::string Name() = 0;
    virtual void OnInitialize();
    virtual void OnCreate();
};