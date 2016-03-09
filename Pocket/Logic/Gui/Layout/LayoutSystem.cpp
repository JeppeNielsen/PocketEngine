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

void LayoutSystem::ObjectAdded(Pocket::GameObject *object) {
    SetMetaData(object, new LayoutObject(object, this));
}

void LayoutSystem::ObjectRemoved(Pocket::GameObject *object) {
    LayoutObject* layoutObject = (LayoutObject*)GetMetaData(object);
    for (auto o : Objects()) {
        LayoutObject* l = (LayoutObject*)GetMetaData(o);
        if (l && l->parentLayoutObject == layoutObject) {
            l->parentLayoutObject = 0;
        }
    }
    delete layoutObject;
}

LayoutSystem::LayoutObject::LayoutObject(GameObject* object, LayoutSystem* layoutSystem) {
    this->layoutSystem = layoutSystem;
    this->object = object;
    sizeable = object->GetComponent<Sizeable>();
    sizeable->Size.Changed.Bind(this, &LayoutSystem::LayoutObject::SizeChanged);
    layoutable = object->GetComponent<Layoutable>();
    transform = object->GetComponent<Transform>();
    parentSizeable = 0;
    parentLayoutObject = 0;
    object->Parent.Changed.Bind(this, &LayoutSystem::LayoutObject::ParentChanged);
    ParentChanged();
    if (layoutable->ChildLayouting!=Layoutable::ChildLayouting::None) {
        layoutSystem->dirtyChildLayoutables.insert(this);
    }
}

LayoutSystem::LayoutObject::~LayoutObject() {
    object->Parent.Changed.Unbind(this, &LayoutSystem::LayoutObject::ParentChanged);
    ParentChanged();
    auto it = layoutSystem->dirtyObjects.find(this);
    if (it!=layoutSystem->dirtyObjects.end()) {
        layoutSystem->dirtyObjects.erase(it);
    }
    auto it2 = layoutSystem->dirtyChildLayoutables.find(this);
    if (it2!=layoutSystem->dirtyChildLayoutables.end()) {
        layoutSystem->dirtyChildLayoutables.erase(it2);
    }
    sizeable->Size.Changed.Unbind(this, &LayoutSystem::LayoutObject::SizeChanged);
}

void LayoutSystem::LayoutObject::SizeChanged() {
    if (!parentLayoutObject) return;
    if (parentLayoutObject->layoutable->ChildLayouting!=Layoutable::ChildLayouting::None) {
        layoutSystem->dirtyChildLayoutables.insert(parentLayoutObject);
    }
}

void LayoutSystem::LayoutObject::ParentChanged() {
    if (parentSizeable) {
        parentSizeable->Size.Changed.Bind(this, &LayoutSystem::LayoutObject::ParentSizeChanged);
        parentSizeable = 0;
        if (parentLayoutObject) {
            layoutSystem->dirtyChildLayoutables.insert(parentLayoutObject);
        }
        parentLayoutObject = 0;
    }
    
    if (object->Parent) {
        parentSizeable = object->Parent()->GetComponent<Sizeable>();
        if (parentSizeable) {
            parentSizeable->Size.Changed.Bind(this, &LayoutSystem::LayoutObject::ParentSizeChanged);
            
            if (object->Parent()->GetComponent<Layoutable>()) {
                parentLayoutObject = (LayoutObject*)layoutSystem->GetMetaData(object->Parent);
                if (parentLayoutObject) {
                    if (parentLayoutObject->layoutable->ChildLayouting==Layoutable::ChildLayouting::None) {
                        parentLayoutObject = 0;
                    } else {
                        layoutSystem->dirtyChildLayoutables.insert(parentLayoutObject);
                    }
                }
            } else {
                parentLayoutObject = 0;
            }
        }
    }
}

void LayoutSystem::LayoutObject::ParentSizeChanged(){
    oldSize = parentSizeable->Size.PreviousValue();
    deltaSize = parentSizeable->Size - oldSize;
    layoutSystem->dirtyObjects.insert(this);
    if (parentLayoutObject) {
        layoutSystem->dirtyChildLayoutables.insert(parentLayoutObject);
    }
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
        case Layoutable::VAlignment::None:
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

void LayoutSystem::LayoutObject::UpdateChildren() {
    ObjectCollection children = object->Children();
    std::sort(children.begin(), children.end(), [] (GameObject* a, GameObject* b) {
        return a->Order()<b->Order();
    });
    switch (layoutable->ChildLayouting()) {
        case Layoutable::ChildLayouting::HorizontalStackedLeft:
        case Layoutable::ChildLayouting::HorizontalStackedCenter:
        case Layoutable::ChildLayouting::HorizontalStackedRight:
        case Layoutable::ChildLayouting::HorizontalStackedFit: {
            Vector2 position(0,0);
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                t->Position = position;
                position.x += s->Size().x;
            });
            if (layoutable->ChildLayouting() == Layoutable::ChildLayouting::HorizontalStackedCenter) {
                float offset = sizeable->Size().x * 0.5f - position.x * 0.5f;
                IterateChildren(children, [&](Transform* t, Sizeable* s) {
                    Vector3 position = t->Position;
                    position.x+=offset;
                    t->Position = position;
                });
            } else if (layoutable->ChildLayouting() == Layoutable::ChildLayouting::HorizontalStackedRight) {
                float offset = sizeable->Size().x - position.x;
                IterateChildren(children, [&](Transform* t, Sizeable* s) {
                    Vector3 position = t->Position;
                    position.x+=offset;
                    t->Position = position;
                });
            } else if (layoutable->ChildLayouting() == Layoutable::ChildLayouting::HorizontalStackedFit) {
                Vector2 size = sizeable->Size;
                size.x = position.x;
                sizeable->Size = size;
            }
            
            break;
        }
        case Layoutable::ChildLayouting::HorizontalEvenlySized: {
            Vector2 position(0,0);
            int count = 0;
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                count++;
            });
            if (count == 0) return;
            float width = sizeable->Size().x / count;
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                t->Position = position;
                s->Size = Vector2(width, s->Size().y);
                position.x += width;
            });
            break;
        }
        case Layoutable::ChildLayouting::HorizontalCentered: {
            float offset = 0;;
            std::map<Transform*, float> offsets;
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                offsets[t] = offset + s->Size().x * 0.5f;
                offset+=s->Size().x;
            });
            if (offsets.empty()) return;
            float ratio = sizeable->Size().x / offset;
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                Vector3 p = t->Position;
                p.x = offsets[t] * ratio - s->Size().x * 0.5f;
                t->Position = p;
            });
            break;
        }
        case Layoutable::ChildLayouting::VerticalStackedTop:
        case Layoutable::ChildLayouting::VerticalStackedCenter:
        case Layoutable::ChildLayouting::VerticalStackedBottom:
        case Layoutable::ChildLayouting::VerticalStackedFit: {
            Vector2 position(0,0);
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                Vector3 p = t->Position;
                p.y = position.y;
                t->Position = p;
                position.y += s->Size().y;
            });
            if (layoutable->ChildLayouting() == Layoutable::ChildLayouting::VerticalStackedCenter) {
                float offset = sizeable->Size().y * 0.5f - position.y * 0.5f;
                IterateChildren(children, [&](Transform* t, Sizeable* s) {
                    Vector3 position = t->Position;
                    position.y+=offset;
                    t->Position = position;
                });
            } else if (layoutable->ChildLayouting() == Layoutable::ChildLayouting::VerticalStackedTop) {
                float offset = sizeable->Size().y - position.y;
                IterateChildren(children, [&](Transform* t, Sizeable* s) {
                    Vector3 position = t->Position;
                    position.y+=offset;
                    t->Position = position;
                });
            } else if (layoutable->ChildLayouting() == Layoutable::ChildLayouting::VerticalStackedFit) {
                Vector2 size = sizeable->Size;
                size.y = position.y;
                sizeable->Size = size;
            }
            break;
        }
        case Layoutable::ChildLayouting::VerticalEvenlySized: {
            Vector2 position(0,0);
            int count = 0;
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                count++;
            });
            if (count == 0) return;
            float height = sizeable->Size().y / count;
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                t->Position = position;
                s->Size = Vector2(s->Size().x,height);
                position.y += height;
            });
            break;
        }
        case Layoutable::ChildLayouting::VerticalCentered: {
            float offset = 0;
            std::map<Transform*, float> offsets;
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                offsets[t] = offset + s->Size().y * 0.5f;
                offset+=s->Size().y;
            });
            if (offsets.empty()) return;
            float ratio = sizeable->Size().y / offset;
            IterateChildren(children, [&](Transform* t, Sizeable* s) {
                Vector3 p = t->Position;
                p.y = offsets[t] * ratio - s->Size().y * 0.5f;
                t->Position = p;
            });
            break;
        }
    default:
        break;
    }
}

void LayoutSystem::LayoutObject::IterateChildren(const ObjectCollection& children, std::function<void (Transform *, Sizeable *)> function) {
    for(auto child : children) {
        Layoutable* childLayoutable = child->GetComponent<Layoutable>();
        if (!childLayoutable) continue;
        Transform* transform = child->GetComponent<Transform>();
        if (!transform) continue;
        Sizeable* sizeable = child->GetComponent<Sizeable>();
        if (!sizeable) continue;
        
        function(transform, sizeable);
    }
}

void LayoutSystem::Update(float dt) {
    while (true) {
        while (!dirtyObjects.empty()) {
            DirtyObjects temp = dirtyObjects;
            dirtyObjects.clear();
            for (DirtyObjects::iterator it = temp.begin(); it!=temp.end(); ++it) {
                (*it)->Update();
            }
        }
        
        while (!dirtyChildLayoutables.empty()) {
            DirtyObjects temp = dirtyChildLayoutables;
            dirtyChildLayoutables.clear();
            for(auto layoutObject : temp) {
                layoutObject->UpdateChildren();
            }
        }
        if (dirtyObjects.empty()) break;
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

