#pragma once
#include "Event.hpp"
#include "Vector2.hpp"
#include <string>
#include "Property.hpp"
#include <string>
#include "GamePadManager.hpp"

namespace Pocket {
    
	struct TouchEvent {
		TouchEvent(int index, Vector2 position) :Index(index), Position(position) {}
		int Index;
		Vector2 Position;
	};

	class InputDevice;
	class InputManager {
	public:
		InputManager();
		~InputManager();

		Event<std::string> ButtonDown;
		Event<std::string> ButtonUp;

		Event<TouchEvent> TouchDown;
		Event<TouchEvent> TouchUp;

		const Vector2& GetTouchPosition(int index);
        
        Property<InputManager*, bool> KeyboardActive;
        Property<InputManager*, std::string> KeyboardText;
        
        GamePadManager GamePad;
       
	private:
		InputDevice* device;
		friend class InputDevice;
        
        void KeyboardChanged(InputManager* inputManager);
        
	};
    
    class IInputManagerUpdater {
    public:
        virtual void UpdateInputManager(InputManager* inputManager) = 0;
    };
    
    class IInputManagerIterator {
    public:
        virtual void UpdateInput(IInputManagerUpdater* inputManagerUpdater) = 0;
    };
    
    
}