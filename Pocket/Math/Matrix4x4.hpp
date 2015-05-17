//
//  Matrix4x4.h
//  Components
//
//  Created by Jeppe Nielsen on 3/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#pragma once//;
#include "Vector3.hpp"

namespace Pocket {
    
    class Quaternion;
    
    class Matrix4x4 {
    public:
        
        inline Matrix4x4() {}
        inline ~Matrix4x4() {}
        
        inline Matrix4x4(
                         float m00, float m01, float m02,
                         float m10, float m11, float m12,
                         float m20, float m21, float m22
                         )
        {
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = 0;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = 0;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = 0;
            m[3][0] = 0;
            m[3][1] = 0;
            m[3][2] = 0;
            m[3][3] = 1;
        }

        inline Matrix4x4(
                         float m00, float m01, float m02, float m03,
                         float m10, float m11, float m12, float m13,
                         float m20, float m21, float m22, float m23,
                         float m30, float m31, float m32, float m33 )
        {
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;
            m[3][0] = m30;
            m[3][1] = m31;
            m[3][2] = m32;
            m[3][3] = m33;
        }
        
        inline Matrix4x4(const Matrix4x4& other) {
            m[0][0] = other.m[0][0];
            m[0][1] = other.m[0][1];
            m[0][2] = other.m[0][2];
            m[0][3] = other.m[0][3];
            m[1][0] = other.m[1][0];
            m[1][1] = other.m[1][1];
            m[1][2] = other.m[1][2];
            m[1][3] = other.m[1][3];
            m[2][0] = other.m[2][0];
            m[2][1] = other.m[2][1];
            m[2][2] = other.m[2][2];
            m[2][3] = other.m[2][3];
            m[3][0] = other.m[3][0];
            m[3][1] = other.m[3][1];
            m[3][2] = other.m[3][2];
            m[3][3] = other.m[3][3];
        }
        
        inline void GetRow(const int r, float& x, float& y, float& z, float& w) const	{ 
            x = m[r][0];	
            y = m[r][1];	
            z = m[r][2];	
            w = m[r][3];
        }
        
        inline Matrix4x4 Multiply(const Matrix4x4 &m2) const
        {
            Matrix4x4 r;
            r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
            r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
            r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
            r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];
            
            r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
            r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
            r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
            r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];
            
            r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
            r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
            r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
            r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];
            
            r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
            r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
            r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
            r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];
            
            return r;
        }
        
        inline Matrix4x4 operator * ( const Matrix4x4 &m2 ) const
        {
            return Multiply( m2 );
        }
        
        inline Matrix4x4 operator *= ( const Matrix4x4 &m2 )
        {
            float x,y,z,w;
            GetRow(0, x,y,z,w);
            m[0][0] = x * m2.m[0][0] + y * m2.m[1][0] + z * m2.m[2][0] + w * m2.m[3][0];
            m[0][1] = x * m2.m[0][1] + y * m2.m[1][1] + z * m2.m[2][1] + w * m2.m[3][1];
            m[0][2] = x * m2.m[0][2] + y * m2.m[1][2] + z * m2.m[2][2] + w * m2.m[3][2];
            m[0][3] = x * m2.m[0][3] + y * m2.m[1][3] + z * m2.m[2][3] + w * m2.m[3][3];
            
            GetRow(1, x,y,z,w);
            m[1][0] = x * m2.m[0][0] + y * m2.m[1][0] + z * m2.m[2][0] + w * m2.m[3][0];
            m[1][1] = x * m2.m[0][1] + y * m2.m[1][1] + z * m2.m[2][1] + w * m2.m[3][1];
            m[1][2] = x * m2.m[0][2] + y * m2.m[1][2] + z * m2.m[2][2] + w * m2.m[3][2];
            m[1][3] = x * m2.m[0][3] + y * m2.m[1][3] + z * m2.m[2][3] + w * m2.m[3][3];
            
            GetRow(2, x,y,z,w);
            m[2][0] = x * m2.m[0][0] + y * m2.m[1][0] + z * m2.m[2][0] + w * m2.m[3][0];
            m[2][1] = x * m2.m[0][1] + y * m2.m[1][1] + z * m2.m[2][1] + w * m2.m[3][1];
            m[2][2] = x * m2.m[0][2] + y * m2.m[1][2] + z * m2.m[2][2] + w * m2.m[3][2];
            m[2][3] = x * m2.m[0][3] + y * m2.m[1][3] + z * m2.m[2][3] + w * m2.m[3][3];
            
            GetRow(3, x,y,z,w);
            m[3][0] = x * m2.m[0][0] + y * m2.m[1][0] + z * m2.m[2][0] + w * m2.m[3][0];
            m[3][1] = x * m2.m[0][1] + y * m2.m[1][1] + z * m2.m[2][1] + w * m2.m[3][1];
            m[3][2] = x * m2.m[0][2] + y * m2.m[1][2] + z * m2.m[2][2] + w * m2.m[3][2];
            m[3][3] = x * m2.m[0][3] + y * m2.m[1][3] + z * m2.m[2][3] + w * m2.m[3][3];
            
            return *this;
        }
        
        inline bool operator == ( const Matrix4x4& m2 ) const
        {
            if( 
               m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
               m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
               m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
               m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
                return false;
            return true;
        }
        
        /** Tests 2 matrices for inequality.
         */
        inline bool operator != ( const Matrix4x4& m2 ) const
        {
            if( 
               m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
               m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
               m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
               m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
                return true;
            return false;
        }
        
        inline Matrix4x4 Transpose(void) const
        {
            return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                             m[0][1], m[1][1], m[2][1], m[3][1],
                             m[0][2], m[1][2], m[2][2], m[3][2],
                             m[0][3], m[1][3], m[2][3], m[3][3]);
        }
        
        
        inline void SetTranslation( const Vector3& v )
        {
            m[0][3] = v.x;
            m[1][3] = v.y;
            m[2][3] = v.z;
        }
        
        inline Vector3 Translation() const
        {
            return Vector3(m[0][3], m[1][3], m[2][3]);
        }
        
        inline void InitTranslation( const Vector3& v )
        {
            m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
            m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
            m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
            m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
        }
        
        inline static Matrix4x4 CreateTranslation( const Vector3& v )
        {
            Matrix4x4 r;
            r.InitTranslation(v);
            return r;
        }
        
        inline void SetScale( const Vector3& v )
        {
            m[0][0] = v.x;
            m[1][1] = v.y;
            m[2][2] = v.z;
        }
        
        inline Vector3 Scale() const
        {
            return Vector3(m[0][0],m[1][1],m[2][2]);
        }
        
        inline Vector3 RealScale() const
        {
            return Vector3(
                           sqrtf(m[0][0]*m[0][0]+m[0][1]*m[0][1]+m[0][2]*m[0][2]),
                           sqrtf(m[1][0]*m[1][0]+m[1][1]*m[1][1]+m[1][2]*m[1][2]),
                           sqrtf(m[2][0]*m[2][0]+m[2][1]*m[2][1]+m[2][2]*m[2][2]));
        }
        
        inline void InitScale( const Vector3& v )
        {
            m[0][0] = v.x; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
            m[1][0] = 0.0; m[1][1] = v.y; m[1][2] = 0.0; m[1][3] = 0.0;
            m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = v.z; m[2][3] = 0.0;
            m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
        }
        
        inline static Matrix4x4 CreateScale( const Vector3& v )
        {
            Matrix4x4 r;
            r.m[0][0] = v.x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
            r.m[1][0] = 0.0; r.m[1][1] = v.y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
            r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = v.z; r.m[2][3] = 0.0;
            r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;
            
            return r;
        }
        
        inline bool IsAffine() const
        {
			if (m[3][0]!=0) return false;
			if (m[3][1]!=0) return false;
			if (m[3][2]!=0) return false;
			if (m[3][3]!=1) return false;
			return true;
            //return (m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1);
        }
        
        inline Vector3 TransformPosition(const Vector3& position) const {
            Vector3 r;
            
            float fInvW = 1.0f / ( m[3][0] * position.x + m[3][1] * position.y + m[3][2] * position.z + m[3][3] );
            
            r.x = ( m[0][0] * position.x + m[0][1] * position.y + m[0][2] * position.z + m[0][3] ) * fInvW;
            r.y = ( m[1][0] * position.x + m[1][1] * position.y + m[1][2] * position.z + m[1][3] ) * fInvW;
            r.z = ( m[2][0] * position.x + m[2][1] * position.y + m[2][2] * position.z + m[2][3] ) * fInvW;
            
            return r;
        }

		inline Vector3 TransformPositionAffine(const Vector3& position) const {
			Vector3 r;

			r.x = ( m[0][0] * position.x + m[0][1] * position.y + m[0][2] * position.z + m[0][3] );
			r.y = ( m[1][0] * position.x + m[1][1] * position.y + m[1][2] * position.z + m[1][3] );
			r.z = ( m[2][0] * position.x + m[2][1] * position.y + m[2][2] * position.z + m[2][3] );

			return r;
		}
        
        inline Vector3 TransformVector(const Vector3& vector) const {
            Vector3 r;
            
            float fInvW = 1.0f / ( m[3][0] * vector.x + m[3][1] * vector.y + m[3][2] * vector.z + m[3][3] );
            
            r.x = ( m[0][0] * vector.x + m[0][1] * vector.y + m[0][2] * vector.z) * fInvW;
            r.y = ( m[1][0] * vector.x + m[1][1] * vector.y + m[1][2] * vector.z) * fInvW;
            r.z = ( m[2][0] * vector.x + m[2][1] * vector.y + m[2][2] * vector.z) * fInvW;
            
            return r;
        }

		inline Vector3 TransformVectorAffine(const Vector3& vector) const {
			Vector3 r;

			r.x = ( m[0][0] * vector.x + m[0][1] * vector.y + m[0][2] * vector.z);
			r.y = ( m[1][0] * vector.x + m[1][1] * vector.y + m[1][2] * vector.z);
			r.z = ( m[2][0] * vector.x + m[2][1] * vector.y + m[2][2] * vector.z);

			return r;
		}
        
        inline void TransformPosition(const Vector3& position, Vector3& out) const {
            float fInvW = 1.0f / ( m[3][0] * position.x + m[3][1] * position.y + m[3][2] * position.z + m[3][3] );
            out.x = ( m[0][0] * position.x + m[0][1] * position.y + m[0][2] * position.z + m[0][3] ) * fInvW;
            out.y = ( m[1][0] * position.x + m[1][1] * position.y + m[1][2] * position.z + m[1][3] ) * fInvW;
            out.z = ( m[2][0] * position.x + m[2][1] * position.y + m[2][2] * position.z + m[2][3] ) * fInvW;
        }
        
        inline void TransformPositionAffine(const Vector3& position, Vector3& out) const {
            out.x = ( m[0][0] * position.x + m[0][1] * position.y + m[0][2] * position.z + m[0][3] );
            out.y = ( m[1][0] * position.x + m[1][1] * position.y + m[1][2] * position.z + m[1][3] );
            out.z = ( m[2][0] * position.x + m[2][1] * position.y + m[2][2] * position.z + m[2][3] );
        }
        
        inline void TransformVector(const Vector3& vector, Vector3& out) const {
            float fInvW = 1.0f / ( m[3][0] * vector.x + m[3][1] * vector.y + m[3][2] * vector.z + m[3][3] );
            
            out.x = ( m[0][0] * vector.x + m[0][1] * vector.y + m[0][2] * vector.z) * fInvW;
            out.y = ( m[1][0] * vector.x + m[1][1] * vector.y + m[1][2] * vector.z) * fInvW;
            out.z = ( m[2][0] * vector.x + m[2][1] * vector.y + m[2][2] * vector.z) * fInvW;
        }
        
        inline void TransformVectorAffine(const Vector3& vector, Vector3& out) const {
            out.x = ( m[0][0] * vector.x + m[0][1] * vector.y + m[0][2] * vector.z);
            out.y = ( m[1][0] * vector.x + m[1][1] * vector.y + m[1][2] * vector.z);
            out.z = ( m[2][0] * vector.x + m[2][1] * vector.y + m[2][2] * vector.z);
        }
                
        inline float* operator[] (int iRow) const
		{
			return (float*)m[iRow];
		}
        
        
        Matrix4x4 Invert() const;
        
        Matrix4x4 InvertAffine() const;
        
        void InitTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);
        
        inline static Matrix4x4 CreateTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation) {
            Matrix4x4 m;
            m.InitTransform(position, scale, orientation);
            return m;
        }
        
        void ToAxisAngle (Vector3& axis, float& angle) const;
        void FromAxisAngle (const Vector3& axis, const float& angle);
        
        // The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
        // where yaw is rotation about the Up vector, pitch is rotation about the
        // Right axis, and roll is rotation about the Direction axis.
        bool ToEulerAnglesXYZ (float& rfYAngle, float& rfPAngle,
                               float& rfRAngle) const;
        bool ToEulerAnglesXZY (float& rfYAngle, float& rfPAngle,
                               float& rfRAngle) const;
        bool ToEulerAnglesYXZ (float& rfYAngle, float& rfPAngle,
                               float& rfRAngle) const;
        bool ToEulerAnglesYZX (float& rfYAngle, float& rfPAngle,
                               float& rfRAngle) const;
        bool ToEulerAnglesZXY (float& rfYAngle, float& rfPAngle,
                               float& rfRAngle) const;
        bool ToEulerAnglesZYX (float& rfYAngle, float& rfPAngle,
                               float& rfRAngle) const;
        void FromEulerAnglesXYZ (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesXZY (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesYXZ (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesYZX (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesZXY (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        void FromEulerAnglesZYX (const float& fYAngle, const float& fPAngle, const float& fRAngle);
        
        static Matrix4x4 Lerp(const Matrix4x4& source, const Matrix4x4& dest, const float& t);
        static Matrix4x4 Add(const Matrix4x4& a, const Matrix4x4& b);
        
        inline void InitYawPitchRoll(const float& yaw, const float& pitch, const float& roll) {
            FromEulerAnglesYXZ(yaw, pitch, roll);
        }
        
        inline static Matrix4x4 CreateRotation( const Vector3& axis, const float& angle ) {
            Matrix4x4 r;
            r.InitScale(1);
            r.FromAxisAngle(axis, angle);
            return r;
        }
        
        inline static Matrix4x4 CreateRotation( const float& yaw, const float& pitch, const float& roll ) {
            Matrix4x4 r;
            r.InitYawPitchRoll(yaw, pitch, roll);
            return r;
        }
        
        inline void InitPitch(float pitch) {
            float fCos = cosf(pitch);
            float fSin = sinf(pitch);
            
            m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0;
            m[1][0] = 0.0; m[1][1] = fCos; m[1][2] = -fSin; m[1][3] = 0;
            m[2][0] = 0.0; m[2][1] = fSin; m[2][2] = fCos; m[2][3] = 0;
            m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
        }
        
        inline static Matrix4x4 CreatePitch( const float& pitch ) {
            Matrix4x4 r;
            r.InitPitch(pitch);
            return r;
        }
        
        inline void InitYaw(float yaw) {
            float fCos = cosf(yaw);
            float fSin = sinf(yaw);
            
            m[0][0] = fCos; m[0][1] = 0.0; m[0][2] = fSin; m[0][3] = 0;
            m[1][0] = 0.0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
            m[2][0] = -fSin; m[2][1] = 0; m[2][2] = fCos; m[2][3] = 0;
            m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
        }
        
        inline static Matrix4x4 CreateYaw( const float& yaw ) {
            Matrix4x4 r;
            r.InitYaw(yaw);
            return r;
        }
        
        inline void InitRoll(float roll) {
            float fCos = cosf(roll);
            float fSin = sinf(roll);
            
            m[0][0] = fCos; m[0][1] = -fSin; m[0][2] = 0.0; m[0][3] = 0;
            m[1][0] = fSin; m[1][1] = fCos; m[1][2] = 0.0; m[1][3] = 0;
            m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0;
            m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
        }
        
        inline static Matrix4x4 CreateRoll( const float& roll ) {
            Matrix4x4 r;
            r.InitRoll(roll);
            return r;
        }
        
        inline float* GetGlMatrix() {
            return _m;
        }
        
		static const Matrix4x4 IDENTITY;

		inline static Matrix4x4 CreatePerspective(float fov, float aspect, float znear, float zfar) {
			Matrix4x4 p;
			p.InitPerspective(fov, aspect, znear, zfar);
			return p;
		}

		inline void InitPerspective_old(float fov, float aspect, float znear, float zfar)
		{
		  float xymax = znear * tanf(fov * (3.14159265f / 360.0f));
		  float ymin = -xymax;
		  float xmin = -xymax;

		  float width = xymax - xmin;
		  float height = xymax - ymin;

		  float depth = zfar - znear;
		  float q = -(zfar + znear) / depth;
		  float qn = -2 * (zfar * znear) / depth;

		  float w = 2 * znear / width;
		  w = w / aspect;
		  float h = 2 * znear / height;

		  _m[0]  = w;
		  _m[1]  = 0;
		  _m[2]  = 0;
		  _m[3]  = 0;

		  _m[4]  = 0;
		  _m[5]  = h;
		  _m[6]  = 0;
		  _m[7]  = 0;

		  _m[8]  = 0;
		  _m[9]  = 0;
		  _m[10] = q;
		  _m[11] = -1;

		  _m[12] = 0;
		  _m[13] = 0;
		  _m[14] = qn;
		  _m[15] = 0;
		}

		inline void InitPerspective(float fov, float aspect, float znear, float zfar)
		{
			float xymax = znear * tanf(fov * (3.14159265f / 360.0f));
			float ymin = -xymax;
			float xmin = -xymax;

			float width = xymax - xmin;
			float height = xymax - ymin;

			float depth = zfar - znear;
			float q = -(zfar + znear) / depth;
			float qn = -2 * (zfar * znear) / depth;

			float w = 2 * znear / width;
			w = w / aspect; 
			float h = 2 * znear / height;

            
            
			_m[0]  = w;
			_m[1]  = 0;
			_m[2]  = 0;
			_m[3]  = 0;

			_m[4]  = 0;
			_m[5]  = h;
			_m[6]  = 0;
			_m[7]  = 0;

			_m[8]  = 0;
			_m[9]  = 0;
			_m[10] = q;
			_m[11] = qn;

			_m[12] = 0;
			_m[13] = 0;
			_m[14] = -1;
			_m[15] = 0;
		}

        inline void InitOrthographic(float left, float top, float right, float bottom, float nearValue, float farValue)
		{
            
		  /*
			m[0][0]  = 2.0f/(right-left);
			m[1][0]  = 0;
			m[2][0]  = 0;
			m[3][0]  = 0;
            
			m[0][1]  = 0;
			m[1][1]  = 2.0f/(top-bottom);
			m[2][1]  = 0;
			m[3][1]  = 0;
            
			m[0][2]  = 0;
			m[1][2]  = 0;
			m[2][2]  = -2.0f/(far-near);
			m[3][2]  = 0;
            
			m[0][3]  = -((right+left)/(right-left));
			m[1][3]  = -((top+bottom)/(top-bottom));
			m[2][3]  = -((far+near)/(far-near));
			m[3][3]  = 1;
            */
            

             _m[0] = 2.0f/(right-left);
             _m[1] = 0;
             _m[2] = 0;
             _m[3] = -((right+left)/(right-left));
             
             _m[4]  = 0;
             _m[5]  = 2.0f/(top-bottom);
             _m[6]  = 0;
             _m[7]  = -((top+bottom)/(top-bottom));
             
             _m[8]  = 0;
             _m[9]  = 0;
             _m[10] = -2.0f/(farValue-nearValue);
             _m[11] = -((farValue+nearValue)/(farValue-nearValue));
             
             _m[12] = 0;
             _m[13] = 0;
             _m[14] = 0;
             _m[15] = 1;
             
		}
        
        
        
    public:
        
        //[row][col].
        union {
            float m[4][4];
            float _m[16];
        };
        
        Matrix4x4 static Deserialize(const std::string& data);
    };
}

std::ostream& operator<<(std::ostream& stream, const Pocket::Matrix4x4& m);