//
//  Layoutable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/19/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "TypeInfo.hpp"
#include "Property.hpp"
#include "Vector2.hpp"

namespace Pocket {
    class Layoutable {
    public:
    Layoutable();
    enum class HAlignment { None, Left, Center, Right, Relative };
    enum class VAlignment { None, Top, Center, Bottom, Relative };
    
    Property<HAlignment> HorizontalAlignment;
    Property<VAlignment> VerticalAlignment;
    
    enum class ChildLayouting {
        None,
        HorizontalStackedFit,
        HorizontalStackedLeft,
        HorizontalStackedCenter,
        HorizontalStackedRight,
        HorizontalCentered, HorizontalEvenlySized,
        VerticalStackedFit,
        VerticalStackedTop,
        VerticalStackedCenter,
        VerticalStackedBottom,
        VerticalCentered, VerticalEvenlySized,
    };
    
    Property<ChildLayouting> ChildLayouting;
    
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(HorizontalAlignment)
    TYPE_FIELD(VerticalAlignment)
    TYPE_FIELD(ChildLayouting)
    TYPE_FIELDS_END
    };
}
