//
//  DeferredShader.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/16/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Shader.hpp"

namespace Pocket {
	class DeferredLight : public Shader {
	public:
        
		void Initialize();
        void SetViewPort(Vector2 viewportSize);
        void SetLightProperties(Vector3 position, Colour color, float size, float brightness);
        void SetInverseProjection(const float* inverseProjection, const float* view);
        
	protected:

        void OnInitBatch(const VisibleObject* visibleObject);

	private:
        
        GLuint positionSlot;
        
		GLuint viewProjectionSlot;
		GLuint colorBuffer;
        GLuint normalBuffer;
        GLuint depthBuffer;
        GLuint viewport;
        GLuint lightColor;
        GLuint lightPosition;
        GLuint lightSize;
        GLuint lightBrightness;
        GLuint inverseProjection;
        GLuint viewSlot;
        
        int vertexStride;
	};
}
