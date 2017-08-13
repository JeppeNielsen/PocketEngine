#include "InputManager.hpp"
#include "InputDevice.hpp"

using namespace Pocket;

InputManager::InputManager() : device(0) {
    auto keyBoardChanged = [this]() {
        device->SetKeyboard(KeyboardText, KeyboardActive);
    };

    KeyboardActive.Changed.Bind(keyBoardChanged);
    KeyboardText.Changed.Bind(keyBoardChanged);
    GamePad.Initialize();
    transformationMatrix = Matrix4x4::IDENTITY;
}

InputManager::~InputManager() {}

const Vector2 InputManager::GetTouchPosition(int index) {
	return transformationMatrix.TransformPosition(device->GetTouchPosition(index));
}

void InputManager::SwallowTouch(int index, int depth) {
    device->SwallowTouch(index, depth);
}

bool InputManager::IsTouchSwallowed(int index, int depth) {
    return device->IsTouchSwallowed(index, depth);
}

void InputManager::SetTransformationMatrix(const Matrix4x4& transformationMatrix) {
    this->transformationMatrix = transformationMatrix;
}

InputDevice* InputManager::GetDevice() {
    return device;
}
