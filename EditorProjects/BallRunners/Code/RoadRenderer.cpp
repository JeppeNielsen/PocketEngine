#include "RoadRenderer.hpp"
#include <iostream>
#include <cmath>

void RoadRendererSystem::Initialize() {
}

void RoadRendererSystem::ObjectAdded(GameObject* object) {
    //object->GetComponent<RoadPosition>()->Position.Changed.Bind(this, &RoadRendererSystem::PositionChanged, object);
    //PositionChanged(object);
}

void RoadRendererSystem::ObjectRemoved(GameObject* object) {
    //object->GetComponent<RoadPosition>()->Position.Changed.Unbind(this, &RoadRendererSystem::PositionChanged, object);
}

void RoadRendererSystem::Update(float dt) {
    for(auto o : Objects()) {
        PositionChanged(o);
    }
}

void RoadRendererSystem::PositionChanged(GameObject* object) {

    auto road = object->GetComponent<Road>();
    auto roadRenderer = object->GetComponent<RoadRenderer>();
    auto mesh = object->GetComponent<Mesh>();
    auto position = object->GetComponent<RoadPosition>();

    auto& vertices = mesh->GetMesh<Vertex>().vertices;
    auto& triangles = mesh->GetMesh<Vertex>().triangles;
    vertices.clear();
    triangles.clear();

    std::cout << roadRenderer->segments << std::endl;

    if (roadRenderer->segments<=0) {
        roadRenderer->segments = 32;
    }

    if (roadRenderer->Depth<=0) {
        roadRenderer->Depth = 16.0f;
    }

    float dS = roadRenderer->Depth / roadRenderer->segments;

    for(int s = 0; s<=roadRenderer->segments; s++) {
        float pos = floorf(position->Position()/dS)*dS + roadRenderer->renderOffset + s * dS;
        float width;
        auto world = road->GetWorld(pos, width);

        Vector3 left = world.TransformPosition(Vector3(-width,0,0));
        Vector3 right = world.TransformPosition(Vector3(width,0,0));
        Vector3 up = world.TransformVector(Vector3(0,1,0));
        up.Normalize();
        
        vertices.push_back({left, Vector2(0, pos * roadRenderer->vScale), Colour::White(), up});
        vertices.push_back({right, Vector2(1, pos * roadRenderer->vScale), Colour::White(), up});
    }

    for(int s = 0; s<roadRenderer->segments; s++) {
        int index = s * 2;
        
        triangles.push_back(index);
        triangles.push_back(index+1);
        triangles.push_back(index+2);
        
        triangles.push_back(index+1);
        triangles.push_back(index+3);
        triangles.push_back(index+2);
    }
}
