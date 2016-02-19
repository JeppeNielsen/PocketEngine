
#include "Camera.hpp"
#include "Transform.hpp"

using namespace Pocket;

Camera::Camera() {
	FieldOfView = 70;
	Near = 0.1f;
	Far = 512.0f;
    Orthographic = false;
    Mask = 0;
	
	Projection.Method = [this] (Matrix4x4& mat) {
        if (!Orthographic) {
            mat.InitPerspective(FieldOfView, Viewport().Aspect(), Near, Far);
        } else {
            mat.InitOrthographic(Viewport().left, Viewport().top, Viewport().right, Viewport().bottom, Near, Far);
        }
    };
    
	ProjectionInverse.Method = [this] (Matrix4x4& mat) {
        mat = Projection().Invert();
    };
    
    auto changed = [this] (auto& v) {
        Projection.MakeDirty();
        ProjectionInverse.MakeDirty();
    };

	FieldOfView.Changed.Bind(changed);
    Near.Changed.Bind(changed);
    Far.Changed.Bind(changed);
    Viewport.Changed.Bind(changed);
}

Matrix4x4 Camera::GetViewProjection(Transform* viewTransform) {
	return Projection().Multiply(viewTransform->WorldInverse);
}

Ray Camera::GetRay(Transform* viewTransform, Pocket::Vector2 screenPosition) {
    const Box& viewPort = Viewport;
    
    Vector2 fromCenter = screenPosition - viewPort.Center();
    fromCenter /= (viewPort.Size() * 0.5f);
    
    Matrix4x4 viewProjection = GetViewProjection(viewTransform).Invert();
    
    Vector3 rayStartPosition = viewProjection.TransformPosition(Vector3(fromCenter.x,fromCenter.y,-1));
    Vector3 rayEndPosition = viewProjection.TransformPosition(Vector3(fromCenter.x,fromCenter.y,1));
    return Ray(rayStartPosition, rayEndPosition - rayStartPosition);
}

Vector3 Camera::TransformPointToViewSpace(Transform* viewTransform, Vector3 worldPoint) {
    Matrix4x4 viewProjection = GetViewProjection(viewTransform);
    return viewProjection.TransformPosition(worldPoint);
}

Vector3 Camera::TransformPointToScreenSpace(Transform* viewTransform, Vector3 worldPoint) {
    worldPoint = TransformPointToViewSpace(viewTransform, worldPoint);
    Vector2 screenPoint = Vector2(worldPoint.x, worldPoint.y );
    const Box& viewPort = Viewport;
    screenPoint *= (viewPort.Size() * 0.5f);
    screenPoint += viewPort.Center();
    return Vector3(screenPoint.x, screenPoint.y, worldPoint.z);
}

Vector3 Camera::TransformViewportToWorld(Transform* viewTransform, Vector3 viewportPoint) {
    Matrix4x4 viewProjection = GetViewProjection(viewTransform).Invert();
    return viewProjection.TransformPosition(viewportPoint);
}

Vector3 Camera::TransformWorldToViewport(Transform* viewTransform, Vector3 worldPoint) {
    Matrix4x4 viewProjection = GetViewProjection(viewTransform);
    return viewProjection.TransformPosition(worldPoint);
}