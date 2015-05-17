//
//  ShaderParticleSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/14/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Shader.hpp"
#include "Property.hpp"
#include "TextureComponent.hpp"

namespace Pocket {
	class ShaderParticleSystem : public Shader {
	public:
        
		void Initialize();
		void Apply(VisibleObject* visibleObject);
        void Bind();
        
        float Aspect;
        
        bool RenderVertices(const VisibleObject* visibleObject, const Mesh* mesh, VertexRenderer& renderer);
        
	protected:
        
		void OnStart(float* viewProjection);
        void OnInitBatch(const VisibleObject* visibleObject);
        
	private:
        
		GLuint positionSlot;
        GLuint sizeSlot;
		GLuint textureCoordSlot;
		GLuint colorSlot;
        
		GLuint viewProjectionSlot;
		GLuint worldSlot;
		GLuint textureSlot;
        GLuint aspectSlot;
        
        
        GLuint positionSlot_batched;
        GLuint sizeSlot_batched;
		GLuint textureCoordSlot_batched;
		GLuint colorSlot_batched;
        
		GLuint viewProjectionSlot_batched;
		GLuint textureSlot_batched;
        GLuint aspectSlot_batched;
        
        int vertexStride;
        TextureComponent* lastTexture;
	};
}

