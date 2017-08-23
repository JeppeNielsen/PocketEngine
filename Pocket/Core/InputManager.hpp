#pragma once
#include "Event.hpp"
#include "Vector2.hpp"
#include <string>
#include "Property.hpp"
#include <string>
#include "GamePadManager.hpp"
#include "Matrix4x4.hpp"

namespace Pocket {

    enum class ModifierKey {
        None,
        Shift,
        Alt,
        Command,
        Ctrl,
        Fn
    };
    
	struct TouchEvent {
		TouchEvent(int index, Vector2 position) :Index(index), Position(position) {}
		int Index;
		Vector2 Position;
	};
    
    struct ButtonEvent {
        std::string Id;
        ModifierKey modifierKey;
    };

	class InputDevice;
	class InputManager {
	public:
		InputManager();
		~InputManager();

		Event<ButtonEvent> ButtonDown;
		Event<ButtonEvent> ButtonUp;

		Event<TouchEvent> TouchDown;
		Event<TouchEvent> TouchUp;

		const Vector2 GetTouchPosition(int index);
        
        Property<bool> KeyboardActive;
        Property<std::string> KeyboardText;
        
        GamePadManager GamePad;
        
        Event<float> ScrollChanged;
       
        void SwallowTouch(int index, int depth);
        bool IsTouchSwallowed(int index, int depth);
        
        InputDevice* GetDevice();
                
	private:
		InputDevice* device;
		friend class InputDevice;
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
