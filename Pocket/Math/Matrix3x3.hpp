#pragma once
#include "Vector2.hpp"

namespace Pocket {

	class Matrix3x3
	{
		public:
			
			static Matrix3x3 CreateIdentity(); 
			static Matrix3x3 CreateScale(float sx, float sy);
			static Matrix3x3 CreateScale(const Vector2& scale);
			static Matrix3x3 CreateRotation(float angle);
			static Matrix3x3 CreateTranslation(float x, float y);
			static Matrix3x3 CreateTranslation(const Vector2& position);

			void Scale(float sx, float sy);
			void Rotation(float angle);
			void Translation(float x, float y);
			void Translation(const Vector2& v);

			Matrix3x3();
			~Matrix3x3();

			Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
		    Matrix3x3(const Matrix3x3& mat);
			void Set(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
			
			void Identity();
			void Mult(const Matrix3x3& a, const Matrix3x3& b);
			
			void GetRow(const int r, float& x, float& y, float& z) const;
			float Determinant() const;

			Matrix3x3 operator*(const Matrix3x3& mat) const;
			Matrix3x3& operator*=(const Matrix3x3& mat);

			Matrix3x3& Invert();
			Vector2& TransformPoint(const Vector2& source) const;
			void TransformPointRef(Vector2& source) const;
			Vector2& TransformVector(const Vector2& source) const;
			void TransformVectorRef(Vector2& source) const;
			void TransformPointRef2(Vector2& dest, const Vector2& source) const;

			bool operator==(const Matrix3x3 &other);
			bool operator!=(const Matrix3x3 &other);

			void GetGlMatrix(float* mat);

			Vector2& Position() const;
			float Angle() const;

		public:

				float m[3][3];

				

	};

}