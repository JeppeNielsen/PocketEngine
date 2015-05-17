//
//  ShaderLitUncoloredTexturedBatched.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/10/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Shader.hpp"
#include "Property.hpp"
#include "TextureComponent.hpp"
#include "Colour.hpp"

namespace Pocket {
	class ShaderLitUncoloredTexturedBatched : public Shader
	{
	public:
		void Initialize();
        void Apply(VisibleObject* visibleObject);
        
		Vector3 LightDirection;
		Colour LightColor;
		float LightIntensity;
        Colour AmbientColor;
        float AmbientIntensity;
        
        
        void Bind();
        
	protected:
        
		void OnStart(float* viewProjection);
        
	private:
        
		GLuint positionSlot;
		GLuint textureCoordSlot;
		GLuint colorSlot;
		GLuint normalSlot;
        
		GLuint viewProjectionSlot;
		GLuint textureSlot;
		GLuint lightDirectionSlot;
		GLuint lightColorSlot;
		GLuint ambientColorSlot;
        
        TextureComponent* lastTexture;
        
        int vertexStride;
	};
}

