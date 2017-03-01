//
//  TriggerSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Trigger.hpp"

namespace Pocket {
    class TriggerSystem : public GameSystem<Trigger> {
    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
    private:
        void CloneSourceChanged(GameObject* object);
        
        GameObject* variablesRoot;
        
    };
}
