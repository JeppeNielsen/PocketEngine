#pragma once
#include "Shader.hpp"
#include "Property.hpp"
#include "TextureComponent.hpp"
#include "BoundingFrustum.hpp"
#include "Terrain.hpp"

namespace Pocket {
	class ShaderTerrain : public Shader {
	public:
        
		void Initialize();
		void Apply(VisibleObject* visibleObject);
        void Bind();
        
        bool RenderVertices(const VisibleObject* visibleObject, const Mesh* mesh, VertexRenderer& renderer);
        
        Vector3 LightDirection;
		Colour LightColor;
		float LightIntensity;
        Colour AmbientColor;
        float AmbientIntensity;
        
	protected:

		void OnStart(float* viewProjection);
        void OnInitBatch(const VisibleObject* visibleObject);

	private:

		bool EvaluateCriterion(const Terrain& terrain, float halfU, float halfV, float u1, float u2, int level, const Vector3& camPosition);
        void Draw(const Terrain& terrain, const BoundingFrustum& frustum, const Vector3& camPosition, float u1, float v1, float u2, float v2, float scale, int level, const VisibleObject* visibleObject, const Mesh* mesh, VertexRenderer& renderer);
        
        
        GLuint positionSlot;
        GLuint textureCoordSlot;
		GLuint colorSlot;
        GLuint normalSlot;
        
        
		GLuint viewProjectionSlot;
        GLuint textureSlot[4];
        
        GLuint lightDirectionSlot;
        GLuint lightColorSlot;
        GLuint ambientColorSlot;
        
        int vertexStride;
        
        Matrix4x4* world;
        int chunks;
    };
}

