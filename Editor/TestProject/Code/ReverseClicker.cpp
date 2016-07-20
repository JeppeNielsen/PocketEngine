#pragma once
#include "GameSystem.hpp"
#include "Touchable.hpp"
#include "Turner.hpp"

using namespace Pocket;

struct ReverseClicker { };

struct ReverseClickerSystem : public GameSystem<Turner, Touchable, ReverseClicker> {

    void ObjectAdded(GameObject* object) {
        object->GetComponent<Touchable>()->Click.Bind(this, &ReverseClickerSystem::Clicked, object);
    }

    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Touchable>()->Click.Unbind(this, &ReverseClickerSystem::Clicked, object);
    }

    void Clicked(TouchData data, GameObject* object) {
        object->GetComponent<Turner>()->reverse = !object->GetComponent<Turner>()->reverse;
    }
};
