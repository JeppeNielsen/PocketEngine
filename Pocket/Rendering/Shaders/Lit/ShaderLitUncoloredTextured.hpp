#pragma once
#include "Shader.hpp"
#include "Property.hpp"
#include "TextureComponent.hpp"
#include "Colour.hpp"

namespace Pocket {
	class ShaderLitUncoloredTextured : public Shader
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
		GLuint worldSlot;
		GLuint textureSlot;
		GLuint lightDirectionSlot;
		GLuint lightColorSlot;
		GLuint ambientColorSlot;
        
        TextureComponent* lastTexture;
        
        int vertexStride;
	};
}

