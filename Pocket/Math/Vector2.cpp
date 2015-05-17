#include "Vector2.hpp"
#include "MathHelper.hpp"
#include "StringHelper.hpp"
#include "Vector3.hpp"

using namespace Pocket;

Vector2 Vector2::Bezier(const Vector2& position0, const Vector2& tangent0, const Vector2& position1, const Vector2& tangent1, const float& t) {
    return Vector2(
        MathHelper::Bezier(position0.x, tangent0.x, position1.x, tangent1.x, t),
        MathHelper::Bezier(position0.y, tangent0.y, position1.y, tangent1.y, t)
    );
}

Vector2 Vector2::Deserialize(const std::string &data, const char delim) {
    std::vector<float> v;
    StringHelper::splitToFloats(data, delim, v);
    return Vector2(v.size()>0 ? v[0] : 0, v.size()>1 ? v[1] : 0);
}

Vector2::Vector2(const Vector3& other) {
    x = other.x;
    y = other.y;
}

std::ostream& operator<<(std::ostream& stream, const Vector2& v) {
    stream << v.x <<","<<v.y;
    return stream;
}