//
//  LayoutSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/19/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "LayoutSystem.hpp"
#include "Transform.hpp"

using namespace Pocket;

void LayoutSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Sizeable>();
    AddComponent<Layoutable>();
}

void LayoutSystem::ObjectAdded(Pocket::GameObject *object) {
    SetMetaData(object, new LayoutObject(object, this));
}

void LayoutSystem::ObjectRemoved(Pocket::GameObject *object) {
    LayoutObject* layoutObject = (LayoutObject*)GetMetaData(object);
    delete layoutObject;
}

LayoutSystem::LayoutObject::LayoutObject(GameObject* object, LayoutSystem* layoutSystem) {
    this->layoutSystem = layoutSystem;
    sizeable = object->GetComponent<Sizeable>();
    layoutable = object->GetComponent<Layoutable>();
    transform = object->GetComponent<Transform>();
    object->Parent.ChangedWithOld += event_handler(this, &LayoutSystem::LayoutObject::ParentChanged);
    Property<GameObject*, GameObject*>::EventData d;
    d.Old = 0;
    d.Current = object->Parent;
    ParentChanged(d);
}

LayoutSystem::LayoutObject::~LayoutObject() {
    object->Parent.ChangedWithOld -= event_handler(this, &LayoutSystem::LayoutObject::ParentChanged);
    Property<GameObject*, GameObject*>::EventData d;
    d.Old = object->Parent;
    d.Current = 0;
    ParentChanged(d);
}

void LayoutSystem::LayoutObject::ParentChanged(Property<GameObject*, GameObject*>::EventData d) {
    
    if (d.Old) {
        Sizeable* sizeable = d.Old->GetComponent<Sizeable>();
        if (sizeable) {
            sizeable->Size.ChangedWithOld -= event_handler(this, &LayoutSystem::LayoutObject::ParentSizeChanged);
        }
    }
    
    if (d.Current) {
        Sizeable* sizeable = d.Current->GetComponent<Sizeable>();
        if (sizeable) {
            sizeable->Size.ChangedWithOld += event_handler(this, &LayoutSystem::LayoutObject::ParentSizeChanged);
        }
    }
    
}

void LayoutSystem::LayoutObject::ParentSizeChanged(Property<Sizeable*, Vector2>::EventData d){
    oldSize = d.Old;
    deltaSize = d.Current - d.Old;
    layoutSystem->dirtyObjects.insert(this);
}

void LayoutSystem::LayoutObject::Update() {
    Vector2 size = sizeable->Size;
    Vector3 position = transform->Position;
    
    switch (layoutable->HorizontalAlignment()) {
        case Layoutable::HAlignment::None:
        case Layoutable::HAlignment::Left: {
            break;
        }
        case Layoutable::HAlignment::Center: {
            position.x += deltaSize.x * 0.5f;
            break;
        }
        case Layoutable::HAlignment::Right: {
            position.x += deltaSize.x;
            break;
        }
        case Layoutable::HAlignment::Relative: {
            position.x += deltaSize.x * (position.x / oldSize.x);
            size.x += deltaSize.x * (size.x / oldSize.x);
            break;
        }
    }
    
    switch (layoutable->VerticalAlignment()) {
        case Layoutable::HAlignment::None:
        case Layoutable::VAlignment::Bottom: {
            break;
        }
        case Layoutable::VAlignment::Center: {
            position.y += deltaSize.y * 0.5f;
            break;
        }
        case Layoutable::VAlignment::Top: {
            position.y += deltaSize.y;
            break;
        }
        case Layoutable::VAlignment::Relative: {
            position.y += deltaSize.y * (position.y / oldSize.y);
            size.y += deltaSize.y * (size.y / oldSize.y);
            break;
        }
    }
    
    
    sizeable->Size = size;
    transform->Position = position;
}

void LayoutSystem::Update(float dt) {
 
    while (!dirtyObjects.empty()) {
        DirtyObjects temp = dirtyObjects;
        dirtyObjects.clear();
        for (DirtyObjects::iterator it = temp.begin(); it!=temp.end(); ++it) {
            (*it)->Update();
        }
    }
}


/*
void LayoutSystem::LayoutablesSystem::Initialize() {
    AddComponent<Layoutable>();
    AddComponent<Sizeable>();
    AddComponent<Transform>();
}

void LayoutSystem::LayouterChanged(Pocket::Layouter *layouter) {
    dirtyLayouters.insert(layouter);
}

void LayoutSystem::SizeChanged(Pocket::Sizeable *sizable) {
    LayouterChanged(sizable->Owner()->GetComponent<Layouter>());
}

void LayoutSystem::HierarchyChanged(Pocket::Hierarchy *hierarchy) {
    LayouterChanged(hierarchy->Owner()->GetComponent<Layouter>());
}

void LayoutSystem::Update(float dt) {
    while (!dirtyLayouters.empty()) {
        DirtyLayouters tempList = dirtyLayouters;
        dirtyLayouters.clear();
        
        for (DirtyLayouters::iterator it = tempList.begin(); it!=tempList.end(); ++it) {
            UpdateLayout(*it);
        }
    }
}

void LayoutSystem::GetLayoutables(Pocket::Layouter *layouter, Layoutables &layoutables) {
    Hierarchy* hierarchy = layouter->Owner()->GetComponent<Hierarchy>();
    const Hierarchy::ChildrenCollection& children = hierarchy->Children();
    
    for (Hierarchy::ChildrenCollection::const_iterator it = children.begin(); it!=children.end(); ++it) {
        Hierarchy* child = *it;
        Layoutable* layoutable = child->Owner()->GetComponent<Layoutable>();
        if (!layoutable) continue;
        Sizeable* sizable = child->Owner()->GetComponent<Sizeable>();
        if (!sizable) continue;
        Transform* transform = child->Owner()->GetComponent<Transform>();
        if (!transform) continue;
        layoutables.push_back(layoutable);
    }
}

void LayoutSystem::UpdateLayout(Pocket::Layouter *layouter) {

    Sizeable* sizable = layouter->Owner()->GetComponent<Sizeable>();
    
    int cells = layouter->Cells;
    const Vector2& margin = layouter->Margin.GetValue();

    Layoutables layoutables;
    GetLayoutables(layouter, layoutables);
    if (layoutables.empty()) return;
    int count = (int)layoutables.size();
    
    typedef std::vector<Vector2> Sizes;
    Sizes sizes(count);
    
    
    int cell = 0;
    float rowWidth = 0;
    float rowHeight = 0;
    float totalHeight = 0;
    
    for (int i=0; i<count; ++i) {
        Layoutable* layoutable = layoutables[i];
        cell++;
        rowWidth+=layoutable->Fill().x;
        if (layoutable->Fill().y>rowHeight) {
            rowHeight = layoutable->Fill().y;
        }
        if (cell>=cells || i>=count - 1) {
            for (int o=i-(cell-1); o<=i; ++o) {
                sizes[o].x = layoutables[o]->Fill().x / rowWidth;
                sizes[o].y = rowHeight;
            }
            totalHeight += rowHeight;
            rowWidth = rowHeight = 0;
            cell = 0;
        }
    }
    
    for (int i=0; i<count; ++i) {
        sizes[i].y /= totalHeight;
    }
    
    cell = 0;
    
    Vector2 layoutSize = sizable->Size() - margin * 2.0f;
    
    for (int i=0; i<count; ++i) {
        sizes[i] *= layoutSize;
    }
    
    Vector2 position = margin;
    
    for (int i=0; i<count; ++i) {
        Layoutable* layoutable = layoutables[i];
        
        Transform* transform = layoutable->Owner()->GetComponent<Transform>();
        Sizeable* s = layoutable->Owner()->GetComponent<Sizeable>();
        s->Size = sizes[i];
        transform->Position = position;
        
        cell++;
        position.x += sizes[i].x;
        
        if (cell>=cells) {
            cell = 0;
            position.x = margin.x;
            position.y += sizes[i].y;
        }
    }
}

*/

