//
//  TouchSystem.hpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/21/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "TouchSystem.hpp"
#include <stack>
#include "Material.hpp"

using namespace Pocket;

TouchSystem::TouchSystem() : TouchDepth(0) {
    Input = 0;
    Input.Changed.Bind([this]() {
        if (Input.PreviousValue()) {
            Input.PreviousValue()->TouchDown.Unbind(this, &TouchSystem::TouchDown);
            Input.PreviousValue()->TouchUp.Unbind(this, &TouchSystem::TouchUp);
        }
        
        if (Input) {
            Input()->TouchDown.Bind(this, &TouchSystem::TouchDown);
            Input()->TouchUp.Bind(this, &TouchSystem::TouchUp);
        }
    });
}

TouchSystem::~TouchSystem() {
    Input = 0;
}

void TouchSystem::Initialize() {
    octree = world->CreateSystem<OctreeSystem>();
    cameraSystem = world->CreateSystem<TouchSystem::CameraSystem>();
    world->CreateSystem<OrderableSystem>();
}

TouchSystem::OctreeSystem& TouchSystem::Octree() { return *octree; }

void TouchSystem::ObjectAdded(GameObject* object) {
    TouchableObject* touchableObject = new TouchableObject(object);
    if (touchableObject->clip>0 && touchableObject->orderable) {
        clippers.push_back(touchableObject);
    }
    this->SetMetaData(object, touchableObject);
}

void TouchSystem::ObjectRemoved(GameObject* object) {
    TouchableObject* touchableObject = (TouchableObject*)this->GetMetaData(object);
    if (touchableObject->clip>0 && touchableObject->orderable) {
        clippers.erase(std::find(clippers.begin(), clippers.end(), touchableObject));
    }
    
    for(int i=0; i<12; ++i) {
        Touched& list = touches[i];
        for(int j=0; j<list.size(); ++j) {
            if (list[j].Touchable == touchableObject->touchable) {
                list.erase(list.begin() + j);
                --j;
                break;
            }
        }
    }
    delete touchableObject;
}

void TouchSystem::Update(float dt) {
    if (!cancelledTouchables.empty()) {
        for (CancelledTouchables::iterator it = cancelledTouchables.begin(); it!=cancelledTouchables.end(); ++it) {
            Touchable* cancelled = *it;
            for (int i=0; i<12; i++) {
                Touched& list = touches[i];
                for (size_t j=0; j<list.size(); j++) {
                    if (list[j].Touchable == cancelled) {
                        cancelled->Cancelled.Bind(this, &TouchSystem::TouchableCancelled);
                        ups.push_back(list[j]);
                        list.erase(list.begin()+j);
                        --j;
                    }
                }
            }
        }
        cancelledTouchables.clear();
    }
    
    
    
    for (unsigned i=0; i<downs.size(); i++) {
        if (IsTouchValid(downs[i])) {
            downs[i].Touchable->Down(downs[i]);
        }
    }
    downs.clear();
    for (unsigned i=0; i<clicks.size(); i++) {
        if (IsTouchValid(clicks[i])) {
            clicks[i].Touchable->Click(clicks[i]);
        }
    }
    clicks.clear();
    for (unsigned i=0; i<ups.size(); i++) {
        if (IsTouchValid(ups[i])) {
            ups[i].Touchable->Up(ups[i]);
        }
    }
    ups.clear();
}

bool TouchSystem::IsTouchValid(const Pocket::TouchData &touchData) {
    return !Input()->IsTouchSwallowed(touchData.Index, TouchDepth);
}

void TouchSystem::TouchDown(Pocket::TouchEvent e) {
    Touched& list = touches[e.Index];
    
    FindTouchedObjects(list, e);
    AddToTouchList(list, downs);
    
    for (size_t i = 0; i<list.size(); i++) {
        list[i].Touchable->Cancelled.Bind(this, &TouchSystem::TouchableCancelled);
    }
    
    if (!list.empty()) {
        Input()->SwallowTouch(e.Index, TouchDepth);
    }
}

void TouchSystem::TouchUp(Pocket::TouchEvent e) {
    
    Touched& touchList = touches[e.Index];
    
    for (size_t i=0; i<touchList.size(); i++) {
        CancelledTouchables::iterator it = cancelledTouchables.find(touchList[i].Touchable);
        if (it!=cancelledTouchables.end()) {
            touchList.erase(touchList.begin() + i);
            i--;
        }
    }
    
    if (touchList.empty()) return;
    
    Touched list;
    FindTouchedObjects(list, e);
    
    for (unsigned i=0; i<list.size(); i++) {
        if (IsTouchInList(list[i], touchList)) {
            clicks.push_back(list[i]);
        }
    }
    
    AddToTouchList(touchList, ups);
    
    for (size_t i = 0; i<touchList.size(); i++) {
        touchList[i].Touchable->Cancelled.Unbind(this, &TouchSystem::TouchableCancelled);
    }
    
    touchList.clear();
}


TouchSystem::TouchableObject::TouchableObject(GameObject* object) {
    transform = object->GetComponent<Transform>();
    mesh = object->GetComponent<Mesh>();
    touchable = object->GetComponent<Touchable>();
    orderable = object->GetComponent<Orderable>();
    Material* material = object->GetComponent<Material>();
    clip = material ? material->Clip : 0;
}

bool TouchSystem::SortClippers(const TouchableObject *a, const TouchableObject *b) {
    if (a->orderable && b->orderable) {
        return a->orderable->Order<b->orderable->Order;
    } else {
        return false;
    }
}

TouchSystem::TouchableObject* TouchSystem::FindClipper(TouchableObject *fromThis) {
    if (!fromThis->orderable) return 0;
    int order = fromThis->orderable->Order();
    std::stack<TouchableObject*> stack;
    for (size_t i = 0; i<clippers.size(); ++i) {
        TouchableObject* clipper = clippers[i];
        int clipperOrder = clipper->orderable->Order();
        if (order<=clipperOrder) break;
        if (clipper->clip == 1) {
            stack.push(clipper);
        } else {
            stack.pop();
        }
    }
    
    return stack.empty() ? 0 : stack.top();
}

void TouchSystem::FindTouchedObjects(Touched& list, const TouchEvent& e, bool forceClickThrough) {
    const auto& cameras = cameraSystem->Objects();
    for (auto it = cameras.begin(); it!=cameras.end(); ++it) {
        FindTouchedObjectsFromCamera(*it, list, e, forceClickThrough);
    }
}

void TouchSystem::FindTouchedObjectsFromCamera(GameObject* cameraObject, Touched& list, const TouchEvent& e, bool forceClickThrough) {
    Camera* camera = cameraObject->GetComponent<Camera>();
    
    Ray ray = camera->GetRay(cameraObject->GetComponent<Transform>(), e.Position);
    
    BoundingFrustum::Count = 0;
    octree->GetObjectsAtRay(ray, touchableList);
    
    if (touchableList.empty()) {
        return;
    }
    
    /*
    float minDistance = 1000000;
    float barycentricU;
    float barycentricV;
    size_t triangleIndex = 0;
    Touchable* touchable = 0;
    Vector3 worldPosition;
    TouchableObject* touchableObject = 0;
    Vector3 touchNormal(0,0,0);
    int minOrder = -1000000;
    */

    size_t max = touchableList.size() - 1;
    for(unsigned i=0; i<touchableList.size(); i++) {
        GameObject* object = touchableList[max - i];
        TouchableObject* t = (TouchableObject*)this->GetMetaData(object);
        Mesh* mesh = t->mesh;
        const Matrix4x4& worldInverse = t->transform->WorldInverse();
        //int thisOrder = t->orderable ? t->orderable->Order.Get() : minOrder;
        
        Ray localRay = ray;
        localRay.Transform(worldInverse);
        
        float distanceToPick;
        float u;
        float v;
        size_t triIndex = 0;
        Vector3 normal(0,0,0);
        
        if (mesh->IntersectsRay(localRay, &distanceToPick, &u, &v, &triIndex, &normal)) {
            Intersection intersection;
            intersection.distanceToPick = distanceToPick;
            intersection.u = u;
            intersection.v = v;
            intersection.triIndex = triIndex;
            intersection.normal = normal;
            intersection.touchable = t;
            intersection.localRay = localRay;
            intersection.object = object;
            
            intersections.push_back(intersection);
            
            /*
            if (distanceToPick<minDistance || thisOrder>minOrder) {
                minOrder = thisOrder;
                minDistance = distanceToPick;
                barycentricU = u;
                barycentricV = v;
                touchable = t->touchable;
                triangleIndex = triIndex;
                
                const Matrix4x4* world = t->transform->World.GetValue();
                
                Vector3 localPosition = localRay.GetPosition(distanceToPick);
                worldPosition = world->TransformPosition(localPosition);
                touchableObject = t;
                 
                touchNormal = normal;
            }
            */
        }
    }
    touchableList.clear();
    
    if (intersections.empty()) return;
    
    std::sort(intersections.begin(), intersections.end(), SortIntersections);
    
    std::vector<Intersection> foundIntersections;
    
    for (size_t i = 0; i<intersections.size(); ++i) {
        Intersection& intersection = intersections[i];
        if (intersection.touchable->clip>0) continue;
        TouchableObject* clipper = FindClipper(intersection.touchable);
        if (!clipper) {
            foundIntersections.push_back(intersection);
            if (!forceClickThrough && !intersection.touchable->touchable->ClickThrough) {
                break;
            }
            //continue;
        }
    
        bool foundClipper = false;
        
        while (clipper) {
            foundClipper = false;
            for (size_t c=0; c<intersections.size(); ++c) {
                if (intersections[c].touchable == clipper) {
                    clipper = FindClipper(clipper);
                    foundClipper = true;
                    break;
                }
            }
            if (!foundClipper) break;
        }
       
    
        if (!foundClipper) continue;
        
        
        
        foundIntersections.push_back(intersection);
        if (!forceClickThrough && !intersection.touchable->touchable->ClickThrough) {
            break;
        }
    }
    
    if (foundIntersections.empty()) {
        intersections.clear();
        return;
    }
    
    for (size_t i=0; i<foundIntersections.size(); i++) {
        
        Intersection* foundIntersection = &foundIntersections[i];
        
        TouchableObject* touchableObject = foundIntersection->touchable;
        
        TouchData touch;
        touch.object = foundIntersection->object;
        touch.Input = Input;
        touch.CameraTransform = cameraObject->GetComponent<Transform>();
        touch.Camera = camera;
        touch.Touchable = foundIntersection->touchable->touchable;
        touch.Index = e.Index;
        touch.Position = e.Position;
        touch.Ray = ray;
        
        const Matrix4x4& world = touchableObject->transform->World();
        
        Vector3 localPosition = foundIntersection->localRay.GetPosition(foundIntersection->distanceToPick);
        touch.WorldPosition = world.TransformPosition(localPosition);
        touch.TriangleIndex = foundIntersection->triIndex;
        touch.WorldNormal = foundIntersection->normal;
        
        
        if (touch.WorldNormal.x == 0 && touch.WorldNormal.y == 0 && touch.WorldNormal.z == 0) {
            
            Vector3 v0 = touchableObject->mesh->Position(touchableObject->mesh->Triangles()[touch.TriangleIndex  ]);
            Vector3 v1 = touchableObject->mesh->Position(touchableObject->mesh->Triangles()[touch.TriangleIndex+1]);
            Vector3 v2 = touchableObject->mesh->Position(touchableObject->mesh->Triangles()[touch.TriangleIndex+2]);
        
            Vector3 tan1 = v1 - v0;
            Vector3 tan2 = v2 - v0;
        
            touch.WorldNormal = tan1.Cross(tan2);
            
            touch.WorldNormal = touchableObject->transform->World().TransformVector(touch.WorldNormal);
            touch.WorldNormal.Normalize();
        } 
    
        list.push_back(touch);
    }
    
    intersections.clear();
}

bool TouchSystem::SortIntersections(const Intersection &a, const Intersection &b) {
    if (a.touchable->orderable && b.touchable->orderable) {
        return a.touchable->orderable->Order()>b.touchable->orderable->Order();
    } else {
        return a.distanceToPick<b.distanceToPick;
    }
}

void TouchSystem::AddToTouchList(Touched &from, Touched &to) {
    for (unsigned i=0; i<from.size(); i++) {
        to.push_back(from[i]);
    }
}

bool TouchSystem::IsTouchInList(const Pocket::TouchData &touchData, const Touched &list) {
    for (unsigned i=0; i<list.size(); i++) {
        if (list[i].Touchable == touchData.Touchable) return true;
    }
    return false;
}

void TouchSystem::TouchableCancelled(Pocket::Touchable *touchable) {
    cancelledTouchables.insert(touchable);
}
