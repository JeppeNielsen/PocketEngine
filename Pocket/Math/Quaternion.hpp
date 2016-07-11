//
//  Quaternion.h
//  Typhoon
//
//  Created by Jeppe Nielsen on 3/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#pragma once//;
#include "Matrix4x4.hpp"

namespace Pocket {
    class Quaternion {
    public:
        float w,x,y,z;
        
        inline Quaternion() { w=1; x=0; y=0; z=0;}
        
        inline Quaternion(float w, float x, float y, float z) {
            this->w = w; this->x = x; this->y = y; this->z = z;
        }
        
        inline Quaternion(const float& angle, const Vector3& axis) {
            this->FromAngleAxis(angle, axis);
        }
        /// Construct a quaternion from 3 orthonormal local axes
        inline Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis) {
            this->FromAxes(xaxis, yaxis, zaxis);
        }
        /// Construct a quaternion from 3 orthonormal local axes
        inline Quaternion(const Vector3* axis) {
            this->FromAxes(axis);
        }
        
        inline Quaternion(const float& rotation2d) {
            this->FromAngleAxis(rotation2d, Vector3(0,0,1));
        }
        
        inline Quaternion(const Vector3& eulerAngles) {
            this->FromEuler(eulerAngles);
        }
        
        inline float& operator [] ( const int index ) {
			//assert( i < 4 );
			return *(&w + index);
		}
        
        inline ~Quaternion() {}
        
        
        
        void FromRotationMatrix (const Matrix4x4& kRot);
        void ToRotationMatrix (Matrix4x4& kRot) const;
        void FromAngleAxis (const float& rfAngle, const Vector3& rkAxis);
        void ToAngleAxis (float& rfAngle, Vector3& rkAxis) const;
        void FromAxes (const Vector3* akAxis);
        void FromAxes (const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
        void ToAxes (Vector3* akAxis) const;
        void ToAxes (Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;
        void FromEuler(const Vector3& eulerAngles);
        Vector3 ToEuler() const;
        
        inline static Quaternion Create(const float& angle, const Vector3& axis) {
            Quaternion q;
            q.FromAngleAxis(angle, axis);
            return q;
        }
        
        /// Get the local x-axis
        Vector3 xAxis(void) const;
        /// Get the local y-axis
        Vector3 yAxis(void) const;
        /// Get the local z-axis
        Vector3 zAxis(void) const;
        
        inline Quaternion& operator= (const Quaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			return *this;
		}
        
        Quaternion operator+ (const Quaternion& rkQ) const;
        Quaternion operator- (const Quaternion& rkQ) const;
        Quaternion operator* (const Quaternion& rkQ) const;
        Quaternion operator* (float fScalar) const;
        
        friend Quaternion operator* (float fScalar,
                                     const Quaternion& rkQ);
        
        
        Quaternion operator- () const;
        inline bool operator== (const Quaternion& rhs) const
		{
			return (rhs.x == x) && (rhs.y == y) &&
            (rhs.z == z) && (rhs.w == w);
		}
        inline bool operator!= (const Quaternion& rhs) const
		{
			return !operator==(rhs);
		}
        // functions of a quaternion
        float Dot (const Quaternion& rkQ) const;  // dot product
        float Norm () const;  // squared-length
        /// Normalises this quaternion, and returns the previous length
        float Normalize(void); 
        Quaternion Inverse () const;  // apply to non-zero quaternion
        Quaternion UnitInverse () const;  // apply to unit-length quaternion
        Quaternion Exp () const;
        Quaternion Log () const;
        
        // rotation of a vector by a quaternion
        Vector3 operator* (const Vector3& rkVector) const;
        
		float GetRoll() const;
		float GetPitch() const;
		float GetYaw() const;
        
		/// Equality with tolerance (tolerance is max angle difference)
		bool Equals(const Quaternion& rhs, const float& tolerance) const;
		
	    // spherical linear interpolation
        static Quaternion Slerp (float fT, const Quaternion& rkP,
                                 const Quaternion& rkQ, bool shortestPath = false);
        
        static Quaternion SlerpExtraSpins (float fT,
                                           const Quaternion& rkP, const Quaternion& rkQ,
                                           int iExtraSpins);
        
        // setup for spherical quadratic interpolation
        static void Intermediate (const Quaternion& rkQ0,
                                  const Quaternion& rkQ1, const Quaternion& rkQ2,
                                  Quaternion& rka, Quaternion& rkB);
        
        // spherical quadratic interpolation
        static Quaternion Squad (float fT, const Quaternion& rkP,
                                 const Quaternion& rkA, const Quaternion& rkB,
                                 const Quaternion& rkQ, bool shortestPath = false);
        
        // normalised linear interpolation - faster but less accurate (non-constant rotation velocity)
        static Quaternion nlerp(float fT, const Quaternion& rkP, 
                                const Quaternion& rkQ, bool shortestPath = false);
        
        static Quaternion LookAt(const Vector3& location, const Vector3& lookAt, const Vector3& up);
        
        // cutoff for sine near zero
        static const float ms_fEpsilon;
        
        // special values
        static const Quaternion ZERO;
        static const Quaternion IDENTITY;
                
		/// Check whether this quaternion contains valid values
		//inline bool IsNaN() const
		//{
		//	return isNaN(x) || isNaN(y) || isNaN(z) || isNaN(w);
		//}
        
        operator float() const {
            return GetRoll();
        }
    
        Quaternion static Deserialize(const std::string& data);
        
        friend std::ostream& operator<<(std::ostream& stream, const Pocket::Quaternion& q) {
            stream<<q.w<<','<<q.x<<","<<q.y<<","<<q.z;
            return stream;
        }
    };
}