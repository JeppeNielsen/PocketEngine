//
//  HoverSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "OctreeSystem.hpp"
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Hoverable.hpp"
#include "Picker.hpp"

namespace Pocket {
    class HoverSystem : public GameSystem<Transform, Mesh, Hoverable> {
    private:
    
        using OctreeSystem = OctreeSystem<Hoverable>;
        
        struct CameraSystem : GameSystem<Transform, Camera> {};
        
        Picker picker;
        
    public:
        
        void Initialize() override;
        OctreeSystem& Octree();
        void ObjectAdded(GameObject* object) override;
        void ObjectRemoved(GameObject* object) override;
        void Update(float dt) override;
        static void CreateSubSystems(GameStorage& storage);
    private:
        
        CameraSystem* cameras;
        OctreeSystem* octree;
        
        using TouchList = std::vector<TouchData>;
        
        TouchList previousHovers;
        
    public:
        void SetCameras(CameraSystem* cameraSystem);
        CameraSystem* GetCameras();
        CameraSystem* GetOriginalCameras();
    };
}
