#pragma once
#include "Vector2.hpp"
#include <vector>
#include "InputManager.hpp"
#include <map>
#include <string>
#include "Property.hpp"

namespace Pocket {
    

    
	class InputDevice : public IInputManagerUpdater
	{
	public:
		InputDevice();
		~InputDevice();

		void Initialize(int maxTouches);

		struct Touch {
			Touch() : IsDown(false), Position(0) {}
			bool IsDown;
			Vector2 Position;
		};

		void SetTouchPosition(int index, const Vector2& position);
		void SetTouch(int index, bool isDown, const Vector2& position);
		const Vector2& GetTouchPosition(int index);

		void Update(IInputManagerIterator* inputManagers);

		void SetButton(const std::string& button, bool isDown);
		void ReleaseAllButtons();
        
        bool KeyboardActive;
        std::string KeyboardText;
        
        void SetKeyboard(std::string text, bool active);
        
        struct KeyboardEventData {
            std::string text;
            bool active;
        };
        
        Event<KeyboardEventData> KeyboardChanged;
        
        void UpdateInputManager(InputManager* inputManager);

	private:

		typedef std::vector<Touch> Touches;

		Touches currentTouches;
		Touches previousTouches;

		typedef std::map<std::string, int> Buttons;

		Buttons currentButtons;
		Buttons previousButtons;

		int maxTouches;
        
        Vector2 zero;
        
        bool updating;

	};
}

