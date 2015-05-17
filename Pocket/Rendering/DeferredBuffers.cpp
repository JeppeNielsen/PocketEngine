//
//  DeferredBuffers.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/16/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "DeferredBuffers.hpp"
#include "FileReader.hpp"

#include "Window.hpp"
#include "LodePNG.h"

using namespace Pocket;

DeferredBuffers::DeferredBuffers() {

}

DeferredBuffers::~DeferredBuffers() {
    Destroy();
}

void DeferredBuffers::Initialize() {
    
     /* Create framebuffer */
    ASSERT_GL(glGenFramebuffers(1, &gbuffer_framebuffer));

    ASSERT_GL(glGenTextures(2, gbuffer));
    for(int i=0;i<2;i++) {
        ASSERT_GL(glBindTexture(GL_TEXTURE_2D, gbuffer[i]));
        ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    }
    ASSERT_GL(glGenTextures(1, &depth_buffer));
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, depth_buffer));
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, 0));
    
    width = 0;
    height = 0;
}

void DeferredBuffers::Destroy() {
    ASSERT_GL(glDeleteTextures(2, gbuffer));
    ASSERT_GL(glDeleteTextures(1, &depth_buffer));
}

void DeferredBuffers::Resize(int w, int h) {
    if (width == w && height == h) return;
    width = w;
    height = h;
    
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, gbuffer[0]));
    ASSERT_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, gbuffer[1]));
    ASSERT_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

    /* Depth texture */
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, depth_buffer));
    ASSERT_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0));

    /* Framebuffer */
    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_framebuffer));
    ASSERT_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbuffer[0], 0));
    ASSERT_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbuffer[1], 0));
    ASSERT_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer, 0));

    GLenum framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
       // system_log("%s:%d Framebuffer error: %s\n", __FILE__, __LINE__, _glStatusString(framebuffer_status));
       std::cout<<"Framebuffer error:"<<__FILE__<<" "<<__LINE__<<" : "<<_glStatusString(framebuffer_status)<<std::endl;
    }

    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, 0));
}

void DeferredBuffers::BeginGeometry() {

    GLenum buffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
    };
    
    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_framebuffer));
    GLenum framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(framebuffer_status != GL_FRAMEBUFFER_COMPLETE) {
       std::cout<<"Framebuffer error:"<<__FILE__<<" "<<__LINE__<<" : "<<_glStatusString(framebuffer_status)<<std::endl;
    }
    ASSERT_GL(glDrawBuffers(2, buffers));
    ASSERT_GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    ASSERT_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    ASSERT_GL(glDepthMask(GL_TRUE));
    ASSERT_GL(glDepthFunc(GL_LESS));
    ASSERT_GL(glCullFace(GL_BACK));

}

void DeferredBuffers::BeginLights() {

    GLenum buffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
    };

    ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, Window::Framebuffer));
    ASSERT_GL(glDrawBuffers(1, buffers));
    ASSERT_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer, 0));
    ASSERT_GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    ASSERT_GL(glClear(GL_COLOR_BUFFER_BIT));

    ASSERT_GL(glEnable(GL_BLEND));
    ASSERT_GL(glBlendFunc(GL_ONE, GL_ONE));
    ASSERT_GL(glCullFace(GL_BACK));
    ASSERT_GL(glDepthMask(GL_FALSE));
    //ASSERT_GL(glDepthFunc(GL_GEQUAL));
    //ASSERT_GL(glDepthFunc(GL_LEQUAL));
    ASSERT_GL(glDepthFunc(GL_ALWAYS));
    
    int ii;
    for(ii=0;ii<2;++ii) {
        ASSERT_GL(glActiveTexture(GL_TEXTURE0+ii));
        ASSERT_GL(glBindTexture(GL_TEXTURE_2D, gbuffer[ii]));
    }
    ASSERT_GL(glActiveTexture(GL_TEXTURE0+ii));
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, depth_buffer));
}

void DeferredBuffers::End() {
    ASSERT_GL(glActiveTexture(GL_TEXTURE0));
    ASSERT_GL(glDisable(GL_BLEND));
    ASSERT_GL(glDepthMask(GL_TRUE));
    ASSERT_GL(glDepthFunc(GL_LESS));
    ASSERT_GL(glCullFace(GL_BACK));
}

void DeferredBuffers::SaveBuffer(std::string filename) {
    //GLubyte* pixels = (GLubyte*) malloc(width * height * sizeof(GLubyte) * 4);
    std::vector<GLubyte> pixels;
    pixels.resize(width * height * sizeof(GLubyte) * 4);
    
    //ASSERT_GL(glBindTexture(GL_TEXTURE_2D, gbuffer[0]));
    ASSERT_GL(glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));

    LodePNG::Encoder encoder;
    std::vector<unsigned char> image;
    encoder.encode(image, pixels, width, height);
    
    std::string output = FileReader::GetWritablePath() +"/"+ filename;
    std::cout<<"Writing file : "<<output<<std::endl;
    LodePNG::saveFile(image, output);
}



