//
//  Orderable.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/29/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Orderable.hpp"

using namespace Pocket;

Orderable::Orderable() : Order(this) {}
void Orderable::Reset() { Order = 0; }

void Orderable::Clone(const Orderable& source) {
    Order = source.Order;
}
