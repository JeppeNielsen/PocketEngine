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
#include "Orderable.hpp"
#include <set>

namespace Pocket {

    class TouchSystem : public GameSystem<Transform, Mesh, Touchable> {
    private:
    
        using OctreeSystem = OctreeSystem<Touchable>;
        
        struct CameraSystem : GameSystem<Transform, Camera> {};
        struct OrderableSystem : GameSystem<Orderable> {};
        
    public:
        
    
        TouchSystem();
        ~TouchSystem();
        
        void Initialize() override;
        void Destroy() override;
        OctreeSystem& Octree();
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
        void Update(float dt) override;
        
        void EnqueueDown(GameObject* touchObject, TouchData touchData);
        
        int TouchDepth;
        
    private:
                
        void TouchDown(Pocket::TouchEvent e);
        void TouchUp(Pocket::TouchEvent e);
        
        CameraSystem* cameras;
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
        
        Touched equeuedDowns;
        
    public:
        void FindTouchedObjects(Touched& list, const TouchEvent& e, bool forceClickThrough = false);
        void SetCameras(CameraSystem* cameraSystem);
        CameraSystem* GetCameras();
    private:
        
        void FindTouchedObjectsFromCamera(GameObject* cameraObject, Touched& list, const TouchEvent& e, bool forceClickThrough);
        using CancelledTouchables = std::set<Touchable*>;
        CancelledTouchables cancelledTouchables;
        
        static bool SortIntersections(const Intersection &a, const Intersection &b);
        void AddToTouchList(Touched &from, Touched &to);
        bool IsTouchInList(const Pocket::TouchData &touchData, const Touched &list);
        void TouchableCancelled(Pocket::Touchable *touchable);
        bool IsTouchValid(const TouchData& touchData);
    };
}
