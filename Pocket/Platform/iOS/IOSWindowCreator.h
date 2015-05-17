//
//  IOSWindowCreator.h
//  Nano
//
//  Created by Jeppe Nielsen on 3/10/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Event.hpp"
#include "InputDevice.hpp"
#include <map>

namespace Nano {
    class IOSWindowCreator {
      
    public:
        static IOSWindowCreator* Instance();
        
        void Create();
        
        
        Event<void*> OnInitialize;
        Event<bool> OnUpdate;
        
        InputDevice inputDevice;
        
        void Down(int hash, int x, int y);
        void Up(int hash, int x, int y);
        void Move(int hash, int x, int y);
        
        bool isLandscape;
                
    private:
        static IOSWindowCreator* instance;
        
        IOSWindowCreator();
        ~IOSWindowCreator();
        
        typedef std::map<int, int> HashToID;
        
        HashToID hashToID;
        
        int CreateIndex();
        bool IndexExists(int index);
    
    };
}

