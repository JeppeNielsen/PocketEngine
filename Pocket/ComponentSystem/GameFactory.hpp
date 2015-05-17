//
//  GameFactory.hpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/21/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

namespace Pocket {
    class GameWorld;
    class GameFactory {
    private:
        GameWorld* world;
        friend class GameWorld;
    protected:
        GameFactory();
        virtual ~GameFactory();
        GameWorld* World();
        
        template<typename T>
        T* CreateSystem();
        
    public:
        virtual void Initialize() = 0;
    };
}


