//
//  Clipper.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/13/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
namespace Pocket {
    class Clipper {
    public:
        Clipper();
        ~Clipper();
        
        void PushBegin();
        void PushEnd();
        void PopBegin();
        void PopEnd();
        
        bool UseDepth;
        
    private:
        int stackCount;
        
    };
}