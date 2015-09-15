//
//  SizeModifierSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/22/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SizeModifierSystem.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Touchable.hpp"

void SizeModifierSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed += event_handler(this, &SizeModifierSystem::SelectionChanged, object);
}

void SizeModifierSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed -= event_handler(this, &SizeModifierSystem::SelectionChanged, object);
    object->GetComponent<SizeModifier>()->DeleteNodes();
}

void SizeModifierSystem::SelectionChanged(Selectable *selectable, GameObject* object) {
    SizeModifier* modifier = object->GetComponent<SizeModifier>();
    if (selectable->Selected) {
        for (int i=0; i<8; ++i) {
                Draggable::MovementMode mode = i==1 || i==5 ? Draggable::MovementMode::XAxis : (i==3 || i==7 ? Draggable::MovementMode::YAxis : Draggable::MovementMode::XYPlane);
            
            modifier->Nodes[i]=CreateNode(object, i, mode);
        }
    } else {
        modifier->DeleteNodes();
    }
}

GameObject* SizeModifierSystem::CreateNode(GameObject *object, int cornerIndex, Draggable::MovementMode movementMode) {
    GameObject* go = World()->CreateObject();
    if (go == object) {
        std::cout<<"NONON";
    }
    go->AddComponent<Transform>();
    go->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 10, Colour::White());
    go->AddComponent<Material>();
    go->AddComponent<Touchable>();
    go->AddComponent<Draggable>()->Movement = movementMode;
    SizeModifierNode* node = go->AddComponent<SizeModifierNode>();
    node->cornerIndex = cornerIndex;
    node->sizableTarget = object->GetComponent<Sizeable>();
    node->transformTarget = object->GetComponent<Transform>();
    go->Parent = object;
    return go;
}