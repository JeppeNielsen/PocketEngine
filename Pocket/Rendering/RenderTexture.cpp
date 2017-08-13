//
//  RenderTexture.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "RenderTexture.hpp"
#include "OpenGL.hpp"

using namespace Pocket;

RenderTexture::RenderTexture() : framebuffer(0), texure(0), old_fbo(0) {}
RenderTexture::~RenderTexture() { Free(); }

void RenderTexture::Initialize(int width, int height) {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        
    // The texture we're going to render to
    glGenTextures(1, &texure);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texure);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texure, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
    glBindTexture(GL_TEXTURE_2D, 0);
        
}

void RenderTexture::Free() {
    if (framebuffer) {
        glDeleteRenderbuffers(0, &framebuffer);
        framebuffer = 0;
    }
    
    if (texure) {
        glDeleteTextures(0, &texure);
        texure = 0;
    }
}

void RenderTexture::Begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void RenderTexture::End() {
    glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

GLuint RenderTexture::GetTexture() const {
    return texure;
}
