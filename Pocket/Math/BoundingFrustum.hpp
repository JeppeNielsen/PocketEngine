#pragma once
#include "Matrix4x4.hpp"
#include "Plane.hpp"
#include "BoundingBox.hpp"

namespace Pocket {
	class BoundingFrustum
	{
	public:
		BoundingFrustum();
		~BoundingFrustum();

		void SetFromViewProjection(const Matrix4x4& viewProjection);


		enum Intersection {
			INTERSECT = 0,
			INSIDE = 1,
			OUTSIDE = 2,
		};

		static int Count;

		Intersection Intersect(const BoundingBox& box) const;

		Plane planes[6];
	};
}

