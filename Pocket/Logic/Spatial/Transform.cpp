
#include "Transform.hpp"

using namespace Pocket;

Transform::Transform()
	:
Position(this),
Rotation(this),
EulerRotation(this),
Scale(this),
Anchor(this),
Matrix(this),
Local(this),
World(this),
WorldInverse(this)
{
	Position.Changed+=event_handler(this, &Transform::PropertyChanged);
	Rotation.Changed+=event_handler(this, &Transform::PropertyChanged);
    EulerRotation.Changed += event_handler(this, &Transform::EulerRotationChanged);
    Scale.Changed+=event_handler(this, &Transform::PropertyChanged); 
	Anchor.Changed+=event_handler(this, &Transform::PropertyChanged);
	Matrix.Changed+=event_handler(this, &Transform::PropertyChanged);

	Matrix = Matrix4x4::IDENTITY;

	Local.Method += event_handler(this, &Transform::CalcLocal);
	World.Method += event_handler(this, &Transform::CalcWorld);
	WorldInverse.Method += event_handler(this, &Transform::CalcWorldInverse);
}

void Transform::Reset() {
	Position = 0;
	Rotation = Quaternion::IDENTITY;
    EulerRotation = 0;
    Scale = 1;
	Anchor = 0;
	Matrix = Matrix4x4::IDENTITY;
    useEulerRotation = false;
}

void Transform::PropertyChanged(Transform* transform) {
	Local.MakeDirty();
	World.MakeDirty();
	WorldInverse.MakeDirty();
}

void Transform::CalcLocal( DirtyProperty<Transform*, Matrix4x4>::EventData& event )
{
	Matrix4x4& local = *event.Value;
	Matrix4x4 rotation;
	Rotation.GetValue().ToRotationMatrix(rotation);

	Matrix4x4 translation;
	translation.InitTranslation(Position.GetValue());

	Vector3 anchor = Anchor;
	//local.InitTranslation(-anchor);
	local.InitScale(1.0f);
    local *= translation;
    local *= Matrix.GetValue();
	local *= rotation;
    local *= Matrix4x4::CreateScale(Scale);
    Matrix4x4 anchorMatrix;
    anchorMatrix.InitTranslation(-anchor);
    local*=anchorMatrix;
}

void Transform::CalcWorld( DirtyProperty<Transform*, Matrix4x4>::EventData& event )
{
	Matrix4x4& world = *event.Value;
	world = *Local.GetValue();
}

void Transform::CalcWorldInverse( DirtyProperty<Transform*, Matrix4x4>::EventData& event )
{
	Matrix4x4& inv = *event.Value;
	inv = World.GetValue()->Invert();
}

void Transform::HookOther( Transform* other )
{
	other->World.HasBecomeDirty += event_handler(this, &Transform::OtherWorldChanged);
}

void Transform::UnHookOther( Transform* other )
{
	other->World.HasBecomeDirty -= event_handler(this, &Transform::OtherWorldChanged);
}

void Transform::OtherWorldChanged( DirtyProperty<Transform*, Matrix4x4>* event )
{
	World.MakeDirty();
	WorldInverse.MakeDirty();
}

void Transform::EulerRotationChanged(Pocket::Transform *transform) {
    useEulerRotation = true;
    Rotation = Quaternion(EulerRotation);
}

void Transform::Lerp(const Transform& source, const Transform& dest, const float& t) {
    Position = Vector3::Lerp(source.Position, dest.Position, t);
    
    if (source.useEulerRotation || dest.useEulerRotation) {
        EulerRotation = Vector3::Lerp(source.EulerRotation, dest.EulerRotation, t);
    } else {
        Rotation = Quaternion::Slerp(t, source.Rotation, dest.Rotation);
    }
    Scale = Vector3::Lerp(source.Scale, dest.Scale, t);
    Anchor = Vector3::Lerp(source.Anchor, dest.Anchor, t);
    Matrix = Matrix4x4::Lerp(source.Matrix, dest.Matrix, t);
    useEulerRotation = dest.useEulerRotation;
}

void Transform::Relative(const Pocket::Transform &a, const Pocket::Transform &b) {
    //Position = a.Position + b.Position;
    //Rotation = a.Rotation + b.Rotation;
    //Anchor = a.Anchor + b.Anchor;
    //Matrix = Matrix4x4::Add(a.Matrix, b.Matrix);
    
    if (b.useEulerRotation) {
        Matrix = Matrix4x4::CreateTransform(b.Position, b.Scale, Quaternion(b.EulerRotation)) * b.Matrix;
    } else {
        Matrix = Matrix4x4::CreateTransform(b.Position, b.Scale, b.Rotation) * b.Matrix;
    }
}

void Transform::Serialize(Pocket::ISerializedProperty *property) {
    property->Add("Position", Position());
    property->Add("Rotation", Rotation());
    property->Add("Scale", Scale());
}

void Transform::Deserialize(Pocket::ISerializedProperty *property) {
    Position = property->GetValue<Vector3>("Position", Vector3(0,0,0));
    Rotation = property->GetValue<Quaternion>("Rotation", Quaternion::IDENTITY);
    Scale = property->GetValue<Vector3>("Scale", Vector3(1,1,1));
}
