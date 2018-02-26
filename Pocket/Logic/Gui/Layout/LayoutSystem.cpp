//
//  LayoutSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/12/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "LayoutSystem.hpp"

using namespace Pocket;

void LayoutSystem::ObjectAdded(GameObject* object) {
    Layouter* layouter = object->GetComponent<Layouter>();
    
    layouter->GlobalMin.Method = [layouter, object] (Vector2& value) {
        value = DoLayout(layouter, object,
        [] (Layouter* l) {
            return l->Min();
        }, [] (Layouter* l) {
            return l->GlobalMin();
        });
    };
    
    layouter->GlobalMax.Method = [layouter, object] (Vector2& value) {
        value = DoLayout(layouter, object, [] (Layouter* l) { return l->Max(); }, [] (Layouter* l) { return l->GlobalMax(); });
    };
    
    layouter->GlobalDesired.Method = [layouter, object] (Vector2& value) {
        value = DoLayout(layouter, object, [] (Layouter* l) { return l->Desired(); }, [] (Layouter* l) { return l->GlobalDesired(); });
    };

    layouter->Min.Changed.Bind(this, &LayoutSystem::MinChanged, layouter);
    layouter->Max.Changed.Bind(this, &LayoutSystem::MaxChanged, layouter);
    layouter->Desired.Changed.Bind(this, &LayoutSystem::DesiredChanged, layouter);
    
    layouter->GlobalMin.MakeDirty();
    layouter->GlobalMax.MakeDirty();
    layouter->GlobalDesired.MakeDirty();
    
    layouter->GlobalMin.HasBecomeDirty.Bind(this, &LayoutSystem::GlobalMinDirty, object);
    layouter->GlobalMax.HasBecomeDirty.Bind(this, &LayoutSystem::GlobalMaxDirty, object);
    layouter->GlobalDesired.HasBecomeDirty.Bind(this, &LayoutSystem::GlobalDesiredDirty, object);
    
    object->Hierarchy().Parent.Changed.Bind(this, &LayoutSystem::ParentChanged, object);
    
    object->GetComponent<Sizeable>()->Size.Changed.Bind(this, &LayoutSystem::SizeChanged, object);
    
    dirtyObjects.insert(object);
    TryInvokeChangesToParent(object);
}

void LayoutSystem::ObjectRemoved(GameObject* object) {
    Layouter* layouter = object->GetComponent<Layouter>();
    layouter->GlobalMin.Method = nullptr;
    layouter->GlobalMax.Method = nullptr;
    layouter->GlobalDesired.Method = nullptr;
    
    layouter->Min.Changed.Unbind(this, &LayoutSystem::MinChanged, layouter);
    layouter->Max.Changed.Unbind(this, &LayoutSystem::MaxChanged, layouter);
    layouter->Desired.Changed.Unbind(this, &LayoutSystem::DesiredChanged, layouter);
    
    layouter->GlobalMin.HasBecomeDirty.Unbind(this, &LayoutSystem::GlobalMinDirty, object);
    layouter->GlobalMax.HasBecomeDirty.Unbind(this, &LayoutSystem::GlobalMaxDirty, object);
    layouter->GlobalDesired.HasBecomeDirty.Unbind(this, &LayoutSystem::GlobalDesiredDirty, object);
    
    object->Hierarchy().Parent.Changed.Unbind(this, &LayoutSystem::ParentChanged, object);
    
    object->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &LayoutSystem::SizeChanged, object);
    
    TryInvokeChangesToParent(object);
    
    auto it = dirtyObjects.find(object);
    if (it!=dirtyObjects.end()) {
        dirtyObjects.erase(it);
    }
}

void LayoutSystem::TryInvokeChangesToParent(Pocket::GameObject *object) {
    GameObject* current = object->Hierarchy().Parent;
    Layouter* currentLayouter = current ? current->GetComponent<Layouter>() : nullptr;
    if (currentLayouter && current && !current->IsRemoved()) {
        currentLayouter->GlobalMin.MakeDirty();
        currentLayouter->GlobalMax.MakeDirty();
        currentLayouter->GlobalDesired.MakeDirty();
        dirtyObjects.insert(current);
    }
}

void LayoutSystem::MinChanged(Layouter* layouter) {
    layouter->GlobalMin.MakeDirty();
}

void LayoutSystem::MaxChanged(Layouter* layouter) {
    layouter->GlobalMax.MakeDirty();
}

void LayoutSystem::DesiredChanged(Layouter* layouter) {
    layouter->GlobalDesired.MakeDirty();
}

void LayoutSystem::GlobalMinDirty(GameObject* object) {
    GameObject* parent = object->Hierarchy().Parent;
    if (!object) return;
    Layouter* parentLayouter = parent->GetComponent<Layouter>();
    if (parentLayouter) {
        parentLayouter->GlobalMin.MakeDirty();
    }
    dirtyObjects.insert(object);
}

void LayoutSystem::GlobalMaxDirty(GameObject* object) {
    GameObject* parent = object->Hierarchy().Parent;
    if (!object) return;
    Layouter* parentLayouter = parent->GetComponent<Layouter>();
    if (parentLayouter) {
        parentLayouter->GlobalMax.MakeDirty();
    }
    dirtyObjects.insert(object);
}

void LayoutSystem::GlobalDesiredDirty(GameObject* object) {
    GameObject* parent = object->Hierarchy().Parent;
    if (!object) return;
    Layouter* parentLayouter = parent->GetComponent<Layouter>();
    if (parentLayouter) {
        parentLayouter->GlobalDesired.MakeDirty();
    }
    dirtyObjects.insert(object);
}

void LayoutSystem::ParentChanged(GameObject* object) {
    GameObject* old = object->Hierarchy().Parent.PreviousValue();
    GameObject* current = object->Hierarchy().Parent;
    
    Layouter* oldLayouter = old ? old->GetComponent<Layouter>() : nullptr;
    Layouter* currentLayouter = current ? current->GetComponent<Layouter>() : nullptr;
    
    if (oldLayouter) {
        dirtyObjects.insert(old);
    }
    if (currentLayouter) {
        dirtyObjects.insert(current);
    }
}

void LayoutSystem::SizeChanged(GameObject* object) {
    dirtyObjects.insert(object);
}

Vector2 LayoutSystem::DoLayout(Layouter* layouter, GameObject* object,
    const std::function<Vector2(Layouter* layouter)>& localGetter,
    const std::function<Vector2(Layouter* layouter)>& globalGetter
) {
    int count = 0;
    for(auto child : object->Hierarchy().Children()) {
        if (!child->Hierarchy().Enabled) continue;
        Layouter* childLayouter = child->GetComponent<Layouter>();
        if (!childLayouter) continue;
        count++;
    }
    if (count == 0) {
        return localGetter(layouter);
    }
	
    if (layouter->ChildrenLayoutMode() == Layouter::LayoutMode::Horizontal) {
        Vector2 size { 0, localGetter(layouter).y };
        for(auto child : object->Hierarchy().Children()) {
            if (!child->Hierarchy().Enabled) continue;
            Layouter* childLayouter = child->GetComponent<Layouter>();
            if (!childLayouter) continue;
            Vector2 childSize = globalGetter(childLayouter);
            size.x += childSize.x;
            size.y = std::max(size.y, childSize.y);
        }
        //size.x = std::max(layoutSize.x, size.x);
        return size;
    } else {
        Vector2 size { localGetter(layouter).x, 0 };
        for(auto child : object->Hierarchy().Children()) {
            if (!child->Hierarchy().Enabled) continue;
            Layouter* childLayouter = child->GetComponent<Layouter>();
            if (!childLayouter) continue;
            Vector2 childSize = globalGetter(childLayouter);
            size.x = std::max(size.x, childSize.x);
            size.y += childSize.y;
        }
        //size.y = std::max(layoutSize.y, size.y);
        return size;
    }
}

void LayoutSystem::Update(float dt) {
    while (!dirtyObjects.empty()) {
        std::set<GameObject*> temp = dirtyObjects;
        dirtyObjects.clear();
        for(auto o : temp) {
            CalcLayout(o);
        }
    }
}

void LayoutSystem::CalcLayout(GameObject* object) {
    Vector2 size = object->GetComponent<Sizeable>()->Size;
    Layouter* layouter = object->GetComponent<Layouter>();
    
    if (layouter->ChildrenLayoutMode() == Layouter::LayoutMode::Horizontal) {
        if (layouter->GlobalMin().x>=size.x) {
            float x = 0;
            for(auto child : object->Hierarchy().Children()) {
                if (!child->Hierarchy().Enabled) continue;
                Layouter* childLayouter = child->GetComponent<Layouter>();
                Sizeable* childSizable = child->GetComponent<Sizeable>();
                Transform* childTransform = child->GetComponent<Transform>();
                if (!childLayouter || !childSizable || !childTransform) continue;
                
                float yPos = size.y * 0.5f - childLayouter->Min().y*0.5f;
                
                Vector2 childSize = { childLayouter->GlobalMin().x, childLayouter->Min().y };
                
                childSizable->Size = childSize;
                childTransform->Position = {x,yPos};
                x+=childSize.x;
            }
        } else if (layouter->GlobalDesired().x>=size.x) {
            float desiredMargin = layouter->GlobalDesired().x - layouter->GlobalMin().x;
            float fraction = (size.x - layouter->GlobalMin().x) / desiredMargin;
            float x = 0;
            for(auto child : object->Hierarchy().Children()) {
                if (!child->Hierarchy().Enabled) continue;
                Layouter* childLayouter = child->GetComponent<Layouter>();
                Sizeable* childSizable = child->GetComponent<Sizeable>();
                Transform* childTransform = child->GetComponent<Transform>();
                if (!childLayouter || !childSizable || !childTransform) continue;
                
                float minWidth = childLayouter->GlobalMin().x;
                float desWidth = childLayouter->GlobalDesired().x;
                
                float yPos = size.y * 0.5f - childLayouter->Min().y*0.5f;
                
                
                Vector2 childSize = { minWidth + (desWidth - minWidth) * fraction, childLayouter->Min().y };
                
                childSizable->Size = childSize;
                childTransform->Position = {x,yPos};
                x+=childSize.x;
            }
        } else {
            float maxMargin = layouter->GlobalMax().x - layouter->GlobalDesired().x;
            float fraction = maxMargin > -0.00001f && maxMargin < 0.00001f ? 1 : (size.x - layouter->GlobalDesired().x) / maxMargin;
            if (fraction>1) fraction = 1;
            
            float x = 0;
            for(auto child : object->Hierarchy().Children()) {
                if (!child->Hierarchy().Enabled) continue;
                Layouter* childLayouter = child->GetComponent<Layouter>();
                Sizeable* childSizable = child->GetComponent<Sizeable>();
                Transform* childTransform = child->GetComponent<Transform>();
                if (!childLayouter || !childSizable || !childTransform) continue;
                
                float desWidth = childLayouter->GlobalDesired().x;
                float maxWidth = childLayouter->GlobalMax().x;
                
                float yPos = size.y * 0.5f - childLayouter->Min().y*0.5f;
                
                Vector2 childSize = { desWidth + (maxWidth - desWidth) * fraction, childLayouter->Min().y };
                
                childSizable->Size = childSize;
                childTransform->Position = {x, yPos};
                x+=childSize.x;
            }
        }
    } else {
    
        if (layouter->GlobalMin().y>=size.y) {
            float y = 0;
            for(auto child : object->Hierarchy().Children()) {
                if (!child->Hierarchy().Enabled) continue;
                Layouter* childLayouter = child->GetComponent<Layouter>();
                Sizeable* childSizable = child->GetComponent<Sizeable>();
                Transform* childTransform = child->GetComponent<Transform>();
                if (!childLayouter || !childSizable || !childTransform) continue;
                
                Vector2 childSize = { size.x, childLayouter->GlobalMin().y };
                
                childSizable->Size = childSize;
                childTransform->Position = {0,y};
                y+=childSize.y;
            }
        } else if (layouter->GlobalDesired().y>=size.y) {
            float desiredMargin = layouter->GlobalDesired().y - layouter->GlobalMin().y;
            float fraction = (size.y - layouter->GlobalMin().y) / desiredMargin;
            float y = 0;
            for(auto child : object->Hierarchy().Children()) {
                if (!child->Hierarchy().Enabled) continue;
                Layouter* childLayouter = child->GetComponent<Layouter>();
                Sizeable* childSizable = child->GetComponent<Sizeable>();
                Transform* childTransform = child->GetComponent<Transform>();
                if (!childLayouter || !childSizable || !childTransform) continue;
                
                float minWidth = childLayouter->GlobalMin().y;
                float desWidth = childLayouter->GlobalDesired().y;
                
                Vector2 childSize = { size.x, minWidth + (desWidth - minWidth) * fraction };
                
                childSizable->Size = childSize;
                childTransform->Position = {0,y};
                y+=childSize.y;
            }
        } else {
            float maxMargin = layouter->GlobalMax().y - layouter->GlobalDesired().y;
            float fraction = maxMargin > -0.00001f && maxMargin < 0.00001f ? 1 : (size.y - layouter->GlobalDesired().y) / maxMargin;
            if (fraction>1) fraction = 1;
        
            float y = 0;
            for(auto child : object->Hierarchy().Children()) {
                if (!child->Hierarchy().Enabled) continue;
                Layouter* childLayouter = child->GetComponent<Layouter>();
                Sizeable* childSizable = child->GetComponent<Sizeable>();
                Transform* childTransform = child->GetComponent<Transform>();
                if (!childLayouter || !childSizable || !childTransform) continue;
                
                float desWidth = childLayouter->GlobalDesired().y;
                float maxWidth = childLayouter->GlobalMax().y;
                
                Vector2 childSize = { size.x, desWidth + (maxWidth - desWidth) * fraction };
                
                childSizable->Size = childSize;
                childTransform->Position = {0, y};
                y+=childSize.y;
            }
        }
    }
}
