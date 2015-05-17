//
//  ShaderLitUncoloredTexturedBatched.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#include "ShaderLitUncoloredTexturedBatched.hpp"
using namespace Pocket;
using namespace std;

void ShaderLitUncoloredTexturedBatched::Initialize() {
    
    LightDirection = Vector3(0,-1,-1).Normalized();
	LightColor = Colour::White();
    LightIntensity = 1.0f;
	AmbientColor = Colour(0.6f,0.6f,0.6f,1.0f);
    AmbientIntensity = 1.0f;
    
	string vertexShader =
#ifdef OPENGL_DESKTOP
    "#version 120 \n"
#endif
    "attribute vec4 Position;\n"
    "attribute vec2 TexCoords;\n"
    "attribute vec4 Color;\n"
    "attribute vec3 Normal;\n"
    "uniform mat4 ViewProjection;\n"
    "varying vec2 DestinationTexCoords;\n"
    "varying vec4 DestinationColor;\n"
    "varying vec3 DestinationNormal;\n"
    "void main(void) {\n"
    "	DestinationTexCoords = TexCoords;\n"
    "	DestinationColor = Color;\n"
    "	DestinationNormal = normalize(Normal);\n"
    "	gl_Position = Position * ViewProjection;\n"
    "}\n";
	
	string fragmentShader =
    
#ifdef OPENGL_EMBEDDED
    "precision mediump float;"
    "varying lowp vec2 DestinationTexCoords;"
    "varying lowp vec4 DestinationColor;"
    "varying lowp vec3 DestinationNormal;"
#endif
#ifdef OPENGL_DESKTOP
    "#version 120 \n"
    "varying vec2 DestinationTexCoords;"
    "varying vec4 DestinationColor;"
    "varying vec3 DestinationNormal;"
#endif
    "uniform vec3 LightDirection;"
    "uniform vec4 LightColor;"
    "uniform vec4 AmbientColor;"
    "uniform sampler2D Texture;"
    "void main(void) {"
    "   float lightIntensity = max(dot(LightDirection, DestinationNormal), 0.0); "
    "	gl_FragColor = texture2D(Texture, DestinationTexCoords) * DestinationColor * (AmbientColor + LightColor * lightIntensity);"
    "}";
    
	if (!shader.CreateShaders(vertexShader, fragmentShader)) return;
    
	positionSlot = shader.CreateAttribVariable("Position");
	textureCoordSlot = shader.CreateAttribVariable("TexCoords");
	colorSlot = shader.CreateAttribVariable("Color");
	normalSlot = shader.CreateAttribVariable("Normal");
    
	viewProjectionSlot = shader.CreateUniformVariable("ViewProjection");
	textureSlot = shader.CreateUniformVariable("Texture");
	lightDirectionSlot = shader.CreateUniformVariable("LightDirection");
	lightColorSlot = shader.CreateUniformVariable("LightColor");
	ambientColorSlot = shader.CreateUniformVariable("AmbientColor");
    
    vertexStride = sizeof(Vertex);
    
    lastTexture = 0;
}

void ShaderLitUncoloredTexturedBatched::OnStart(float* viewProjection) {
    glUniformMatrix4fv(viewProjectionSlot, 1, GL_FALSE, viewProjection);
    
    glUniform1i(textureSlot, 0);
    
    glUniform3f(lightDirectionSlot, LightDirection.x, LightDirection.y, LightDirection.z);
	glUniform4f(lightColorSlot, LightColor.R() * LightIntensity, LightColor.G()* LightIntensity, LightColor.B()* LightIntensity, LightColor.A());
	glUniform4f(ambientColorSlot, AmbientColor.R() * AmbientIntensity, AmbientColor.G() * AmbientIntensity, AmbientColor.B() * AmbientIntensity, AmbientColor.A());
    
    lastTexture = 0;
}

void ShaderLitUncoloredTexturedBatched::Bind() {
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(textureCoordSlot, 2, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)12);
	glVertexAttribPointer(colorSlot, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
	glVertexAttribPointer(normalSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)24);
}

void ShaderLitUncoloredTexturedBatched::Apply(VisibleObject *visibleObject) {
    
    if (visibleObject->texture!=lastTexture) {
        lastTexture = visibleObject->texture;
        glBindTexture(GL_TEXTURE_2D, lastTexture->Texture().GetHandle());
    }
}
