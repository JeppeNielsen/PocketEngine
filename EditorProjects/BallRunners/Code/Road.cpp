#include "Road.hpp"

void RoadSystem::ObjectAdded(GameObject* object) {
    auto road = object->GetComponent<Road>();

    // road->nodes.AddNode(0, {0, 2, Vector3(0,0,0)});
    // road->nodes.AddNode(5, {5, 4, Vector3(0,0,10)});
    // road->nodes.AddNode(10,{10, 2, Vector3(0,0,-10)});

        // road->positions.nodes.Clear();
        // road->widths.Clear();

        // road->positions.Add(0,0, 0, {0,0,5});
        // road->positions.Add(20,{0,10,20}, {0,10,15}, {0,10,25});
        // road->positions.Add(40,{5,0,40});

        // road->widths.AddNode(0, 5);
        //widths.AddNode(20, 5);
        //widths.AddNode(40, 5);

}

void RoadSystem::ObjectRemoved(GameObject* object) {

}

void RoadSystem::Update(float dt) {
   
}
