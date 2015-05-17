#pragma once
#include "OpenGL.hpp"
#include <string>
#include <map>
#include "ShaderProgram.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "VisibleObject.hpp"
#include "BoundingFrustum.hpp"
#include "Camera.hpp"
#include "VertexRenderer.hpp"

namespace Pocket {
    class VisibleObject;
	class Shader
	{
	public:
		Shader();
		virtual ~Shader();

		virtual void Initialize();

        void Start(float* viewProjection);
        virtual void Apply(VisibleObject* visibleObject);
        virtual void Bind();
        
        virtual void BindTexture(Texture* texture);
        
        void InitBatching(const VisibleObject* visibleObject);
        
        virtual bool RenderVertices(const VisibleObject* visibleObject, const Mesh* mesh, VertexRenderer& renderer);
        
        GameObject* cameraObject;
        Camera* camera;
        const BoundingFrustum* frustum;
        const float* viewProjection;
        
	protected:

		virtual void OnStart(float* viewProjection);
        virtual void OnInitBatch(const VisibleObject* visibleObject);

		ShaderProgram shader;
        ShaderProgram shaderBatching;
        
	};
}