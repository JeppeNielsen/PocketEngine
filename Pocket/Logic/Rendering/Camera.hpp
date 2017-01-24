
#pragma once
#include "Property.hpp"
#include "DirtyProperty.hpp"
#include "Matrix4x4.hpp"
#include "Rect.hpp"
#include "Ray.hpp"
#include "Transform.hpp"

namespace Pocket {
    
    typedef uint64_t RenderMask;

	class Camera {
	public:
		Camera();
        ~Camera();
        
		Property<float> FieldOfView;
		Property<float> Near;
		Property<float> Far;
		Property<Rect> Viewport;
        Property<float> Aspect;
        Property<bool> Orthographic;
        
        Property<RenderMask> Mask;
    
		DirtyProperty<Matrix4x4> Projection;
		DirtyProperty<Matrix4x4> ProjectionInverse;
        
        Matrix4x4 GetViewProjection(Transform* viewTransform);
        Ray GetRay(Transform* viewTransform, Vector2 screenPosition);
    
        Vector3 TransformPointToViewSpace(Transform* viewTransform, Vector3 worldPoint);
        Vector3 TransformPointToScreenSpace(Transform* viewTransform, Vector3 worldPoint);
        Vector3 TransformViewportToWorld(Transform* viewTransform, Vector3 viewportPoint);
        Vector3 TransformWorldToViewport(Transform* viewTransform, Vector3 worldPoint);
        
    private:
        void ScreenSizeChanged();
    public:
    	
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(FieldOfView)
        TYPE_FIELD(Near)
        TYPE_FIELD(Far)
        TYPE_FIELD(Viewport)
        TYPE_FIELD(Aspect)
        TYPE_FIELD(Orthographic)
        TYPE_FIELD(Mask)
        TYPE_FIELDS_END
	};
}
