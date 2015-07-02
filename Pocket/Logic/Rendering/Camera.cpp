
#include "Camera.hpp"
#include "OpenGL.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"

using namespace Pocket;

Camera::Camera():
	FieldOfView(this),
	Near(this),
	Far(this),
    Viewport(this),
	Projection(this),
	ProjectionInverse(this),
    Orthographic(this),
    Mask(this)
{
	FieldOfView = 70;
	Near = 0.1f;
	Far = 512.0f;
    Orthographic = false;
    Mask = 0;
	
	Projection.Method += event_handler(this, &Camera::CalcProjectionMatrix);
	ProjectionInverse.Method += event_handler(this, &Camera::CalcProjectionInverseMatrix);

	FieldOfView.Changed += event_handler(this, &Camera::ProjectionPropertyChanged);
	Near.Changed += event_handler(this, &Camera::ProjectionPropertyChanged);
	Far.Changed += event_handler(this, &Camera::ProjectionPropertyChanged);
    Viewport.Changed += event_handler(this, &Camera::ProjectionPropertyChanged);
}

void Camera::ProjectionPropertyChanged(Camera* camera) {
	Projection.MakeDirty();
	ProjectionInverse.MakeDirty();
}

void Camera::CalcProjectionMatrix(DirtyProperty<Camera*, Matrix4x4>::EventData& event) {
	Matrix4x4& mat = *event.Value;
    if (!Orthographic) {
        mat.InitPerspective(FieldOfView, Viewport().Aspect(), Near, Far);
    } else {
        mat.InitOrthographic(Viewport().left, Viewport().top, Viewport().right, Viewport().bottom, Near, Far);
        
        //mat.InitOrthographic(-1, 1, 1, -1, 1, FAR);
    }
}

void Camera::CalcProjectionInverseMatrix(DirtyProperty<Camera*, Matrix4x4>::EventData& event) {
	Matrix4x4& mat = *event.Value;
	mat = Projection.GetValue()->Invert();
}

Matrix4x4 Camera::GetViewProjection(Transform* viewTransform) {
	return Projection.GetValue()->Multiply(*viewTransform->WorldInverse.GetValue());
}

Ray Camera::GetRay(Transform* viewTransform, Pocket::Vector2 screenPosition) {
    const Box& viewPort = Viewport.GetValue();
    
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
    const Box& viewPort = Viewport.GetValue();
    screenPoint *= (viewPort.Size() * 0.5f);
    screenPoint += viewPort.Center();
    return Vector3(screenPoint.x, screenPoint.y, worldPoint.z);
}

void Camera::Reset() {
    FieldOfView = 70;
	Near = 0.1f;
	Far = 512.0f;
    Orthographic = false;
    Mask = 0;
}

void Camera::Clone(const Camera& source) {
    FieldOfView = source.FieldOfView;
    Near = source.Near;
    Far = source.Far;
    Viewport = source.Viewport;
    Orthographic = source.Orthographic;
    Mask = source.Mask;
}

void Camera::Serialize(Pocket::ISerializedProperty *property) {
    property->Add("FieldOfView", FieldOfView());
    property->Add("Near", Near());
    property->Add("Far", Far());
    property->Add("Orthographic", Orthographic());
    property->Add("Mask", Mask());
}

void Camera::Deserialize(Pocket::ISerializedProperty *property) {
    FieldOfView = property->GetValue("FieldOfView", 60.0f);
    Near = property->GetValue("Near", 0.1f);
    Far = property->GetValue("Far", 1000.0f);
    Orthographic = property->GetValue("Orthographic", false);
    Mask = property->GetValue("Mask", (RenderMask)0);
}
