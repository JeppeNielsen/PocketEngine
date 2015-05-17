//
//  BigButtonManager.h
//  QuizMaster
//
//  Created by Jeppe Nielsen on 3/31/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Event.hpp"
#include <set>
#include <map>
#include "Thread.hpp"
#include "Timer.hpp"

namespace Pocket {
  
    enum class BigButtonButton {
        Back,
        Start,
        Right,
        Left,
        Down,
        Up,
        X,
        Y,
        B,
        A,
        BigButton,
        Xbox
    };
  
  struct BigButtonEvent {
    int PlayerIndex;
    BigButtonButton Button;
    inline bool operator ==(const BigButtonEvent &other) const{
        return !(PlayerIndex!=other.PlayerIndex || Button!=other.Button);
    }
        
    inline  bool operator !=(const BigButtonEvent &other) const{
        return (PlayerIndex!=other.PlayerIndex || Button!=other.Button);
    }
    
    inline  bool operator < (const BigButtonEvent &other) const{
        if (PlayerIndex<other.PlayerIndex) {
            return true;
        } else if (PlayerIndex>other.PlayerIndex) {
            return false;
        } else {
            if (Button == other.Button) {
                return false;
            } else {
                int one = (int)Button;
                int two = (int)other.Button;
                return one<two;
            }
        }
    }
  };
    
  class BigButtonManager {
    public:
        BigButtonManager();
        ~BigButtonManager();
    
        bool Initialize();
        void Destroy();
        void Update(float dt);
    
        Event<BigButtonEvent> ButtonDown;
        Event<BigButtonEvent> ButtonUp;
      
        void Print(int result);
      
      private:
      
        typedef std::set<BigButtonEvent> ButtonDowns;
      
        struct Controller {
            bool buttonsChanged;
            ButtonDowns downs;
            ButtonDowns currentDowns;
            ButtonDowns previousDowns;
            float timeSinceButtonClick;
            
            void Update(BigButtonManager* manager, float dt);
        };
      
        void* device;
        void* interface;
      
        char inputData[5];
        Thread thread;
        Controller controller[4];
      
        void PollLoop(Thread* thread);
        void UpdateButtonStates();
        void SetButtonDown(int playerIndex, BigButtonButton button, bool isDown);
    };
}