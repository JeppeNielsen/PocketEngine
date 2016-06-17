//
//  Sound.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/30/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//


#include "Sound.hpp"
#include "FileReader.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include "File.hpp"

using namespace Pocket;

Sound::Sound() :
channel(0), sampleRate(0), bps(0), size(0), alBuffer(0), bufferDirty(false), soundData(0)
{ }

Sound::~Sound() {
    Reset();
}

void Sound::Reset() {
    if (alBuffer) alDeleteBuffers(1, &alBuffer);
    delete [] soundData;
    channel = 0;
    sampleRate = 0;
    bps = 0;
    size = 0;
    alBuffer = 0;
    bufferDirty = false;
    soundData = 0;
}

bool Sound::LoadFromWav(std::string filename) {
    soundData = LoadWAV(filename, channel, sampleRate, bps, size);
    if (!soundData) {
        std::cout << " '" << filename << "' is not a valid WAVE file"  << std::endl;
        return false;
    }
    
    CreateALBuffer();
    
    return true;
}

bool Sound::isBigEndian()
{
    int a=1;
    return !((char*)&a)[0];
}

int Sound::convertToInt(char* buffer,int len)
{
    int a=0;
    if(!isBigEndian())
        for(int i=0;i<len;i++)
            ((char*)&a)[i]=buffer[i];
    else
        for(int i=0;i<len;i++)
            ((char*)&a)[3-i]=buffer[i];
    return a;
}

char* Sound::LoadWAV(std::string filename, int& chan, int& samplerate, int& bps, int& size)
{
    File file;
    if (!file.Load(filename)) {
        return 0;
    }

    char buffer[4];

    //std::istringstream in;
    
    std::istringstream in(std::string((char*)file.Data(), file.Size()));
    
    //in.rdbuf()->pubsetbuf((char*)file.Data(),file.Size());

    //std::ifstream in(filename,std::ios::binary);
    in.read(buffer,4);
    if(strncmp(buffer,"RIFF",4)!=0)
    {
        
        return NULL;
    }
    in.read(buffer,4);
    in.read(buffer,4);	//WAVE
    in.read(buffer,4);	//fmt
    in.read(buffer,4);	//16
    in.read(buffer,2);	//1
    in.read(buffer,2);
    chan=convertToInt(buffer,2);
    in.read(buffer,4);
    samplerate=convertToInt(buffer,4);
    in.read(buffer,4);
    in.read(buffer,2);
    in.read(buffer,2);
    bps=convertToInt(buffer,2);
    in.read(buffer,4);	//data
    in.read(buffer,4);
    size=convertToInt(buffer,4);
    char* data=new char[size];
    in.read(data,size);
    return data;
}

SoundFormat::Enum Sound::GetFormat() {
    return channel == 1 ?
    (bps == 8 ? SoundFormat::Mono8 : SoundFormat::Mono16)
    :
    (bps == 8 ? SoundFormat::Stereo8 : SoundFormat::Stereo16);
}

void Sound::CreateALBuffer() {
    alGenBuffers(1, &alBuffer);
    if (alBuffer) bufferDirty = true;
}

ALuint Sound::GetOpenALBuffer() {
    
    if (bufferDirty) {
        ALenum alSoundFormat;
    
        switch (GetFormat()) {
            case SoundFormat::Mono8: alSoundFormat = AL_FORMAT_MONO8; break;
            case SoundFormat::Mono16: alSoundFormat = AL_FORMAT_MONO16; break;
            case SoundFormat::Stereo8: alSoundFormat = AL_FORMAT_STEREO8; break;
            case SoundFormat::Stereo16: alSoundFormat = AL_FORMAT_STEREO16; break;
            default: alSoundFormat = 0; break;
        }
        
        alBufferData(alBuffer,alSoundFormat,soundData,size,sampleRate);
        bufferDirty = false;
    }
    
    return alBuffer;
}

char* Sound::SoundData() {
    bufferDirty = true;
    return soundData;
}
