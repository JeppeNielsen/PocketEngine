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

		Property<Transform*, Vector3> Position;  
		Property<Transform*, Quaternion> Rotation;
        Property<Transform*, Vector3> EulerRotation;
        Property<Transform*, Vector3> Scale;
		Property<Transform*, Vector3> Anchor; 
		Property<Transform*, Matrix4x4> Matrix;

		DirtyProperty<Transform*, Matrix4x4> Local;
		DirtyProperty<Transform*, Matrix4x4> World;
		DirtyProperty<Transform*, Matrix4x4> WorldInverse;

		void HookOther(Transform* other);
		void UnHookOther(Transform* other);

		void Reset();
    
    protected:
        
        void Lerp(const Transform& source, const Transform& dest, const float& t);
        void Relative(const Transform& a, const Transform& b);
    
	private:
		void PropertyChanged(Transform* transform);
        void EulerRotationChanged(Transform* transform);
		void CalcLocal(DirtyProperty<Transform*, Matrix4x4>::EventData& event);
		void CalcWorld(DirtyProperty<Transform*, Matrix4x4>::EventData& event);
		void CalcWorldInverse(DirtyProperty<Transform*, Matrix4x4>::EventData& event);

		void OtherWorldChanged(DirtyProperty<Transform*, Matrix4x4>* event);
        
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