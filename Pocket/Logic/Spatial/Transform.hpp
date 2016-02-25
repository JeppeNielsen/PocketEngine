#pragma once
#include "Property.hpp"
#include "DirtyProperty.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
	class Transform {
	public:
		Transform();

		Property<Vector3> Position;  
		Property<Quaternion> Rotation;
        Property<Vector3> EulerRotation;
        Property<Vector3> Scale;
		Property<Vector3> Anchor; 
		Property<Matrix4x4> Matrix;

		DirtyProperty<Matrix4x4> Local;
		DirtyProperty<Matrix4x4> World;
		DirtyProperty<Matrix4x4> WorldInverse;

		void HookOther(Transform* other);
		void UnHookOther(Transform* other);
        
        void ResetWorldCalculation();
    
    protected:
        
        void Lerp(const Transform& source, const Transform& dest, const float& t);
        void Relative(const Transform& a, const Transform& b);
    
	private:
		void PropertyChanged();
        void OtherWorldChanged();
        
        bool useEulerRotation;
    
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(Position);
        TYPE_FIELD(Rotation);
        TYPE_FIELD(Scale);
        TYPE_FIELD(Anchor);
        TYPE_FIELD(Matrix);
        TYPE_FIELDS_END
	};
}