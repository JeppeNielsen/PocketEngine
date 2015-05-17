#pragma once
#include "Shader.hpp"
#include "Property.hpp"
#include "TextureComponent.hpp"

namespace Pocket {
	class ShaderUnlitUncoloredTextured : public Shader {
	public:
        
		void Initialize();
		void Apply(VisibleObject* visibleObject);
        void Bind();

	protected:

		void OnStart(float* viewProjection);
        void OnInitBatch(const VisibleObject* visibleObject);

	private:

		GLuint positionSlot;
		GLuint textureCoordSlot;
		GLuint colorSlot;

		GLuint viewProjectionSlot;
		GLuint worldSlot;
		GLuint textureSlot;
        
        
        GLuint positionSlot_batched;
		GLuint textureCoordSlot_batched;
		GLuint colorSlot_batched;
        
		GLuint viewProjectionSlot_batched;
		GLuint textureSlot_batched;
        
        
        int vertexStride;
        
        TextureComponent* lastTexture;
	};
}

