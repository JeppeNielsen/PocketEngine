//
//  Vector3.cpp
//  Components
//
//  Created by Jeppe Nielsen on 3/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Vector3.hpp"
#include "MathHelper.hpp"
#include "StringHelper.hpp"

using namespace Pocket;

Vector3 Vector3::Bezier(const Vector3& position0, const Vector3& tangent0, const Vector3& position1, const Vector3& tangent1, const float& t) {
    return Vector3(
        MathHelper::Bezier(position0.x, tangent0.x, position1.x, tangent1.x, t),
        MathHelper::Bezier(position0.y, tangent0.y, position1.y, tangent1.y, t),
        MathHelper::Bezier(position0.z, tangent0.z, position1.z, tangent1.z, t)
    );
}



Vector3 Vector3::Deserialize(const std::string &data, const char delim) {
    std::vector<float> v;
    StringHelper::splitToFloats(data, delim, v);
    return Vector3(v.size()>0 ? v[0] : 0, v.size()>1 ? v[1] : 0, v.size()>2 ? v[2] : 0);
}

std::ostream& operator<<(std::ostream& stream, const Vector3& v) {
    stream << v.x <<","<<v.y<<","<<v.z;
    return stream;
}

Vector3 Vector3::zero(0,0,0);