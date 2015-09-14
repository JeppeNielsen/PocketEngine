
#pragma once//;
#include "Property.hpp"
#include "DirtyProperty.hpp"
#include "Matrix4x4.hpp"
#include "Box.hpp"
#include "Ray.hpp"
#include "GameWorld.hpp"
#include "Transform.hpp"

namespace Pocket {
    
    typedef uint64_t RenderMask;

	Component(Camera)
	public:
		
		Camera();

		Property<Camera*, float> FieldOfView;
		Property<Camera*, float> Near;
		Property<Camera*, float> Far;
		Property<Camera*, Box> Viewport;
        Property<Camera*, bool> Orthographic;
        Property<Camera*, RenderMask> Mask;
    
		DirtyProperty<Camera*, Matrix4x4> Projection;
		DirtyProperty<Camera*, Matrix4x4> ProjectionInverse;
        
        Matrix4x4 GetViewProjection(Transform* viewTransform);
        Ray GetRay(Transform* viewTransform, Vector2 screenPosition);
    
        Vector3 TransformPointToViewSpace(Transform* viewTransform, Vector3 worldPoint);
        Vector3 TransformPointToScreenSpace(Transform* viewTransform, Vector3 worldPoint);
    
        void Reset();
    
    protected:
        void Clone(const Camera& source);
    
	private:
		void CalcProjectionMatrix(DirtyProperty<Camera*, Matrix4x4>::EventData& event);
		void CalcProjectionInverseMatrix(DirtyProperty<Camera*, Matrix4x4>::EventData& event);
		
		void ProjectionPropertyChanged(Camera* camera);

	};
}