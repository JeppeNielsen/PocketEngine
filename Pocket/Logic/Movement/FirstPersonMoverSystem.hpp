//
//  FirstPersonMoverSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/28/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"
#include "Property.hpp"
#include "InputManager.hpp"
#include "Transform.hpp"
#include "FirstPersonMover.hpp"
#include "DraggableSystem.hpp"
#include <map>

namespace Pocket {
    class DraggableSystem;
    class FirstPersonMoverSystem : public GameSystem<Transform, FirstPersonMover>  {
    public:
        FirstPersonMoverSystem();
        
        void Initialize();
        void Destroy();
        void Update(float dt);
        
        bool FlipControls;
        
private:
        
        void TouchDown(TouchEvent e);
        void TouchUp(TouchEvent e);
        
        void UpdateMovement(float dt, int touchIndex);
        bool UpdateRotation(float dt, int touchIndex);
        
        typedef std::map<int, Vector2> Touches;
        Touches touches;

        DraggableSystem* draggableSystem;
        bool isDraggableSystemChecked;
    };
}