//
//  TouchSystem.hpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/21/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Touchable.hpp"
#include "InputManager.hpp"
#include "Orderable.hpp"
#include <set>

namespace Pocket {
    class OctreeSystem;
    
    class TouchSystem : public GameSystem {
    public:
        TouchSystem();
        ~TouchSystem();
        
        Property<TouchSystem*, InputManager*> Input;
        
        void Initialize();
        void AddedToWorld(GameWorld& world);
        
        OctreeSystem& Octree();
        
    protected:
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        
    private:
        
        void InputManagerChanged(Property<TouchSystem*, InputManager*>::EventData e);
        
        void TouchDown(TouchEvent e);
        void TouchUp(TouchEvent e);
        
        class CameraSystem : public GameSystem {
        public:
            void Initialize();
        };
        
        CameraSystem* cameraSystem;
        OctreeSystem* octree;
        
        ObjectCollection touchableList;
        
        class TouchableObject {
        public:
            TouchableObject(GameObject* object);
            ~TouchableObject();
            
            Transform* transform;
            Mesh* mesh;
            Touchable* touchable;
            Orderable* orderable;
            int clip;
        };
        
        typedef std::vector<TouchableObject*> Clippers;
        
        Clippers clippers;
        
        static bool SortClippers(const TouchableObject* a, const TouchableObject* b);
        
        TouchableObject* FindClipper(TouchableObject* fromThis);
        
        
        struct Intersection {
            GameObject* object;
            float distanceToPick;
            float u, v;
            size_t triIndex;
            Vector3 normal;
            TouchableObject* touchable;
            Ray localRay;
        };
        
        typedef std::vector<Intersection> Intersections;
        Intersections intersections;
        
        static bool SortIntersections(const Intersection& a, const Intersection& b);
        
        typedef std::vector<TouchData> Touched;
        
        Touched touches[12];
        
        Touched downs;
        Touched clicks;
        Touched ups;
        
        void FindTouchedObjects(Touched& list, const TouchEvent& e);
        void FindTouchedObjectsFromCamera(GameObject* cameraObject, Touched& list, const TouchEvent& e);
        void AddToTouchList(Touched& from, Touched& to);
        bool IsTouchInList(const TouchData& touchData, const Touched& list);
        
        typedef std::set<Touchable*> CancelledTouchables;
        CancelledTouchables cancelledTouchables;
        
        void TouchableCancelled(Touchable* touchable);
    };
}
