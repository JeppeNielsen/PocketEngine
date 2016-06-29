
#include "Transform.hpp"

using namespace Pocket;

Transform::Transform() {

    Position = 0;
	Rotation = Quaternion::IDENTITY;
    EulerRotation = 0;
    Scale = 1;
	Anchor = 0;
	Matrix = Matrix4x4::IDENTITY;
    useEulerRotation = false;
    World.HasBecomeDirty.Clear();
    
    Position.Changed.Bind(this, &Transform::PropertyChanged);
    Rotation.Changed.Bind(this, &Transform::PropertyChanged);
    Scale.Changed.Bind(this, &Transform::PropertyChanged);
    Anchor.Changed.Bind(this, &Transform::PropertyChanged);
    Matrix.Changed.Bind(this, &Transform::PropertyChanged);
    
    EulerRotation.Changed.Bind([this] () {
        useEulerRotation = true;
        Rotation = Quaternion(EulerRotation);
    });
    
    Position.Changed.MarkDefaults();
    Rotation.Changed.MarkDefaults();
    Scale.Changed.MarkDefaults();
    Anchor.Changed.MarkDefaults();
    Matrix.Changed.MarkDefaults();
    EulerRotation.Changed.MarkDefaults();
    
	Local.Method = [this] (auto& local) {
        Matrix4x4 rotation;
        Rotation().ToRotationMatrix(rotation);

        Matrix4x4 translation;
        translation.InitTranslation(Position);

        Vector3 anchor = Anchor;
        //local.InitTranslation(-anchor);
        local.InitScale(1.0f);
        local *= translation;
        local *= Matrix;
        local *= rotation;
        local *= Matrix4x4::CreateScale(Scale);
        Matrix4x4 anchorMatrix;
        anchorMatrix.InitTranslation(-anchor);
        local*=anchorMatrix;
    };
    
    WorldInverse.Method = [this] (auto& worldInverse) {
        worldInverse = World().Invert();
    };
    
    ResetWorldCalculation();
}

void Transform::HookOther( Transform* other ) {
	other->World.HasBecomeDirty.Bind(this, &Transform::OtherWorldChanged);
}

void Transform::UnHookOther( Transform* other ) {
	other->World.HasBecomeDirty.Unbind(this, &Transform::OtherWorldChanged);
}

void Transform::PropertyChanged() {
    Local.MakeDirty();
    World.MakeDirty();
    WorldInverse.MakeDirty();
}

void Transform::OtherWorldChanged() {
	World.MakeDirty();
	WorldInverse.MakeDirty();
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

void Transform::ResetWorldCalculation() {
    World.Method = [this] (auto& world) {
        world = Local;
    };
}