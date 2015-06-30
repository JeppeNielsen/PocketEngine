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
	class DeferredShader : public Shader {
	public:
        
		void Initialize();
        
        const float* viewMatrix;
        
	protected:

        void OnInitBatch(const VisibleObject* visibleObject);

	private:
        
        GLuint positionSlot;
		GLuint textureCoordSlot;
		GLuint colorSlot;
        GLuint normalSlot;
        
		GLuint viewProjectionSlot;
		GLuint textureSlot;
        GLuint viewSlot;
        
        int vertexStride;
	};
}
