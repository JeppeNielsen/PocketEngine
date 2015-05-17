#include "BoundingBox.hpp"
#include "Matrix4x4.hpp"

using namespace Pocket;

BoundingBox::BoundingBox() {
}

BoundingBox::~BoundingBox()
{
}

BoundingBox::BoundingBox(const BoundingBox& other) {
	center = other.center;
	extends = other.extends;
}

BoundingBox::BoundingBox(const Vector3& center, const Vector3& extends) {
	this->center = center;
	this->extends = extends;
}

bool BoundingBox::Intersects(const BoundingBox& other) const {
	const Vector3 v = other.center - center;
	if (fabs(v.x)>(extends.x + other.extends.x)) return false;
	if (fabs(v.y)>(extends.y + other.extends.y)) return false;
	if (fabs(v.z)>(extends.z + other.extends.z)) return false;
	return true;
}

float BoundingBox::Volume() const {
	return extends.x * extends.y * extends.z;
}

bool BoundingBox::operator ==(const BoundingBox &other) {
	if (extends!=other.extends) return false;
	if (center!=other.center) return false;
	return true;
}

bool BoundingBox::operator !=(const BoundingBox &other) {
	if (extends!=other.extends) return true;
	if (center!=other.center) return true;
	return false;
}

bool BoundingBox::Contains(const BoundingBox& other) const {
	
	Vector3 min1 = center - extends * 0.5f;
	Vector3 max1 = center + extends * 0.5f;

	Vector3 min2 = other.center - other.extends * 0.5f;
	Vector3 max2 = other.center + other.extends * 0.5f;

	if (min2.x<min1.x) return false;
	if (max2.x>max1.x) return false;

	if (min2.y<min1.y) return false;
	if (max2.y>max1.y) return false;

	if (min2.z<min1.z) return false;
	if (max2.z>max1.z) return false;

	return true;
};

void BoundingBox::CreateWorldAligned(const Matrix4x4& matrix, BoundingBox& boundingBox) {
	Vector3 halfExtends = extends * 0.5f;

	boundingBox.center = matrix.TransformPosition(center);
    
    //float fInvW = 1.0f / ( fabsf(matrix.m[3][0]) * halfExtends.x + fabsf(matrix.m[3][1]) * halfExtends.y + fabsf(matrix.m[3][2]) * halfExtends.z + fabsf(matrix.m[3][3]) );
    
    Vector3 extends;

	extends.x = (fabsf(matrix.m[0][0]) * halfExtends.x + fabsf(matrix.m[0][1]) * halfExtends.y + fabsf(matrix.m[0][2]) * halfExtends.z);// * fInvW;
	extends.y = (fabsf(matrix.m[1][0]) * halfExtends.x + fabsf(matrix.m[1][1]) * halfExtends.y + fabsf(matrix.m[1][2]) * halfExtends.z);// * fInvW;
	extends.z = (fabsf(matrix.m[2][0]) * halfExtends.x + fabsf(matrix.m[2][1]) * halfExtends.y + fabsf(matrix.m[2][2]) * halfExtends.z);// * fInvW;

	boundingBox.extends = extends * 2.0f;
}