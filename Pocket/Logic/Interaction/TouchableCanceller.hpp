//
//  TouchableCanceller.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/11/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
namespace Pocket {
    struct TouchableCanceller {
        TouchableCanceller() : MovementToCancel(10.0f), trackTouchPosition(false)  {}
        float MovementToCancel;
        bool trackTouchPosition;
    };
}