//
//  Rect.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Rect.hpp"
#include "StringHelper.hpp"
#include <vector>

using namespace Pocket;

Rect Rect::Deserialize(const std::string &data, const char delim) {
    std::vector<float> v;
    StringHelper::splitToFloats(data, ',', v);
    return Rect(v.size()>0 ? v[0] : 0,
                v.size()>1 ? v[1] : 0,
                v.size()>2 ? v[2] : 0,
                v.size()>3 ? v[3] : 0);
}