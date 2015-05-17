#pragma once
#include "Shader.hpp"
#include "Property.hpp"
#include "TextureComponent.hpp"

namespace Pocket {
	class ShaderUnlitUncolored : public Shader {
	public:
        
		void Initialize();
		void Apply(VisibleObject* visibleObject);
        void Bind();

	protected:

		void OnStart(float* viewProjection);
        void OnInitBatch(const VisibleObject* visibleObject);
        
	private:

		GLuint positionSlot;
		GLuint colorSlot;

		GLuint viewProjectionSlot;
		GLuint worldSlot;
        
        GLuint positionSlot_batched;
		GLuint colorSlot_batched;
        
		GLuint viewProjectionSlot_batched;
		GLuint worldSlot_batched;
        
        int vertexStride;
	};
}

