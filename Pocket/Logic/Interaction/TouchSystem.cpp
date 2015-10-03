//
//  TouchSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "TouchSystem.hpp"
#include "GameWorld.hpp"
#include "OctreeSystem.hpp"
#include <stack>

using namespace Pocket;

TouchSystem::TouchSystem() : Input(this) {
    Input = 0;
    Input.ChangedWithOld += event_handler(this, &TouchSystem::InputManagerChanged);
}
TouchSystem::~TouchSystem() { }

void TouchSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Mesh>();
    AddComponent<Touchable>();
}

void TouchSystem::AddedToWorld(Pocket::GameWorld &world) {
    octree = world.CreateSystem<OctreeSystem>();
    octree->AddComponent<Touchable>();
    
    cameraSystem = world.CreateSystem<TouchSystem::CameraSystem>();
}

OctreeSystem& TouchSystem::Octree() { return *octree; }

void TouchSystem::ObjectAdded(GameObject* object) {
    TouchableObject* touchableObject = new TouchableObject(object);
    if (touchableObject->clip>0 && touchableObject->orderable) {
        clippers.push_back(touchableObject);
    }
    SetMetaData(object, touchableObject);
}

void TouchSystem::ObjectRemoved(GameObject* object) {
    TouchableObject* touchableObject = (TouchableObject*)GetMetaData(object);
    if (touchableObject->clip>0 && touchableObject->orderable) {
        clippers.erase(std::find(clippers.begin(), clippers.end(), touchableObject));
    }
    delete touchableObject;
}

bool TouchSystem::SortClippers(const Pocket::TouchSystem::TouchableObject *a, const Pocket::TouchSystem::TouchableObject *b) {
    if (a->orderable && b->orderable) {
        return a->orderable->Order.Get()<b->orderable->Order.Get();
    } else {
        return false;
    }
}

TouchSystem::TouchableObject* TouchSystem::FindClipper(Pocket::TouchSystem::TouchableObject *fromThis) {
    if (!fromThis->orderable) return 0;
    int order = fromThis->orderable->Order.Get();
    std::stack<TouchableObject*> stack;
    for (size_t i = 0; i<clippers.size(); ++i) {
        TouchableObject* clipper = clippers[i];
        int clipperOrder = clipper->orderable->Order.Get();
        if (order<=clipperOrder) break;
        if (clipper->clip == 1) {
            stack.push(clipper);
        } else {
            stack.pop();
        }
    }
    
    return stack.empty() ? 0 : stack.top();
}

void TouchSystem::CameraSystem::Initialize() {
    AddComponent<Transform>();
    AddComponent<Camera>();
}

TouchSystem::TouchableObject::TouchableObject(GameObject* object) {
    transform = object->GetComponent<Transform>();
    mesh = object->GetComponent<Mesh>();
    touchable = object->GetComponent<Touchable>();
    orderable = object->GetComponent<Orderable>();
    Material* material = object->GetComponent<Material>();
    clip = material ? material->Clip : 0;
}

TouchSystem::TouchableObject::~TouchableObject() {}

void TouchSystem::InputManagerChanged(Property<TouchSystem*, InputManager*>::EventData e) {
    
    if (e.Old) {
        e.Old->TouchDown -= event_handler(this, &TouchSystem::TouchDown);
        e.Old->TouchUp -= event_handler(this, &TouchSystem::TouchUp);
    }
    
    if (e.Current) {
        e.Current->TouchDown += event_handler(this, &TouchSystem::TouchDown);
        e.Current->TouchUp += event_handler(this, &TouchSystem::TouchUp);
    }
}

void TouchSystem::TouchDown(Pocket::TouchEvent e) {
    Touched& list = touches[e.Index];
    
    FindTouchedObjects(list, e);
    AddToTouchList(list, downs);
    
    for (size_t i = 0; i<list.size(); i++) {
        list[i].Touchable->Cancelled += event_handler(this, &TouchSystem::TouchableCancelled);
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
        touchList[i].Touchable->Cancelled -= event_handler(this, &TouchSystem::TouchableCancelled);
    }
    
    touchList.clear();
}

void TouchSystem::FindTouchedObjects(Touched& list, const TouchEvent& e, bool forceClickThrough) {
    const ObjectCollection& cameras = cameraSystem->Objects();
    for (ObjectCollection::const_iterator it = cameras.begin(); it!=cameras.end(); ++it) {
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
        TouchableObject* t = (TouchableObject*)GetMetaData(object);
		Mesh* mesh = t->mesh;
        const Matrix4x4* worldInverse = t->transform->WorldInverse.GetValue();
        //int thisOrder = t->orderable ? t->orderable->Order.Get() : minOrder;
        
        Ray localRay = ray;
        localRay.Transform(*worldInverse);
        
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
        touch.CameraObject = cameraObject;
        touch.Camera = camera;
        touch.Touchable = foundIntersection->touchable->touchable;
        touch.Index = e.Index;
        touch.Position = e.Position;
        touch.Ray = ray;
        
        const Matrix4x4* world = touchableObject->transform->World.GetValue();
        
        Vector3 localPosition = foundIntersection->localRay.GetPosition(foundIntersection->distanceToPick);
        touch.WorldPosition = world->TransformPosition(localPosition);
        touch.TriangleIndex = foundIntersection->triIndex;
        touch.WorldNormal = foundIntersection->normal;
        
        
        if (touch.WorldNormal.x == 0 && touch.WorldNormal.y == 0 && touch.WorldNormal.z == 0) {
            
            Vector3 v0 = touchableObject->mesh->Position(touchableObject->mesh->Triangles()[touch.TriangleIndex  ]);
            Vector3 v1 = touchableObject->mesh->Position(touchableObject->mesh->Triangles()[touch.TriangleIndex+1]);
            Vector3 v2 = touchableObject->mesh->Position(touchableObject->mesh->Triangles()[touch.TriangleIndex+2]);
        
            Vector3 tan1 = v1 - v0;
            Vector3 tan2 = v2 - v0;
        
            touch.WorldNormal = tan1.Cross(tan2);
            
            touch.WorldNormal = touchableObject->transform->World.GetValue()->TransformVector(touch.WorldNormal);
            touch.WorldNormal.Normalize();
        } 
    
        list.push_back(touch);
    }
    
    intersections.clear();
    
    
}

bool TouchSystem::SortIntersections(const Pocket::TouchSystem::Intersection &a, const Pocket::TouchSystem::Intersection &b) {
    if (a.touchable->orderable && b.touchable->orderable) {
        return a.touchable->orderable->Order.Get()>b.touchable->orderable->Order.Get();
    } else {
        return a.distanceToPick<b.distanceToPick;
    }
}

void TouchSystem::Update(float dt) {
    
    if (!cancelledTouchables.empty()) {
        for (CancelledTouchables::iterator it = cancelledTouchables.begin(); it!=cancelledTouchables.end(); ++it) {
            Touchable* cancelled = *it;
            for (int i=0; i<12; i++) {
                Touched& list = touches[i];
                for (size_t j=0; j<list.size(); j++) {
                    if (list[j].Touchable == cancelled) {
                        cancelled->Cancelled -= event_handler(this, &TouchSystem::TouchableCancelled);
                        ups.push_back(list[j]);
                        list.erase(list.begin()+j);
                        j--;
                    }
                }
            }
        }
        cancelledTouchables.clear();
    }
    
    
    for (unsigned i=0; i<downs.size(); i++) downs[i].Touchable->Down(downs[i]);
    downs.clear();
    for (unsigned i=0; i<clicks.size(); i++) clicks[i].Touchable->Click(clicks[i]);
    clicks.clear();
    for (unsigned i=0; i<ups.size(); i++) ups[i].Touchable->Up(ups[i]);
    ups.clear();
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

