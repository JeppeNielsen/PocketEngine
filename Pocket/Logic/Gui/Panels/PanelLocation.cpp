//
//  PanelLocation.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "PanelLocation.hpp"

using namespace Pocket;

bool PanelLocation::operator !=(const PanelLocation &other) const{
    if (other.directions.size()!=directions.size()) return true;
    for(int i=0; i<directions.size(); ++i) {
        if (directions[i]!=other.directions[i]) return true;
    }
    return false;
}

bool PanelLocation::operator == (const PanelLocation &other) const{
    return !operator!=(other);
}

void PanelLocation::Clear() {
    directions.clear();
}

void PanelLocation::Push(PanelDirection direction) {
    directions.push_back(direction);
}

bool PanelLocation::StartsWith(const PanelLocation& location) const {
    if (location.directions.size()>=directions.size()) return false;
    for(int i=0; i<location.directions.size(); ++i) {
        if (location.directions[i]!=directions[i]) return false;
    }
    return true;
}

void PanelLocation::RemoveSplit(const PanelLocation& location) {
    if (directions.empty()) return;
    if (location.directions.size()>directions.size()) return;
    directions.erase(directions.begin() + location.directions.size() - 1);
}

PanelLocation PanelLocation::Split() const {
    if (directions.empty()) return PanelLocation();
    PanelLocation split = *this;
    split.directions.pop_back();
    return split;
}

std::string PanelLocation::Id() const {
    const char dirIds[] = "TBLR";
    std::string id = "";
    for(auto d : directions) {
        id += dirIds[(int)d];
    }
    return id;
}

Rect PanelLocation::GetRect(Vector2 size, const std::function<float(const std::string&)>& splitFunction) const {
    const char dirIds[] = "TBLR";
    Rect rect(0, size);
    std::string id = "";
    for(auto d : directions) {
        rect = DivideRect(rect, d, splitFunction ? splitFunction(id) : 0.5f);
        id += dirIds[(int)d];
    }
    return rect;
}

Rect PanelLocation::DivideRect(Rect input, PanelDirection direction, float splitPosition) const {
    
    switch (direction) {
        case PanelDirection::Top: {
            input.y += input.height * splitPosition;
            input.height *= (1 - splitPosition);
        }
        break;
        case PanelDirection::Bottom: {
            input.height *= splitPosition;
        }
        break;
        case PanelDirection::Left: {
            input.width *= splitPosition;
        }
        break;
        case PanelDirection::Right: {
            input.x += input.width * splitPosition;
            input.width *= (1 - splitPosition);
        }
        break;

      default:
        break;
    }

    return input;
}

bool PanelLocation::IsHorizontal() const {
    return directions.empty() ? false : (directions.back() == PanelDirection::Right || directions.back() == PanelDirection::Left);
}

void PanelLocation::RecurseLocations(const std::function<void(const PanelLocation&)> callback) const {
    std::vector<PanelDirection> dirs;
    for(int i = 0; i<directions.size(); ++i) {
        dirs.push_back(directions[i]);
        PanelLocation location;
        location.directions = dirs;
        callback(location);
    }
}
