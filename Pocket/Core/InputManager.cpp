#include "InputManager.hpp"
#include "InputDevice.hpp"

using namespace Pocket;

InputManager::InputManager() : KeyboardActive(this), KeyboardText(this) {
    KeyboardActive.Changed += event_handler(this, &InputManager::KeyboardChanged);
    KeyboardText.Changed += event_handler(this, &InputManager::KeyboardChanged);
    GamePad.Initialize();
}

InputManager::~InputManager() { }

const Vector2& InputManager::GetTouchPosition(int index) {
	return device->GetTouchPosition(index);
}

void InputManager::KeyboardChanged(InputManager *inputManager) {
    device->SetKeyboard(KeyboardText, KeyboardActive);
}