#pragma once
#include "GameSystem.hpp"
#include "Timeline.hpp"
#include "Vector3.hpp"
#include "BezierCurve3.hpp"
#include "Quaternion.hpp"
#include <vector>
#include <string>

using namespace Pocket;

struct Road { 

    BezierCurve3 positions;
    Timeline<float> widths;
    Timeline<float> rotations;

    Property<float> Origin;
    
    Matrix4x4 GetWorld(float pos, float& width) {

        Vector3 originPosition = positions.Evaluate(Origin());

        float rotation = 0;
        rotations.Evaluate(pos, [&rotation] (float t, float& a, float& b) {
            rotation = a + (b-a) * t;
        });

        width = 1;
        widths.Evaluate(pos, [&width] (float t, float& a, float& b) {
            width = a + (b-a) * t;
        });

        Quaternion rot;
        rot.FromEuler({0,0,rotation});
        
        Matrix4x4 world = positions.GetWorld(pos, {0,1,0});//mat.TransformVector({0,1,0}));
        world.SetTranslation(world.Translation() - originPosition);
        world *= Matrix4x4::CreateTransform(0, {1,1,1}, rot);
        
        return world;
    }
};

struct RoadSystem : GameSystem<Road> {
   void ObjectAdded(GameObject* object) override;
   void ObjectRemoved(GameObject* object) override;
   void Update(float dt) override;
};

