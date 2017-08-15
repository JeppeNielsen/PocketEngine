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

void InputDevice::SwallowTouch(int index, int depth) {
    if (index<0 || index>=maxTouches) return;
    if (depth>currentTouches[index].swallowedDepth) {
        currentTouches[index].swallowedDepth = depth;
    }
}

bool InputDevice::IsTouchSwallowed(int index, int depth) {
    if (index<0 || index>=maxTouches) return false;
    return depth<currentTouches[index].swallowedDepth;
}

void InputDevice::SetButton(const std::string& button, bool isDown, ModifierKey modifierKey) {
	if (isDown) {
		currentButtons[button] = modifierKey;
	} else {
		Buttons::iterator it = currentButtons.find(button);
		if (it!=currentButtons.end()) currentButtons.erase(it);
	}
}

void InputDevice::ReleaseAllButtons() {
	currentButtons.clear();
}

void InputDevice::StartFrame(IInputManagerIterator* inputManagers) {

    

    for (unsigned i=0; i<currentTouches.size(); i++) {
        Touch& current = currentTouches[i];
        Touch& prev = previousTouches[i];
        if (current.IsDown!=prev.IsDown) {
            if (current.IsDown) {
                current.swallowedDepth = -1;
            }
        }
    }

    if (inputManagers) {
        inputManagers->UpdateInput(this);
    }
}

void InputDevice::EndFrame() {
    previousTouches = currentTouches;
	previousButtons = currentButtons;
    scrollValues.clear();
}

void InputDevice::UpdateInputManager(Pocket::InputManager *inputManager) {
    inputManager->device = this;
    
    for (unsigned i=0; i<currentTouches.size(); i++) {
        const Touch& current = currentTouches[i];
        const Touch& prev = previousTouches[i];
        if (current.IsDown!=prev.IsDown) {
            if (current.IsDown) {
                inputManager->TouchDown(TouchEvent(i, inputManager->transformationMatrix.TransformPosition(current.Position)));
            } else {
                inputManager->TouchUp(TouchEvent(i, inputManager->transformationMatrix.TransformPosition(current.Position)));
            }
        }
    }
    
    for (Buttons::iterator it = currentButtons.begin(); it!=currentButtons.end(); ++it) {
        bool isDown = previousButtons.find(it->first)==previousButtons.end();
        if (isDown) inputManager->ButtonDown({ it->first, it->second });
    }
    
    for (Buttons::iterator it = previousButtons.begin(); it!=previousButtons.end(); ++it) {
        bool isUp = currentButtons.find(it->first)==currentButtons.end();
        if (isUp) inputManager->ButtonUp({ it->first, it->second });
    }
    
    updating = true;
    inputManager->KeyboardActive = KeyboardActive;
    inputManager->KeyboardText = KeyboardText;
    updating = false;
    
    inputManager->GamePad.Update();
    
    for(float v : scrollValues) {
        inputManager->ScrollChanged(v);
    }
}

void InputDevice::SetKeyboard(std::string text, bool active) {
    if (updating) return;
    KeyboardChanged({text, active});
}

void InputDevice::SetScroll(float delta) {
    scrollValues.push_back(delta);
}
