#pragma once
#include "GameComponent.hpp"
#include "DirtyProperty.hpp"
#include "BoundingBox.hpp"
#include "Vertex.hpp"
#include <vector>
#include "Matrix3x3.hpp"
#include "Matrix4x4.hpp"
#include "OpenGL.hpp"
#include "Box.hpp"

namespace Pocket {
    class Ray;
    class TextureAtlas;
    class Texture;
    
    class Mesh;
    class MeshIntersector {
        public:
        virtual bool IntersectsRay(Mesh& mesh, const Ray& ray,
                                float* pickDistance, float* barycentricU, float* barycentricV,
                                size_t* triangleIndex, Vector3* normal) = 0;
    };
    
    Component(Mesh)
	public:
		Mesh();
		~Mesh();
        
		typedef std::vector<Vertex> VerticesList;
		typedef std::vector<short> TrianglesList;
		
		VerticesList& Vertices();
		TrianglesList& Triangles();

		virtual const VerticesList& ReadOnlyVertices();
		virtual const TrianglesList& ReadOnlyTriangles();

		DirtyProperty<Mesh*, BoundingBox> LocalBoundingBox;

		void AddQuad(Vector2 center, Vector2 size, Colour color);
        void AddTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Colour color);
		void AddTexturedQuad(Texture* texture, const Box& textureCoords, const Matrix3x3& transform);
		void AddTexturedQuad(TextureAtlas* atlas, const std::string& name);
		void AddTexturedQuad(TextureAtlas* atlas, const std::string& name, float scale);
		void AddTexturedQuad(TextureAtlas* atlas, const std::string& name, float scale, Vector2 center, float rotation);

		void SetColor(const Colour& color);
		void SetAlpha(const float& alpha);
        
        void Clear();

        void AddCube(Vector3 center, Vector3 size);
		void AddCube(Vector3 center, Vector3 size, Colour color);
    
        void AddPlane(const Vector3& center, const Vector2& size, const Box& textureCoords);
        void AddPlane(const Vector3& center, const Vector2& size, const Box& textureCoords, const Colour& color);

		void AddTriangle(const Vector3& p1, const Vector3& p2, const Vector3& p3,
			const Vector3& n1, const Vector3& n2, const Vector3& n3,
			const Vector2& texCoord1, const Vector2& texCoord2, const Vector2& texCoord3, 
			const Colour& color, const Matrix4x4& world);
        
        void AddSegmentedQuad(const Matrix4x4& world, const Vector2& size, const Vector2& blTexCoord, const Vector2& trTexCoord, int segments);
        
        void AddGeoSphere(const Vector3 center, float radius, int segments);
        
        bool IntersectsRay(const Ray& ray,
                           float* pickDistance, float* barycentricU, float* barycentricV,
                           size_t* triangleIndex, Vector3* normal);
        
        //void AddText(const Font& font, std::string text, Vector2 size, float fontSize, Font::HAlignment hAlign, Font::VAlignment vAlign, bool wordWrap, const Box& textureCoords);
    
        bool AddObjFile(std::string objFile);
    
        void Center();
    
    
        void Reset();
        void Clone(const Mesh& source);
    
        MeshIntersector* customIntersector;
    
        void Flip();
    
	public:
		VerticesList vertices;
		TrianglesList triangles;

		void Bind();
		void Draw();
    
        size_t VerticesSize() const;
        size_t TrianglesSize() const;
    
        void SetBufferSizes(size_t verticesSize, size_t trianglesSize);
    
        void RevertDefaultCalcBoundingBox();
    
    static bool RayIntersectsTriangle(const Ray& ray,
                               const Vector3& tri0, const Vector3& tri1, const Vector3& tri2,
                               float* pickDistance, float* barycentricU, float* barycentricV);

	private:
		void CalcBoundingBox(DirtyProperty<Mesh*, BoundingBox>::EventData& eventData);


		bool vertexBufferDirty;
		GLuint vertexBuffer;

		bool indexBufferDirty;
		GLuint indexBuffer;
    
    

        size_t customVerticesSize;
        size_t customTrianglesSize;
    
        
    };
}