//
//  TerrainVertexEditor.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 07/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "TerrainVertexEditor.h"
#include "Transform.hpp"
#include "TouchSystem.hpp"
#include "DraggableSystem.hpp"
#include "ClickSelectorSystem.hpp"

void TerrainVertexEditor::AddedToWorld(Pocket::GameWorld &world) {
    touchSystem = world.CreateOrGetSystem<TouchSystem>();
    world.CreateOrGetSystem<DraggableSystem>();
    world.CreateSystem<AddVertexSystem>();
    selectedVertices = world.CreateSystem<SelectableCollection>();
    world.CreateSystem<ClickSelectorSystem>();
}

void TerrainVertexEditor::SetInput(Pocket::InputManager *input) {
    if (!touchSystem->Input()) {
        touchSystem->Input = input;
    }
    input->ButtonDown += event_handler(this, &TerrainVertexEditor::ButtonDown);
}

void TerrainVertexEditor::ObjectAdded(Pocket::GameObject *object) {
    Terrain* terrain = object->GetComponent<Terrain>();
    TerrainEditableVertices* everts = object->GetComponent<TerrainEditableVertices>();
    for (int i=0; i<terrain->vertices.size(); i++) {
        everts->vertices.push_back(CreateVertexObject(terrain->vertices[i]));
    }
}

void TerrainVertexEditor::ObjectRemoved(Pocket::GameObject *object) {
    TerrainEditableVertices* everts = object->GetComponent<TerrainEditableVertices>();
    for (int i=0; i<everts->vertices.size(); i++) {
        everts->vertices[i]->Remove();
    }
    everts->vertices.clear();
}

void TerrainVertexEditor::Update(float dt) {
    for(GameObject* go : Objects()) {
        Terrain* terrain = go->GetComponent<Terrain>();
        TerrainEditableVertices* everts = go->GetComponent<TerrainEditableVertices>();
        bool wasChanged = false;
        if (terrain->vertices.size() > everts->vertices.size()) {
            size_t diff = terrain->vertices.size() - everts->vertices.size();
            for (int i=0; i<diff; i++) {
                everts->vertices.push_back(CreateVertexObject(terrain->vertices[i + terrain->vertices.size() - diff]));
            }
            wasChanged = true;
        } else if (terrain->vertices.size() < everts->vertices.size()) {
            size_t diff = everts->vertices.size() - terrain->vertices.size();
            for (int i=0; i<diff; i++) {
                everts->vertices.back()->Remove();
                everts->vertices.pop_back();
            }
            wasChanged = true;
        }
        if (wasChanged) {
            for (int i=0; i<everts->vertices.size(); i++) {
                Vector3 pos = everts->vertices[i]->GetComponent<Transform>()->Position = terrain->vertices[i];
            }
        }
        
        for (int i=0; i<everts->vertices.size(); i++) {
            Vector3 pos = everts->vertices[i]->GetComponent<Transform>()->Position;
            terrain->vertices[i] = pos;
        }
        terrain->CheckForChange();
    }
}

GameObject* TerrainVertexEditor::CreateVertexObject(Vector2 position) {
    GameObject* go = World()->CreateObject();
    go->AddComponent<Transform>()->Position = position;
    go->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
    go->AddComponent<Material>();
    go->AddComponent<Touchable>();
    go->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XYPlane;
    go->AddComponent<Selectable>();
    return go;
}

void TerrainVertexEditor::AddVertexSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click += event_handler(this, &TerrainVertexEditor::AddVertexSystem::Down, object);
}

void TerrainVertexEditor::AddVertexSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Touchable>()->Click -= event_handler(this, &TerrainVertexEditor::AddVertexSystem::Down, object);
}

void TerrainVertexEditor::AddVertexSystem::Down(Pocket::TouchData d, GameObject* object) {
    if (d.Index!=0) return;
    Terrain* terrain = object->GetComponent<Terrain>();
    Vector3 localPosition = object->GetComponent<Transform>()->WorldInverse.GetValue()->TransformPosition(d.WorldPosition);
    int index = FindInsertPosition(terrain, localPosition);
    terrain->vertices.insert(terrain->vertices.begin() + index, localPosition);
}

int TerrainVertexEditor::AddVertexSystem::FindInsertPosition(Terrain *terrain, const Pocket::Vector2 &position) {
    if (terrain->vertices.empty()) return 0;
    int found = 0;
    float minDistance = 1000000;
    
    for (int i=0; i<terrain->vertices.size(); i++) {
        int nextIndex = i==terrain->vertices.size() - 1 ? 0 : i + 1;
        Vector2& p1 = terrain->vertices[i];
        Vector2& p2 = terrain->vertices[nextIndex];
        float distance = SegmentDistance(p1, p2, position);
        if (distance<minDistance) {
            minDistance = distance;
            found = nextIndex;
        }
    }
    
    return found;
}

float TerrainVertexEditor::AddVertexSystem::SegmentDistance(const Pocket::Vector2 &v, const Pocket::Vector2 &w, const Pocket::Vector2 &p) {
  // Return minimum distance between line segment vw and point p
  const float l2 = (v-w).SquareLength();// length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
  if (l2 == 0.0) return (p-v).SquareLength();   // v == w case
  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line. 
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  const float t = (p - v).Dot(w - v) / l2;
  if (t < 0.0) return (p-v).SquareLength();       // Beyond the 'v' end of the segment
  else if (t > 1.0) return (p-w).SquareLength();  // Beyond the 'w' end of the segment
  const Vector2 projection = v + (w - v) * t;  // Projection falls on the segment
  return (p-projection).SquareLength();
}

void TerrainVertexEditor::ButtonDown(std::string button) {
    if (button == "\x7f") {
        for(GameObject* vertex : selectedVertices->Selected()) {
            vertex->Remove();
            for(GameObject* go : Objects()) {
                Terrain* terrain = go->GetComponent<Terrain>();
                TerrainEditableVertices* everts = go->GetComponent<TerrainEditableVertices>();
                for (int i=0; i<everts->vertices.size(); i++) {
                    if (everts->vertices[i] == vertex) {
                        terrain->vertices.erase(terrain->vertices.begin() + i);
                        everts->vertices.erase(everts->vertices.begin() + i);
                        break;
                    }
                }
            }
        }
    }
}
