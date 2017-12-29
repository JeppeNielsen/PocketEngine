#include "RoadOrigin.hpp"

void RoadOriginSystem::ObjectAdded(GameObject* object) {

}

void RoadOriginSystem::ObjectRemoved(GameObject* object) {

}

void RoadOriginSystem::Update(float dt) {
    for(auto o : Objects()) {
        o->GetComponent<Road>()->Origin = o->GetComponent<RoadPosition>()->Position;
    }
}
