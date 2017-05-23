//
//  AppMenu.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/07/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Event.hpp"
#include <string>
#include "Vector2.hpp"

namespace Pocket {
    class AppMenu {
    public:
        AppMenu();
        ~AppMenu();
        
        Event<> Clicked;
        AppMenu& AddChild(const std::string& text, const std::string& shortcut="");
        
        void ShowPopup(const Vector2& location);
    
        void InitializeMainMenu();
        void InitializePopUp();
        void Clear();
    
    private:
        void CreateAppMenu(AppMenu* parent, const std::string& text, const std::string& shortcut="");

        std::string text;
        std::vector<AppMenu*> children;
        AppMenu* parent;
    };
}
