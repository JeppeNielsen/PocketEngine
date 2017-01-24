
#include "Camera.hpp"
#include "Transform.hpp"
#include "Engine.hpp"
#include "MathHelper.hpp"

using namespace Pocket;

Camera::Camera() {
	FieldOfView = 70;
	Near = 0.1f;
	Far = 512.0f;
    Orthographic = false;
    Mask = 0;
    Viewport = {0,0,1,1};
    Aspect = 0;
	
	Projection.Method = [this] (Matrix4x4& mat) {
        float fieldOfView = FieldOfView;
        if (!Orthographic) {
            float aspect;
            if (MathHelper::FloatEqual(Aspect, 0, 0.001f)) {
                const Rect& viewport = Viewport;
                const Vector2& screenSize = Engine::Context().ScreenSize;
                Rect screenRect = viewport * screenSize;
                aspect = screenRect.Aspect();
            } else {
                aspect = Aspect;
            }
            mat.InitPerspective(fieldOfView, aspect, Near, Far);
        } else {
            if (fieldOfView<=0.0001f) {
                const Rect& viewport = Viewport;
                const Vector2& screenSize = Engine::Context().ScreenSize;
                Rect screenRect = viewport * screenSize;
                mat.InitOrthographic(0, screenRect.height, screenRect.width, 0, Near, Far);
            } else {
                const Rect& viewport = Viewport;
                const Vector2& screenSize = Engine::Context().ScreenSize;
                Rect screenRect = viewport * screenSize;
                float aspect = screenRect.Aspect();
                
                float top = fieldOfView * 0.5f;
                float right = top * aspect;
                //(float left, float top, float right, float bottom, float nearValue, float farValue)
                mat.InitOrthographic(-right, top, right, -top, Near, Far);
            }
        }
    };
    
	ProjectionInverse.Method = [this] (Matrix4x4& mat) {
        mat = Projection().Invert();
    };
    
    auto changed = [this] () {
        Projection.MakeDirty();
        ProjectionInverse.MakeDirty();
    };

	FieldOfView.Changed.Bind(changed);
    Near.Changed.Bind(changed);
    Far.Changed.Bind(changed);
    Viewport.Changed.Bind(changed);
    Aspect.Changed.Bind([this] () {
        if (MathHelper::FloatEqual(Aspect, 0, 0.001f)) {
            Engine::Context().ScreenSize.Changed.Bind(this, &Camera::ScreenSizeChanged);
        } else {
            Engine::Context().ScreenSize.Changed.Unbind(this, &Camera::ScreenSizeChanged);
        }
    });
    
    FieldOfView.Changed.MarkDefaults();
    Near.Changed.MarkDefaults();
    Far.Changed.MarkDefaults();
    Viewport.Changed.MarkDefaults();
    Aspect.Changed.MarkDefaults();
    
    Engine::Context().ScreenSize.Changed.Bind(this, &Camera::ScreenSizeChanged);
}

Camera::~Camera() {
    Engine::Context().ScreenSize.Changed.Unbind(this, &Camera::ScreenSizeChanged);
}

void Camera::ScreenSizeChanged() {
    Projection.MakeDirty();
    ProjectionInverse.MakeDirty();
}

Matrix4x4 Camera::GetViewProjection(Transform* viewTransform) {
	return Projection().Multiply(viewTransform->WorldInverse);
}

Ray Camera::GetRay(Transform* viewTransform, Pocket::Vector2 screenPosition) {
    const Rect& viewPort = Viewport() * Engine::Context().ScreenSize();
    
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
    const Rect& viewPort = Viewport() * Engine::Context().ScreenSize();
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
