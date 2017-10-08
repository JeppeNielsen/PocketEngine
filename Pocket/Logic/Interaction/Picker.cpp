//
//  Picker.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/10/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Picker.hpp"
#include "Orderable.hpp"
#include "Renderable.hpp"
#include "Mesh.hpp"
#include <stack>

using namespace Pocket;

bool Picker::SortClippers(GameObject *a, GameObject *b) {
    auto ao = a->GetComponent<Orderable>();
    auto bo = b->GetComponent<Orderable>();
    if (ao && bo) {
        return ao->Order<bo->Order;
    } else {
        return false;
    }
}

void Picker::TryAddClipper(Pocket::GameObject *object) {
    if (object->GetComponent<Renderable>() && object->GetComponent<Renderable>()->Clip>0 && object->GetComponent<Orderable>()) {
        clippers.push_back(object);
    }
}

void Picker::TryRemoveClipper(Pocket::GameObject *object) {
    auto it = std::find(clippers.begin(), clippers.end(), object);
    if (it!=clippers.end()) {
        clippers.erase(it);
    }
}

GameObject* Picker::FindClipper(GameObject *fromThis) {
    if (!fromThis->GetComponent<Orderable>()) return 0;
    int order = fromThis->GetComponent<Orderable>()->Order();
    std::stack<GameObject*> stack;
    for (size_t i = 0; i<clippers.size(); ++i) {
        GameObject* clipper = clippers[i];
        int clipperOrder = clipper->GetComponent<Orderable>()->Order();
        if (order<=clipperOrder) break;
        if (clipper->GetComponent<Renderable>()->Clip == 1) {
            stack.push(clipper);
        } else {
            stack.pop();
        }
    }
    
    return stack.empty() ? nullptr : stack.top();
}


void Picker::Pick(GameObject* cameraObject, std::vector<TouchData>& list, const TouchEvent& e, bool forceClickThrough, const std::function<void(const Ray&, ObjectCollection& pickedObjects)>& octreeFunction, InputManager* input) {
    Camera* camera = cameraObject->GetComponent<Camera>();
    
    Ray ray = camera->GetRay(cameraObject->GetComponent<Transform>(), e.Position);
    
    ObjectCollection touchableList;
    
    octreeFunction(ray, touchableList);
    
    if (touchableList.empty()) {
        return;
    }
    
    std::vector<Intersection> intersections;

    size_t max = touchableList.size() - 1;
    for(unsigned i=0; i<touchableList.size(); i++) {
        GameObject* object = touchableList[max - i];
        Mesh* mesh = object->GetComponent<Mesh>();
        const Matrix4x4& worldInverse = object->GetComponent<Transform>()->WorldInverse();
        //int thisOrder = t->orderable ? t->orderable->Order.Get() : minOrder;
        
        Ray localRay = ray;
        localRay.Transform(worldInverse);
        
        float distanceToPick;
        float u;
        float v;
        size_t triIndex = 0;
        Vector3 normal(0,0,0);
        
        if (mesh->IntersectsRay(localRay, &distanceToPick, &u, &v, &triIndex, &normal)) {
            Picker::Intersection intersection;
            intersection.distanceToPick = distanceToPick;
            intersection.u = u;
            intersection.v = v;
            intersection.triIndex = triIndex;
            intersection.normal = normal;
            intersection.localRay = localRay;
            intersection.object = object;
            
            intersections.push_back(intersection);
        }
    }
    if (intersections.empty()) return;
    
    std::sort(intersections.begin(), intersections.end(), SortIntersections);
    
    std::vector<Intersection> foundIntersections;
    
    for (size_t i = 0; i<intersections.size(); ++i) {
        Intersection& intersection = intersections[i];
        if (intersection.object->GetComponent<Renderable>() && intersection.object->GetComponent<Renderable>()->Clip>0) continue;
        GameObject* clipper = FindClipper(intersection.object);
        if (!clipper) {
            foundIntersections.push_back(intersection);
            if (!forceClickThrough && !(intersection.object->GetComponent<Touchable>() && intersection.object->GetComponent<Touchable>()->ClickThrough)) {
                break;
            }
            //continue;
        }
    
        bool foundClipper = false;
        
        while (clipper) {
            foundClipper = false;
            for (size_t c=0; c<intersections.size(); ++c) {
                if (intersections[c].object == clipper) {
                    clipper = FindClipper(clipper);
                    foundClipper = true;
                    break;
                }
            }
            if (!foundClipper) break;
        }
       
    
        if (!foundClipper) continue;
        
        
        
        foundIntersections.push_back(intersection);
        if (!forceClickThrough && !(intersection.object->GetComponent<Touchable>() && intersection.object->GetComponent<Touchable>()->ClickThrough)) {
            break;
        }
    }
    
    if (foundIntersections.empty()) {
        intersections.clear();
        return;
    }
    
    for (size_t i=0; i<foundIntersections.size(); i++) {
        
        Intersection& foundIntersection = foundIntersections[i];
        
        TouchData touch;
        touch.object = foundIntersection.object;
        touch.Input = input;
        touch.CameraTransform = cameraObject->GetComponent<Transform>();
        touch.Camera = camera;
        touch.Touchable = foundIntersection.object->GetComponent<Touchable>();
        touch.Index = e.Index;
        touch.Position = e.Position;
        touch.Ray = ray;
        
        const Matrix4x4& world = foundIntersection.object->GetComponent<Transform>()->World();
        
        Vector3 localPosition = foundIntersection.localRay.GetPosition(foundIntersection.distanceToPick);
        touch.WorldPosition = world.TransformPosition(localPosition);
        touch.TriangleIndex = foundIntersection.triIndex;
        touch.WorldNormal = foundIntersection.normal;
        
        Mesh* mesh = foundIntersection.object->GetComponent<Mesh>();
        
        if (touch.WorldNormal.x == 0 && touch.WorldNormal.y == 0 && touch.WorldNormal.z == 0) {
            
            Vector3 v0 = mesh->Position(mesh->Triangles()[touch.TriangleIndex  ]);
            Vector3 v1 = mesh->Position(mesh->Triangles()[touch.TriangleIndex+1]);
            Vector3 v2 = mesh->Position(mesh->Triangles()[touch.TriangleIndex+2]);
        
            Vector3 tan1 = v1 - v0;
            Vector3 tan2 = v2 - v0;
        
            touch.WorldNormal = tan1.Cross(tan2);
            
            touch.WorldNormal = foundIntersection.object->GetComponent<Transform>()->World().TransformVector(touch.WorldNormal);
            touch.WorldNormal.Normalize();
        } 
    
        list.push_back(touch);
    }
    
    intersections.clear();
}

bool Picker::SortIntersections(const Intersection &a, const Intersection &b) {
    auto ao = a.object->GetComponent<Orderable>();
    auto bo = b.object->GetComponent<Orderable>();
    
    if (ao && bo) {
        return ao->Order()>bo->Order();
    } else {
        return a.distanceToPick<b.distanceToPick;
    }
}
        
