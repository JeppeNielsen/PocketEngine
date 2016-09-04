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

namespace Pocket {
    class AppMenu {
    public:
        AppMenu();
        ~AppMenu();
        
        Event<> Clicked;
        AppMenu& AddChild(const std::string& text, const std::string& shortcut="");
    private:
    
        AppMenu(AppMenu* parent, const std::string& text, const std::string& shortcut="");
    
        std::string text;
        std::vector<AppMenu*> children;
        AppMenu* parent;
        
        void Selected();
    };
}