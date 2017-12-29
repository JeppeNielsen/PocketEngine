#include "RoadGenerator.hpp"
#include "MathHelper.hpp"

void RoadGeneratorSystem::ObjectAdded(GameObject* object) {

}

void RoadGeneratorSystem::ObjectRemoved(GameObject* object) {

}

void RoadGeneratorSystem::Update(float dt) {
    for(auto o : Objects()) {
        auto road = o->GetComponent<Road>();
        auto roadGenerator = o->GetComponent<RoadGenerator>();

        if (roadGenerator->time>roadGenerator->frequency) {

            float length = MathHelper::Random(10,30);
            
            float prevTime = road->positions.nodes.keys.empty() ? 0.0f : road->positions.nodes.keys[road->positions.nodes.keys.size()-1];

            auto prev = road->positions.nodes.values.empty() ? Vector3(0,0,0) : road->positions.nodes.values[road->positions.nodes.values.size()-1].value;

            auto pos = prev+ 
                Vector3(MathHelper::Random(-5,5),MathHelper::Random(-8,8),length);
            
            auto p0 = Vector3::Lerp(prev, pos, 0.75f);
            auto p1 = Vector3::Lerp(prev, pos, 1.25f);
            

            road->positions.Add(prevTime + length,pos,p0,p1);
            roadGenerator->time = 0;
        }

        roadGenerator->time += dt;
    }
}
