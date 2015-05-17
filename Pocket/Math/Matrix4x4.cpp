//
//  Matrix4x4.cpp
//  Components
//
//  Created by Jeppe Nielsen on 3/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Matrix4x4.hpp"
#include "Quaternion.hpp"
#include "MathHelper.hpp"
#include "StringHelper.hpp"

using namespace Pocket;

Matrix4x4 Matrix4x4::Invert() const {
    
    float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
    float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
    float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
    float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];
    
    float v0 = m20 * m31 - m21 * m30;
    float v1 = m20 * m32 - m22 * m30;
    float v2 = m20 * m33 - m23 * m30;
    float v3 = m21 * m32 - m22 * m31;
    float v4 = m21 * m33 - m23 * m31;
    float v5 = m22 * m33 - m23 * m32;
    
    float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
    float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
    float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
    float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);
    
    float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);
    
    float d00 = t00 * invDet;
    float d10 = t10 * invDet;
    float d20 = t20 * invDet;
    float d30 = t30 * invDet;
    
    float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
    
    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;
    
    float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
    
    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;
    
    float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
       
    return Matrix4x4(
                   d00, d01, d02, d03,
                   d10, d11, d12, d13,
                   d20, d21, d22, d23,
                   d30, d31, d32, d33);
}



Matrix4x4 Matrix4x4::InvertAffine() const
{
    //assert(IsAffine());
    
    float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2];
    float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];
    
    float t00 = m22 * m11 - m21 * m12;
    float t10 = m20 * m12 - m22 * m10;
    float t20 = m21 * m10 - m20 * m11;
    
    float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2];
    
    float invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);
    
    t00 *= invDet; t10 *= invDet; t20 *= invDet;
    
    m00 *= invDet; m01 *= invDet; m02 *= invDet;
    
    float r00 = t00;
    float r01 = m02 * m21 - m01 * m22;
    float r02 = m01 * m12 - m02 * m11;
    
    float r10 = t10;
    float r11 = m00 * m22 - m02 * m20;
    float r12 = m02 * m10 - m00 * m12;
    
    float r20 = t20;
    float r21 = m01 * m20 - m00 * m21;
    float r22 = m00 * m11 - m01 * m10;
    
    float m03 = m[0][3], m13 = m[1][3], m23 = m[2][3];
    
    float r03 = - (r00 * m03 + r01 * m13 + r02 * m23);
    float r13 = - (r10 * m03 + r11 * m13 + r12 * m23);
    float r23 = - (r20 * m03 + r21 * m13 + r22 * m23);
    
    return Matrix4x4(
                   r00, r01, r02, r03,
                   r10, r11, r12, r13,
                   r20, r21, r22, r23,
                   0,   0,   0,   1);
}

void Matrix4x4::InitTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation)
{
    // Ordering:
    //    1. Scale
    //    2. Rotate
    //    3. Translate
    
    Matrix4x4 rot3x3;
    orientation.ToRotationMatrix(rot3x3);
    
    // Set up final matrix with scale, rotation and translation
    m[0][0] = scale.x * rot3x3[0][0]; m[0][1] = scale.y * rot3x3[0][1]; m[0][2] = scale.z * rot3x3[0][2]; m[0][3] = position.x;
    m[1][0] = scale.x * rot3x3[1][0]; m[1][1] = scale.y * rot3x3[1][1]; m[1][2] = scale.z * rot3x3[1][2]; m[1][3] = position.y;
    m[2][0] = scale.x * rot3x3[2][0]; m[2][1] = scale.y * rot3x3[2][1]; m[2][2] = scale.z * rot3x3[2][2]; m[2][3] = position.z;
    
    // No projection term
    m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}


















void Matrix4x4::ToAxisAngle (Vector3& rkAxis, float& rffloats) const
{
    // Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
    // The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
    // I is the identity and
    //
    //       +-        -+
    //   P = |  0 -z +y |
    //       | +z  0 -x |
    //       | -y +x  0 |
    //       +-        -+
    //
    // If A > 0, R represents a counterclockwise rotation about the axis in
    // the sense of looking from the tip of the axis vector towards the
    // origin.  Some algebra will show that
    //
    //   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
    //
    // In the event that A = pi, R-R^t = 0 which prevents us from extracting
    // the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
    // P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
    // z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
    // it does not matter which sign you choose on the square roots.
    
    float fTrace = m[0][0] + m[1][1] + m[2][2];
    float fCos = 0.5f*(fTrace-1.0f);
    rffloats = acosf(fCos);  // in [0,PI]
    
    if ( rffloats > float(0.0) )
    {
        if ( rffloats < float(PI) )
        {
            rkAxis.x = m[2][1]-m[1][2];
            rkAxis.y = m[0][2]-m[2][0];
            rkAxis.z = m[1][0]-m[0][1];
            rkAxis.Normalize();
        }
        else
        {
            // angle is PI
            float fHalfInverse;
            if ( m[0][0] >= m[1][1] )
            {
                // r00 >= r11
                if ( m[0][0] >= m[2][2] )
                {
                    // r00 is maximum diagonal term
                    rkAxis.x = 0.5f*sqrtf(m[0][0] -
                                               m[1][1] - m[2][2] + 1.0f);
                    fHalfInverse = 0.5f/rkAxis.x;
                    rkAxis.y = fHalfInverse*m[0][1];
                    rkAxis.z = fHalfInverse*m[0][2];
                }
                else
                {
                    // r22 is maximum diagonal term
                    rkAxis.z = 0.5f*sqrtf(m[2][2] -
                                               m[0][0] - m[1][1] + 1.0f);
                    fHalfInverse = 0.5f/rkAxis.z;
                    rkAxis.x = fHalfInverse*m[0][2];
                    rkAxis.y = fHalfInverse*m[1][2];
                }
            }
            else
            {
                // r11 > r00
                if ( m[1][1] >= m[2][2] )
                {
                    // r11 is maximum diagonal term
                    rkAxis.y = 0.5f*sqrtf(m[1][1] -
                                               m[0][0] - m[2][2] + 1.0f);
                    fHalfInverse  = 0.5f/rkAxis.y;
                    rkAxis.x = fHalfInverse*m[0][1];
                    rkAxis.z = fHalfInverse*m[1][2];
                }
                else
                {
                    // r22 is maximum diagonal term
                    rkAxis.z = 0.5f*sqrtf(m[2][2] -
                                               m[0][0] - m[1][1] + 1.0f);
                    fHalfInverse = 0.5f/rkAxis.z;
                    rkAxis.x = fHalfInverse*m[0][2];
                    rkAxis.y = fHalfInverse*m[1][2];
                }
            }
        }
    }
    else
    {
        // The angle is 0 and the matrix is the identity.  Any axis will
        // work, so just use the x-axis.
        rkAxis.x = 1.0;
        rkAxis.y = 0.0;
        rkAxis.z = 0.0;
    }
}
//-----------------------------------------------------------------------
void Matrix4x4::FromAxisAngle (const Vector3& rkAxis, const float& ffloats)
{
    float fCos = cosf(ffloats);
    float fSin = sinf(ffloats);
    float fOneMinusCos = 1.0f-fCos;
    float fX2 = rkAxis.x*rkAxis.x;
    float fY2 = rkAxis.y*rkAxis.y;
    float fZ2 = rkAxis.z*rkAxis.z;
    float fXYM = rkAxis.x*rkAxis.y*fOneMinusCos;
    float fXZM = rkAxis.x*rkAxis.z*fOneMinusCos;
    float fYZM = rkAxis.y*rkAxis.z*fOneMinusCos;
    float fXSin = rkAxis.x*fSin;
    float fYSin = rkAxis.y*fSin;
    float fZSin = rkAxis.z*fSin;
    
    m[0][0] = fX2*fOneMinusCos+fCos;
    m[0][1] = fXYM-fZSin;
    m[0][2] = fXZM+fYSin;
    m[1][0] = fXYM+fZSin;
    m[1][1] = fY2*fOneMinusCos+fCos;
    m[1][2] = fYZM-fXSin;
    m[2][0] = fXZM-fYSin;
    m[2][1] = fYZM+fXSin;
    m[2][2] = fZ2*fOneMinusCos+fCos;
}
//-----------------------------------------------------------------------
bool Matrix4x4::ToEulerAnglesXYZ (float& rfYAngle, float& rfPAngle,
                                float& rfRAngle) const
{
    // rot =  cy*cz          -cy*sz           sy
    //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
    //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy
    
    rfPAngle = float(asinf(m[0][2]));
    if ( rfPAngle < float(HALF_PI) )
    {
        if ( rfPAngle > float(-HALF_PI) )
        {
            rfYAngle = atan2f(-m[1][2],m[2][2]);
            rfRAngle = atan2f(-m[0][1],m[0][0]);
            return true;
        }
        else
        {
            // WARNING.  Not a unique solution.
            float fRmY = atan2f(m[1][0],m[1][1]);
            rfRAngle = float(0.0);  // any angle works
            rfYAngle = rfRAngle - fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
        float fRpY = atan2f(m[1][0],m[1][1]);
        rfRAngle = float(0.0);  // any angle works
        rfYAngle = fRpY - rfRAngle;
        return false;
    }
}
//-----------------------------------------------------------------------
bool Matrix4x4::ToEulerAnglesXZY (float& rfYAngle, float& rfPAngle,
                                float& rfRAngle) const
{
    // rot =  cy*cz          -sz              cz*sy
    //        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
    //       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz
    
    rfPAngle = asinf(-m[0][1]);
    if ( rfPAngle < float(HALF_PI) )
    {
        if ( rfPAngle > float(-HALF_PI) )
        {
            rfYAngle = atan2f(m[2][1],m[1][1]);
            rfRAngle = atan2f(m[0][2],m[0][0]);
            return true;
        }
        else
        {
            // WARNING.  Not a unique solution.
            float fRmY = atan2f(-m[2][0],m[2][2]);
            rfRAngle = float(0.0);  // any angle works
            rfYAngle = rfRAngle - fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
        float fRpY = atan2f(-m[2][0],m[2][2]);
        rfRAngle = float(0.0);  // any angle works
        rfYAngle = fRpY - rfRAngle;
        return false;
    }
}
//-----------------------------------------------------------------------
bool Matrix4x4::ToEulerAnglesYXZ (float& rfYAngle, float& rfPAngle,
                                float& rfRAngle) const
{
    // rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
    //        cx*sz           cx*cz          -sx
    //       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy
    
    rfPAngle = asinf(-m[1][2]);
    if ( rfPAngle < float(HALF_PI) )
    {
        if ( rfPAngle > float(-HALF_PI) )
        {
            rfYAngle = atan2f(m[0][2],m[2][2]);
            rfRAngle = atan2f(m[1][0],m[1][1]);
            return true;
        }
        else
        {
            // WARNING.  Not a unique solution.
            float fRmY = atan2f(-m[0][1],m[0][0]);
            rfRAngle = float(0.0);  // any angle works
            rfYAngle = rfRAngle - fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
        float fRpY = atan2f(-m[0][1],m[0][0]);
        rfRAngle = float(0.0);  // any angle works
        rfYAngle = fRpY - rfRAngle;
        return false;
    }
}
//-----------------------------------------------------------------------
bool Matrix4x4::ToEulerAnglesYZX (float& rfYAngle, float& rfPAngle,
                                float& rfRAngle) const
{
    // rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
    //        sz              cx*cz          -cz*sx
    //       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz
    
    rfPAngle = asinf(m[1][0]);
    if ( rfPAngle < float(HALF_PI) )
    {
        if ( rfPAngle > float(-HALF_PI) )
        {
            rfYAngle = atan2f(-m[2][0],m[0][0]);
            rfRAngle = atan2f(-m[1][2],m[1][1]);
            return true;
        }
        else
        {
            // WARNING.  Not a unique solution.
            float fRmY = atan2f(m[2][1],m[2][2]);
            rfRAngle = float(0.0);  // any angle works
            rfYAngle = rfRAngle - fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
        float fRpY = atan2f(m[2][1],m[2][2]);
        rfRAngle = float(0.0);  // any angle works
        rfYAngle = fRpY - rfRAngle;
        return false;
    }
}
//-----------------------------------------------------------------------
bool Matrix4x4::ToEulerAnglesZXY (float& rfYAngle, float& rfPAngle,
                                float& rfRAngle) const
{
    // rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
    //        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
    //       -cx*sy           sx              cx*cy
    
    rfPAngle = asinf(m[2][1]);
    if ( rfPAngle < float(HALF_PI) )
    {
        if ( rfPAngle > float(-HALF_PI) )
        {
            rfYAngle = atan2f(-m[0][1],m[1][1]);
            rfRAngle = atan2f(-m[2][0],m[2][2]);
            return true;
        }
        else
        {
            // WARNING.  Not a unique solution.
            float fRmY = atan2f(m[0][2],m[0][0]);
            rfRAngle = float(0.0);  // any angle works
            rfYAngle = rfRAngle - fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
        float fRpY = atan2f(m[0][2],m[0][0]);
        rfRAngle = float(0.0);  // any angle works
        rfYAngle = fRpY - rfRAngle;
        return false;
    }
}
//-----------------------------------------------------------------------
bool Matrix4x4::ToEulerAnglesZYX (float& rfYAngle, float& rfPAngle,
                                float& rfRAngle) const
{
    // rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
    //        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
    //       -sy              cy*sx           cx*cy
    
    rfPAngle = asinf(-m[2][0]);
    if ( rfPAngle < float(HALF_PI) )
    {
        if ( rfPAngle > float(-HALF_PI) )
        {
            rfYAngle = atan2f(m[1][0],m[0][0]);
            rfRAngle = atan2f(m[2][1],m[2][2]);
            return true;
        }
        else
        {
            // WARNING.  Not a unique solution.
            float fRmY = atan2f(-m[0][1],m[0][2]);
            rfRAngle = float(0.0);  // any angle works
            rfYAngle = rfRAngle - fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
        float fRpY = atan2f(-m[0][1],m[0][2]);
        rfRAngle = float(0.0);  // any angle works
        rfYAngle = fRpY - rfRAngle;
        return false;
    }
}
//-----------------------------------------------------------------------
void Matrix4x4::FromEulerAnglesXYZ (const float& fYAngle, const float& fPAngle,
                                  const float& fRAngle)
{
    float fCos, fSin;
    
    fCos = cosf(fYAngle);
    fSin = sinf(fYAngle);
    Matrix4x4 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);
    
    fCos = cosf(fPAngle);
    fSin = sinf(fPAngle);
    Matrix4x4 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);
    
    fCos = cosf(fRAngle);
    fSin = sinf(fRAngle);
    Matrix4x4 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);
    
    *this = kXMat*(kYMat*kZMat);
}
//-----------------------------------------------------------------------
void Matrix4x4::FromEulerAnglesXZY (const float& fYAngle, const float& fPAngle,
                                  const float& fRAngle)
{
    float fCos, fSin;
    
    fCos = cosf(fYAngle);
    fSin = sinf(fYAngle);
    Matrix4x4 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);
    
    fCos = cosf(fPAngle);
    fSin = sinf(fPAngle);
    Matrix4x4 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);
    
    fCos = cosf(fRAngle);
    fSin = sinf(fRAngle);
    Matrix4x4 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);
    
    *this = kXMat*(kZMat*kYMat);
}
//-----------------------------------------------------------------------
void Matrix4x4::FromEulerAnglesYXZ (const float& fYAngle, const float& fPAngle,
                                  const float& fRAngle)
{
    float fCos, fSin;
    
    fCos = cosf(fYAngle);
    fSin = sinf(fYAngle);
    Matrix4x4 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);
    
    fCos = cosf(fPAngle);
    fSin = sinf(fPAngle);
    Matrix4x4 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);
    
    fCos = cosf(fRAngle);
    fSin = sinf(fRAngle);
    Matrix4x4 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);
    
    *this = kYMat*(kXMat*kZMat);
}
//-----------------------------------------------------------------------
void Matrix4x4::FromEulerAnglesYZX (const float& fYAngle, const float& fPAngle,
                                  const float& fRAngle)
{
    float fCos, fSin;
    
    fCos = cosf(fYAngle);
    fSin = sinf(fYAngle);
    Matrix4x4 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);
    
    fCos = cosf(fPAngle);
    fSin = sinf(fPAngle);
    Matrix4x4 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);
    
    fCos = cosf(fRAngle);
    fSin = sinf(fRAngle);
    Matrix4x4 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);
    
    *this = kYMat*(kZMat*kXMat);
}
//-----------------------------------------------------------------------
void Matrix4x4::FromEulerAnglesZXY (const float& fYAngle, const float& fPAngle,
                                  const float& fRAngle)
{
    float fCos, fSin;
    
    fCos = cosf(fYAngle);
    fSin = sinf(fYAngle);
    Matrix4x4 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);
    
    fCos = cosf(fPAngle);
    fSin = sinf(fPAngle);
    Matrix4x4 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);
    
    fCos = cosf(fRAngle);
    fSin = sinf(fRAngle);
    Matrix4x4 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);
    
    *this = kZMat*(kXMat*kYMat);
}
//-----------------------------------------------------------------------
void Matrix4x4::FromEulerAnglesZYX (const float& fYAngle, const float& fPAngle,
                                  const float& fRAngle)
{
    float fCos, fSin;
    
    fCos = cosf(fYAngle);
    fSin = sinf(fYAngle);
    Matrix4x4 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);
    
    fCos = cosf(fPAngle);
    fSin = sinf(fPAngle);
    Matrix4x4 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);
    
    fCos = cosf(fRAngle);
    fSin = sinf(fRAngle);
    Matrix4x4 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);
    
    *this = kZMat*(kYMat*kXMat);
}

const Matrix4x4 Matrix4x4::IDENTITY(
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f);


Matrix4x4 Matrix4x4::Lerp(const Pocket::Matrix4x4 &source, const Pocket::Matrix4x4 &dest, const float &t) {
    
    /*
    Matrix4x4 delta = source.Transpose() * dest;
    
    Vector3 axis;
    float angle;

    delta.ToAxisAngle(axis, angle);
    
    Matrix4x4 c =  source * Matrix4x4::CreateRotation(axis, angle * t);
        //c *= source;
    //c.SetTranslation(0);
    //c.m[3][3]=1;
    //return c;
    */

    
    
    Quaternion a;
    a.FromRotationMatrix(source);
    a.Normalize();
    
    Quaternion b;
    b.FromRotationMatrix(dest);
    b.Normalize();
    
    Matrix4x4 c;
    
    
    Vector3 sc = source.RealScale();
    Vector3 dc = dest.RealScale();
    
    c.InitTransform(
                    Vector3::Lerp(source.Translation(), dest.Translation(), t),
                    Vector3::Lerp(sc, dc, t),
                    Quaternion::Slerp(t, a, b));
    
    /*
    
    Matrix4x4 delta = dest * source.Transpose();
    
    Vector3 axis;
    float angle;
    
    delta.ToAxisAngle(axis, angle);
    
    Matrix4x4 c;
    c.FromAxisAngle(axis, angle *t);
    c *= source;
    c.SetTranslation(Vector3::Lerp(source.Translation(), dest.Translation(), t));
     
     */
    return c;
}


Matrix4x4 Matrix4x4::Add(const Pocket::Matrix4x4& a, const Pocket::Matrix4x4& b) {
    
    Quaternion rotA;
    rotA.FromRotationMatrix(a);
    rotA.Normalize();
    
    Quaternion rotB;
    rotB.FromRotationMatrix(b);
    rotB.Normalize();
    
    Matrix4x4 c;
    
    Vector3 sc = a.RealScale();
    Vector3 dc = b.RealScale();
    
    Quaternion rot = rotA + rotB;
    rot.Normalize();
    
    c.InitTransform(
                    a.Translation() + b.Translation(),
                    sc * dc,
                    rot);
    
    return c;
}

Matrix4x4 Matrix4x4::Deserialize(const std::string &data) {
    std::vector<float> v;
    StringHelper::splitToFloats(data, ',', v);
    Matrix4x4 m = IDENTITY;
    size_t max = 16;
    if (v.size()<max) max = v.size();
    for (int i=0; i<max; ++i) {
        m._m[i] = v[i];
    }
    return m;
}

std::ostream& operator<<(std::ostream& stream, const Pocket::Matrix4x4& m) {
    for (int i=0; i<16; ++i) {
        stream<<m._m[i];
        if (i<15) stream<<",";
    }
    return stream;
}





