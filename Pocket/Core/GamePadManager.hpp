//
//  GamePadManager.h
//  QuizMaster
//
//  Created by Jeppe Nielsen on 3/3/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <map>
#include "Event.hpp"
#include "Vector2.hpp"

namespace Pocket {

    enum class GamePadButton : int {
        ButtonA = 1,
        ButtonB = 2,
        ButtonX = 3,
        ButtonY = 4,
        LeftBumper = 5,
        RightBumper = 6,
        
        LeftStick = 7,
        RightStick = 8,
        
        Start = 9,
        Back = 10,
        GuideButton = 11,
        DpadUp = 12,
        DpadDown = 13,
        DpadLeft = 14,
        DpadRight = 15,
        
        LeftTrigger = 50,
        RightTrigger = 53
    };
        
    struct GamePadData {
        int GamePad;
        GamePadButton Button;
        Vector2 Direction;
    };
    
    class GamePadManager {
        public:
            GamePadManager();
            ~GamePadManager();
        
            void Initialize();
            void Destroy();
            void Print();
        
            void Update();
        
            Event<GamePadData> ButtonDown;
            Event<GamePadData> ButtonUp;
        
            Event<GamePadData> AnalogChanged;
        
            Event<int> GamePadPluggedIn;
            Event<int> GamePadUnplugged;
        
            void SetButtonState(int gamePadID, GamePadButton button, bool down);
            void SetAnalogState(int gamePadID, GamePadButton button, Vector2 value);
            Vector2 GetAnalogState(int gamePadID, GamePadButton button);
        
            void AddGamePad(int gamePadID);
            void RemoveGamePad(int gamePadID);
        
    private:
        
        struct GamePad {
            int index;
          
            typedef std::map<GamePadButton, bool> ButtonValues;
            ButtonValues buttonValues;
            ButtonValues previousButtonValues;
            
            typedef std::map<GamePadButton, Vector2> AnalogValues;
            AnalogValues analogValues;
            AnalogValues previousAnalogValues;
            
            void UpdateButtons(GamePadManager& manager);
            void UpdateAnalogs(GamePadManager& manager);
        };
        
        typedef std::map<int, GamePad> GamePads;
        GamePads gamePads;
        
        typedef std::vector<int> GamePadList;
        GamePadList addedGamePads;
        GamePadList removedGamePads;
        
        void UpdateButtons();
        void UpdateAnalogs();
        void UpdateAddedRemovedGamePads();
        
        void GamePadAdded(int gamePadID);
        void GamePadRemoved(int gamePadID);
        
    public:
        typedef std::map<int, int> Buttons;
        Buttons buttons;

    private:
        void* device;
        int gamePadIndexCounter;
    };
}