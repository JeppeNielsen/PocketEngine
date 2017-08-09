//
//  LayoutSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/12/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Layouter.hpp"
#include "Transform.hpp"
#include "Sizeable.hpp"
#include <set>

namespace Pocket {

    class LayoutSystem : public GameSystem<Layouter, Transform, Sizeable> {

    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        
    public:
        void MinChanged(Layouter* layouter);
        void MaxChanged(Layouter* layouter);
        void DesiredChanged(Layouter* layouter);
        void GlobalMinDirty(GameObject* object);
        void GlobalMaxDirty(GameObject* object);
        void GlobalDesiredDirty(GameObject* object);
        void ParentChanged(GameObject* object);
        void SizeChanged(GameObject* object);
        static Vector2 DoLayout(Layouter* layouter, GameObject* object,
            const std::function<Vector2(Layouter* layouter)>& localGetter,
            const std::function<Vector2(Layouter* layouter)>& globalGetter
        );
        
        void CalcLayout(GameObject* object);
        void TryInvokeChangesToParent(GameObject* object);
    private:
        std::set<GameObject*> dirtyObjects;
        
    };
}
