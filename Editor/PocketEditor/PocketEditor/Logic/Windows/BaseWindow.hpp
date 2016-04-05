//
//  BaseWindow.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "EditorContext.hpp"

class BaseWindow {
public:
    virtual ~BaseWindow();
    void Initialize(EditorContext* context);
    virtual void Create();
    
protected:
    EditorContext* context;
    GameObject* window;
    
    virtual std::string Name() = 0;
    virtual void OnInitialize();
    virtual void OnCreate();
};