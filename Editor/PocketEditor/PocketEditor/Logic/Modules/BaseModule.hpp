//
//  BaseModule.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "EditorContext.hpp"
#include "AppMenu.hpp"
#include "EngineContext.hpp"

class BaseModule {
public:
    virtual ~BaseModule();
    void Initialize(EngineContext* engineContext, EditorContext* context);
    void Create();
    
protected:
    EngineContext* engineContext;
    EditorContext* context;

    virtual void OnInitialize();
    virtual void OnCreate();
};
