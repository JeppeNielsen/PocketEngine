#pragma once
#include <sstream>
#include <iostream>
#include "Vector2.hpp"
#include "Matrix3x3.hpp"

namespace Pocket {
	class Box {

		public:
			Box();
			~Box();
			Box(const Box &source);
			Box(float left, float top, float right, float bottom);
			

			bool Intersect(const Box& other) const;
			bool Inside(const Vector2& position) const;
			void FromMatrix(const Matrix3x3& matrix);
			void FromMatrix(const Vector2& topLeft, const Vector2& bottomRight, const Matrix3x3& matrix);


			float left;
			float top;
			float right;
			float bottom;

			bool operator==(const Box &other) const;
			bool operator!=(const Box &other) const;

			Vector2 Interpolate(const Vector2& p) const; 

			Vector2 Position() const;
			Vector2 Size() const;
            Vector2 Center() const;
            float Aspect() const;
        
            static Box Deserialize(const std::string &data);

	private:
		static Vector2 corners[4];
        
        friend std::ostream& operator<<(std::ostream& stream, const Pocket::Box& v) {
            stream << v.left <<","<<v.top<<","<<v.right<<","<<v.bottom;
            return stream;
        }
	};
}

