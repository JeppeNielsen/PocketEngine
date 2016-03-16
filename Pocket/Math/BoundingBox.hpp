#pragma once
#include "Vector3.hpp"

namespace Pocket {
	class Matrix4x4;
	class BoundingBox
	{
	public:
		BoundingBox();
		BoundingBox(const BoundingBox& other);
		BoundingBox(const Vector3& center, const Vector3& extends);
		~BoundingBox();
		
		bool Intersects(const BoundingBox& other) const;
		bool Contains(const BoundingBox& other) const;
		float Volume() const;

		Vector3 center;
		Vector3 extends;

		bool operator==(const BoundingBox &other);
		bool operator!=(const BoundingBox &other);

		void CreateWorldAligned(const Matrix4x4& matrix, BoundingBox& boundingBox) const;

	};
}

