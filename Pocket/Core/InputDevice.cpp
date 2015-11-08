#include "InputDevice.hpp"
using namespace Pocket;

InputDevice::InputDevice()
: KeyboardActive(false), KeyboardText(""), updating(false)
{
    zero = 0;
}

InputDevice::~InputDevice()
{
}

void InputDevice::Initialize(int maxTouches) {
	this->maxTouches = maxTouches;
	for (int i =0; i<maxTouches; i++)
	{
		currentTouches.push_back(Touch());
	}
	previousTouches = currentTouches;
}

void InputDevice::SetTouchPosition(int index, const Vector2& position) {
	currentTouches[index].Position = position;
}

const Vector2& InputDevice::GetTouchPosition(int index) {
	if (index<0 || index>=maxTouches) return zero;
	return currentTouches[index].Position;
}

void InputDevice::SetTouch(int index, bool isDown, const Vector2& position) {
	Touch& touch = currentTouches[index];
	touch.IsDown = isDown;
	touch.Position = position;
}



void InputDevice::SetButton(const std::string& button, bool isDown) {
	if (isDown) {
		currentButtons[button] = 0;
	} else {
		Buttons::iterator it = currentButtons.find(button);
		if (it!=currentButtons.end()) currentButtons.erase(it);
	}
}

void InputDevice::ReleaseAllButtons() {
	currentButtons.clear();
}

void InputDevice::Update(IInputManagerIterator* inputManagers) {

    updating = true;
    inputManagers->UpdateInput(this);
    updating = false;
    
    previousTouches = currentTouches;
	previousButtons = currentButtons;
}

void InputDevice::UpdateInputManager(Pocket::InputManager *inputManager) {
    inputManager->device = this;
    
    for (unsigned i=0; i<currentTouches.size(); i++) {
        const Touch& current = currentTouches[i];
        const Touch& prev = previousTouches[i];
        if (current.IsDown!=prev.IsDown) {
            if (current.IsDown) {
                inputManager->TouchDown(TouchEvent(i, current.Position));
            } else {
                inputManager->TouchUp(TouchEvent(i, current.Position));
            }
        }
    }
    
    for (Buttons::iterator it = currentButtons.begin(); it!=currentButtons.end(); ++it) {
        bool isDown = previousButtons.find(it->first)==previousButtons.end();
        if (isDown) inputManager->ButtonDown(it->first);
    }
    
    for (Buttons::iterator it = previousButtons.begin(); it!=previousButtons.end(); ++it) {
        bool isUp = currentButtons.find(it->first)==currentButtons.end();
        if (isUp) inputManager->ButtonUp(it->first);
    }
    
    inputManager->KeyboardActive = KeyboardActive;
    inputManager->KeyboardText = KeyboardText;
    
    inputManager->GamePad.Update();
    
    for(float v : scrollValues) {
        inputManager->ScrollChanged(v);
    }
    scrollValues.clear();
}

void InputDevice::SetKeyboard(std::string text, bool active) {
    if (updating) return;
    KeyboardChanged({text, active});
}

void InputDevice::SetScroll(float delta) {
    scrollValues.push_back(delta);
}
