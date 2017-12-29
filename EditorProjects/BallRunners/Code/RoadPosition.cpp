#include "RoadPosition.hpp"

void RoadPositionSystem::ObjectAdded(GameObject* object) {
    //object->GetComponent<RoadPosition>()->Position.Changed.Bind(this, &RoadPositionSystem::PositionChanged, object);
    //PositionChanged(object);
}

void RoadPositionSystem::ObjectRemoved(GameObject* object) {
    //object->GetComponent<RoadPosition>()->Position.Changed.Unbind(this, &RoadPositionSystem::PositionChanged, object);
}

void RoadPositionSystem::Update(float dt) {
    for(auto o : Objects()) {
        PositionChanged(o);
    }
}

void RoadPositionSystem::PositionChanged(GameObject* object) {
    auto road = object->GetComponent<Road>();
    auto roadPosition = object->GetComponent<RoadPosition>();
    auto transform = object->GetComponent<Transform>();

    float width;
    Matrix4x4 world = road->GetWorld(roadPosition->Position(), width);

    transform->Position = world.TransformPosition(0);

    Quaternion rot;
    rot.FromRotationMatrix(world);
    
    transform->Rotation = rot;
}
