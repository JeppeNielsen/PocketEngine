//
//  Layouter.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/12/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

namespace Pocket {
    struct Layouter {
        enum class LayoutMode {
            Horizontal,
            Vertical,
        };

        Property<LayoutMode> ChildrenLayoutMode;
        
        Property<Vector2> Min;
        Property<Vector2> Max;
        Property<Vector2> Desired;
        
        DirtyProperty<Vector2> GlobalMin;
        DirtyProperty<Vector2> GlobalMax;
        DirtyProperty<Vector2> GlobalDesired;
        
        void SetAll(Vector2 size) {
            Min = size;
            Max = size;
            Desired = size;
        }
    };
}