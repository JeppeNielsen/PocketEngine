#pragma once
#include "Vector3.hpp"

namespace Pocket {
    class Ray;
	class Plane
	{
	public:
		Plane();
		Plane(const Vector3& normal, float d);
        Plane(const Vector3& normal, const Vector3& position);
        Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2);
		~Plane();

		float Distance(const Vector3& position) const;

		void Normalize();
		void Flip();
    
        bool IntersectsRay(const Ray& ray, float* distance) const;

		Vector3 normal;
		float d;
	};
}

