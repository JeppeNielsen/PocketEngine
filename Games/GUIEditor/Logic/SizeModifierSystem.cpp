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
#include "Gridable.h"
#include "SizeModifierLine.h"
#include "Orderable.hpp"

using namespace Pocket;

void SizeModifierSystem::Initialize(GameWorld *world) {
    this->world = world;
}

void SizeModifierSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Bind(this, &SizeModifierSystem::SelectionChanged, object);
}

void SizeModifierSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &SizeModifierSystem::SelectionChanged, object);
    object->GetComponent<SizeModifier>()->DeleteNodes();
}

void SizeModifierSystem::SelectionChanged(GameObject* object) {
    Selectable *selectable = object->GetComponent<Selectable>();
    SizeModifier* modifier = object->GetComponent<SizeModifier>();
    
    std::cout << "SizeModifierSystem, " << object->ID << selectable->Selected  << std::endl;
    
    if (selectable->Selected) {
        
        
        for (int i=0; i<8; ++i) {
                Draggable::MovementMode mode = i==1 || i==5 ? Draggable::MovementMode::XAxis : (i==3 || i==7 ? Draggable::MovementMode::YAxis : Draggable::MovementMode::XYPlane);
            
            modifier->Nodes[i]=CreateNode(object, i, mode);
        }
        
        for (int i=0; i<4; ++i) {
            modifier->Lines[i]=CreateLine(object,i);
        }

        
        
        
    } else {
        modifier->DeleteNodes();
    }
}

GameObject* SizeModifierSystem::CreateNode(GameObject *object, int cornerIndex, Draggable::MovementMode movementMode) {
    GameObject* go = world->CreateObject();
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
    //go->AddComponent<Gridable>()->Size = 10;
    go->AddComponent<Orderable>();
    return go;
}

GameObject* SizeModifierSystem::CreateLine(Pocket::GameObject *object, int index) {
    GameObject* go = world->CreateObject();
    go->Parent = object;
    go->AddComponent<Transform>();
    go->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1, Colour(0.0f, 0.0f, 0.0f, 0.25f));
    go->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    go->AddComponent<SizeModifierLine>()->index = index;
    go->AddComponent<Orderable>();
    return go;
}
