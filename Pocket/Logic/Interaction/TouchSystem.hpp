//
//  TouchSystem.hpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/21/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "OctreeSystem.hpp"
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Touchable.hpp"
#include "InputManager.hpp"
#include "Orderable.hpp"
#include <set>
#include <stack>

namespace Pocket {

    class TouchSystem : public GameSystem<Transform, Mesh, Touchable> {
    private:
    
        using OctreeSystem = OctreeSystem<Touchable>;
        using ObjectCollection = typename GameSystem<Transform, Mesh, Touchable>::ObjectCollection;
        
        struct CameraSystem : GameSystem<Transform, Camera> {};
        struct OrderableSystem : GameSystem<Orderable> {};
        
    public:
        
    
        TouchSystem();
        void Initialize(GameWorld* world);
        OctreeSystem& Octree();
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);
        void Update(float dt);
        
        Property<InputManager*> Input;
        
    private:
                
        void TouchDown(Pocket::TouchEvent e);
        void TouchUp(Pocket::TouchEvent e);
        
        CameraSystem* cameraSystem;
        OctreeSystem* octree;
        
        ObjectCollection touchableList;
        
        struct TouchableObject {
            TouchableObject(GameObject* object);
            
            Transform* transform;
            Mesh* mesh;
            Touchable* touchable;
            Orderable* orderable;
            int clip;
        };
        
        using Clippers = std::vector<TouchableObject*>;
        
        Clippers clippers;
        
        static bool SortClippers(const TouchableObject *a, const TouchableObject *b);
        
        TouchableObject* FindClipper(TouchableObject *fromThis);
        
        struct Intersection {
            GameObject* object;
            float distanceToPick;
            float u, v;
            size_t triIndex;
            Vector3 normal;
            TouchableObject* touchable;
            Ray localRay;
        };
        
        using Intersections = std::vector<Intersection>;
        Intersections intersections;
        
        typedef std::vector<TouchData> Touched;
        
        Touched touches[12];
        
        Touched downs;
        Touched clicks;
        Touched ups;
        
    public:
        void FindTouchedObjects(Touched& list, const TouchEvent& e, bool forceClickThrough = false);
    private:
        
        void FindTouchedObjectsFromCamera(GameObject* cameraObject, Touched& list, const TouchEvent& e, bool forceClickThrough);
        using CancelledTouchables = std::set<Touchable*>;
        CancelledTouchables cancelledTouchables;
        
        static bool SortIntersections(const Intersection &a, const Intersection &b);
        void AddToTouchList(Touched &from, Touched &to);
        bool IsTouchInList(const Pocket::TouchData &touchData, const Touched &list);
        void TouchableCancelled(Pocket::Touchable *touchable);
    };
}
