#include "Plane.hpp"
#include "Ray.hpp"

using namespace Pocket;

Plane::Plane()
{
}

Plane::Plane(const Vector3& normal, float d) : normal(normal), d(d)
{
}

Plane::~Plane()
{
}

Plane::Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
    Vector3 p01 = p0 - p1;
    Vector3 p02 = p0 - p2;
    normal = p01.Cross(p02);
    d = normal.Dot(p0);
}

Plane::Plane(const Vector3& normal, const Vector3& position) {
    this->normal = normal;
    d = position.Dot(normal);
}

float Plane::Distance(const Vector3& position) const {
	return position.x * normal.x + position.y * normal.y + position.z * normal.z + d;
}

void Plane::Normalize() {
	float lengthInv = 1.0f / normal.Length();
	normal.x *= lengthInv;
	normal.y *= lengthInv;
	normal.z *= lengthInv;
	d *= lengthInv;
}

void Plane::Flip() {
	normal.x = -normal.x;
	normal.y = -normal.y;
	normal.z = -normal.z;
	d=-d;
}

bool Plane::IntersectsRay(const Pocket::Ray &ray, float *distance) const {
    float dot = normal.Dot(ray.direction);
    if (dot>0.0001f) return false;
    *distance = (d - normal.Dot(ray.position)) / dot;
    return true;
}


