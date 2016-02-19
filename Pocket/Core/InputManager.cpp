#include "InputManager.hpp"
#include "InputDevice.hpp"

using namespace Pocket;

InputManager::InputManager() {
    auto keyBoardChanged = [this](auto& v) {
        device->SetKeyboard(KeyboardText, KeyboardActive);
    };

    KeyboardActive.Changed.Bind(keyBoardChanged);
    KeyboardText.Changed.Bind(keyBoardChanged);
    GamePad.Initialize();
}

InputManager::~InputManager() { }

const Vector2& InputManager::GetTouchPosition(int index) {
	return device->GetTouchPosition(index);
}
