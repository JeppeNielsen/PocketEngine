//
//  SpineAnimation.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 05/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <map>
#include <string>
#include <vector>
#include "Vector2.hpp"
#include "Timeline.hpp"

namespace Pocket {
    class SpineAnimation {
    public:
        struct SlotFrame {
            float time;
            std::string attachment;
        };
        
        typedef Timeline<SlotFrame> SlotTimeline;
        typedef std::map<std::string, SlotTimeline> Slots;
        Slots slots;
        
        struct RotateFrame {
            float time;
            float rotation;
        };
        
        struct Vector2Frame {
            float time;
            Vector2 value;
        };
        
        typedef Timeline<RotateFrame> RotateTimeline;
        typedef Timeline<Vector2Frame> Vector2Timeline;
        
        struct Bone {
            RotateTimeline rotation;
            Vector2Timeline position;
            Vector2Timeline scale;
        };
        
        typedef std::map<std::string, Bone> Bones;
        Bones bones;
        
        void CalcStartEndTime();
        
        float startTime;
        float endTime;
        
    private:
        template<class T>
        void CalcMinMaxTime(Timeline<T>& timeline, float& min, float& max);
    };

    typedef std::map<std::string, SpineAnimation> SpineAnimationCollection;

    template <class T>
    void SpineAnimation::CalcMinMaxTime(Timeline<T> &timeline, float &startTime, float &endTime) {
        float timelineMin = timeline.StartTime();
        float timelineMax = timeline.EndTime();
        if (timelineMin<startTime) {
            startTime = timelineMin;
        }
        if (timelineMax>endTime) {
            endTime = timelineMax;
        }
    }
}

