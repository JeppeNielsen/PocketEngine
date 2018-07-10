//
//  PanelLocation.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <functional>
#include "Rect.hpp"

namespace Pocket {
    class Panel;

    enum class PanelDirection { Top, Bottom, Left, Right };

    class PanelLocation {
        std::vector<PanelDirection> directions;
    public:
        
        bool operator !=(const PanelLocation &other) const;
        bool operator == (const PanelLocation &other) const;
        void Clear();
        void Push(PanelDirection direction);
        
        bool StartsWith(const PanelLocation& location) const;
        void RemoveSplit(const PanelLocation& location);
        PanelLocation Split() const;
        std::string Id() const;
        
        Rect GetRect(Vector2 size, const std::function<float(const std::string&)>& splitFunction) const;
        Rect DivideRect(Rect input, PanelDirection direction, float splitPosition) const;
        bool IsHorizontal() const;
        void RecurseLocations(const std::function<void(const PanelLocation&)> callback) const;
    };
}
