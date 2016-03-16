//
//  Sound.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <string>

namespace Pocket {
    
    struct SoundFormat {
        enum Enum {
            Mono8,
            Mono16,
            Stereo8,
            Stereo16
        };
    };
    
    class Sound {
    public:
    
        Sound();
        ~Sound();
        void Reset();
    
        bool LoadFromWav(std::string filename);
    
        char* SoundData();
    
        int GetSize();
    
        SoundFormat::Enum GetFormat();
        int GetSampleRate();
    
        ALuint GetOpenALBuffer();
    
    private:
        bool isBigEndian();
        int convertToInt(char* buffer,int len);
        char* LoadWAV(std::string filename, int& chan, int& samplerate, int& bps, int& size);
        void CreateALBuffer();
    
        char* soundData;
        int channel;
        int sampleRate;
        int bps;
        int size;
    
        bool bufferDirty;
        ALuint alBuffer;
    };
}