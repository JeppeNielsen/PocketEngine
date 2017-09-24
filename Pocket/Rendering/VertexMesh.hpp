//
//  VertexMesh.h
//  Shaders
//
//  Created by Jeppe Nielsen on 30/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "TextureAtlas.hpp"
#include "Quaternion.hpp"
#include "MathHelper.hpp"
#include "BoundingBox.hpp"
#include "Colour.hpp"
#include "Ray.hpp"
#include "Vertex.hpp"
#include "TypeInfo.hpp"

namespace Pocket {

template<typename T>
class VertexMesh;

class IVertexMesh {
public:
    virtual ~IVertexMesh() { }
    virtual const Vector3& GetPosition(size_t index) = 0;
    virtual size_t Size() = 0;
    virtual void CalcBoundingBox(BoundingBox& box) = 0;
    virtual bool IntersectsRay(const Ray& ray,
                         float* pickDistance, float* barycentricU, float* barycentricV, size_t* triangleIndex, Vector3* normal) = 0;
    virtual void Clear() = 0;
    virtual IVertexMesh* Clone() = 0;
    typedef std::vector<short> Triangles;
    Triangles triangles;
    
    static bool RayIntersectsTriangle(const Ray& ray,
                                 const Vector3& tri0, const Vector3& tri1, const Vector3& tri2,
                                 float* pickDistance, float* barycentricU, float* barycentricV) {
        
        // Find vectors for two edges sharing vert0
        Vector3 edge1 = tri1 - tri0;
        Vector3 edge2 = tri2 - tri0;
        
        // Begin calculating determinant - also used to calculate barycentricU parameter
        Vector3 pvec = ray.direction.Cross(edge2);
        
        // If determinant is near zero, ray lies in plane of triangle
        float det = edge1.Dot(pvec);
        if (det < 0.0001f)
            return false;
        
        // Calculate distance from vert0 to ray origin
        Vector3 tvec = ray.position - tri0;
        
        // Calculate barycentricU parameter and test bounds
        *barycentricU = tvec.Dot(pvec);
        if (*barycentricU < 0.0f || *barycentricU > det)
            return false;
        
        // Prepare to test barycentricV parameter
        Vector3 qvec = tvec.Cross(edge1);
        
        // Calculate barycentricV parameter and test bounds
        *barycentricV = ray.direction.Dot(qvec);
        if (*barycentricV < 0.0f || *barycentricU + *barycentricV > det)
            return false;
        
        // Calculate pickDistance, scale parameters, ray intersects triangle
        *pickDistance = edge2.Dot(qvec);
        float fInvDet = 1.0f / det;
        *pickDistance *= fInvDet;
        *barycentricU *= fInvDet;
        *barycentricV *= fInvDet;
        
        return true;
    }
    
    virtual TypeInfo GetType() = 0;
    
    CONSTRUCTOR_BASE(IVertexMesh)
};

template<class Vertex>
class VertexMesh : public IVertexMesh {
public:
    typedef std::vector<Vertex> Vertices;
    Vertices vertices;
    
    TypeInfo GetType() override {
        TypeInfo info;
        info.name = "VertexMesh";
        info.AddField(vertices, "vertices");
        info.AddField(triangles, "triangles");
        return info;
    }
    
    const Vector3& GetPosition(size_t index) override {
        return vertices[index].Position;
    }
    size_t Size() override { return vertices.size(); }
    
    IVertexMesh* Clone() override {
        VertexMesh<Vertex>* clone = new VertexMesh<Vertex>();
        clone->operator=(*this);
        return clone;
    }
    
    void CalcBoundingBox(BoundingBox& box) override {
        size_t size = vertices.size();
        if (size==0) {
            box.center = Vector3(0,0,0);
            box.extends = Vector3(0,0,0);
            return;
        }

        Vector3 min = vertices[0].Position;
        Vector3 max = min;

        for (size_t i=1; i<size; i++) {
            const Vector3& position = vertices[i].Position;
            if (position.x<min.x) min.x = position.x;
            if (position.y<min.y) min.y = position.y;
            if (position.z<min.z) min.z = position.z;

            if (position.x>max.x) max.x = position.x;
            if (position.y>max.y) max.y = position.y;
            if (position.z>max.z) max.z = position.z;
        }
        box.center = (min + max) * 0.5f;
        box.extends = (max - min);
    }
    
    void AddTexturedQuad(int textureWidth, int textureHeight, const Box& textureCoords, const Matrix3x3& transform )
    {
        Vector2 size = textureCoords.Size() * Vector2(textureWidth, textureHeight) * 0.5f;
        //size *= 0.0025f;
        size.y = -size.y;

        Colour color = Colour::White();
        GLshort index = vertices.size();
        Vector3 normal = Vector3(0,0,1);

        vertices.push_back(Vertex(transform.TransformPoint(-size),Vector2(textureCoords.left,textureCoords.top), normal, color));
        vertices.push_back(Vertex(transform.TransformPoint(Vector2(size.x, -size.y)),Vector2(textureCoords.right,textureCoords.top), normal, color));
        vertices.push_back(Vertex(transform.TransformPoint(size),Vector2(textureCoords.right,textureCoords.bottom), normal,color));
        vertices.push_back(Vertex(transform.TransformPoint(Vector2(-size.x, size.y)),Vector2(textureCoords.left,textureCoords.bottom), normal,color));

        triangles.push_back((GLshort)(index));
        triangles.push_back((GLshort)(index+2));
        triangles.push_back((GLshort)(index+1));
        triangles.push_back((GLshort)(index));
        triangles.push_back((GLshort)(index+3));
        triangles.push_back((GLshort)(index+2));

    }

    void AddTexturedQuad(TextureAtlas* atlas, const std::string& name ) {
        AddTexturedQuad(atlas, atlas->GetTextureCoords(name), Matrix3x3::CreateIdentity());
    }

    void AddTexturedQuad( TextureAtlas* atlas, const std::string& name, float scale ) {
        AddTexturedQuad(atlas, atlas->GetTextureCoords(name), Matrix3x3::CreateScale(scale));
    }

    void AddTexturedQuad( TextureAtlas* atlas, const std::string& name, float scale, Vector2 center, float rotation ) {
        AddTexturedQuad(atlas, atlas->GetTextureCoords(name), Matrix3x3::CreateScale(scale) * Matrix3x3::CreateRotation(rotation) * Matrix3x3::CreateTranslation(center));
    }

     
    void AddQuad( Vector2 center, Vector2 size, Colour color )
    {
        Vector2 sizeHalf = size * 0.5f;

        GLshort index = (GLshort)vertices.size();
        Vector3 normal = Vector3(0,0,1);

        vertices.push_back({center-sizeHalf,Vector2(0,0), color, normal});
        vertices.push_back({center+Vector2(sizeHalf.x, -sizeHalf.y),Vector2(1,0), color, normal});
        vertices.push_back({center+sizeHalf,Vector2(1,1), color, normal});
        vertices.push_back({center+Vector2(-sizeHalf.x, sizeHalf.y),Vector2(0,1), color, normal});

        triangles.push_back((GLshort)(index));
        triangles.push_back((GLshort)(index+1));
        triangles.push_back((GLshort)(index+2));
        triangles.push_back((GLshort)(index));
        triangles.push_back((GLshort)(index+2));
        triangles.push_back((GLshort)(index+3));
    }

    void AddTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Colour color) {
        
        GLshort index = vertices.size();
        Vector3 normal = Vector3(0,0,1);

        vertices.push_back(Vertex(p1,Vector2(0,0), normal, color));
        vertices.push_back(Vertex(p2,Vector2(0,0), normal, color));
        vertices.push_back(Vertex(p3,Vector2(0,0), normal, color));
        
        triangles.push_back((GLshort)(index));
        triangles.push_back((GLshort)(index+1));
        triangles.push_back((GLshort)(index+2));
    }

    void SetColor(const Colour& color) {
        for(int i=0; i<vertices.size(); i++) {
            Vertex& vertex = vertices[i];
            vertex.Color = color;
        }
    }

    void SetAlpha(const float& alpha) {
        for(int i=0; i<vertices.size(); i++) {
            Vertex& vertex = vertices[i];
            vertex.Color.a = alpha;
        }
    }

    void Clear() override {
        vertices.clear();
        triangles.clear();
    }

    void Flip() {
        for (int i=0; i<triangles.size(); i+=3) {
            int temp = triangles[i + 1];
            triangles[i + 1] = triangles[i + 2];
            triangles[i + 2] = temp;
        }
    }

    void AddCube(Vector3 center, Vector3 size) {
        AddCube(center, size, Colour::White());
    }

    void AddCube(Vector3 center, Vector3 size, Colour color) {
        
        Matrix4x4 world = Matrix4x4::CreateTranslation(center) * Matrix4x4::CreateScale(size);
        
        Vector3 corners[8];
        
        corners[0] = world.TransformPosition(Vector3(-1,1,1));
        corners[1] = world.TransformPosition(Vector3(1,1,1));
        corners[2] = world.TransformPosition(Vector3(1,-1,1));
        corners[3] = world.TransformPosition(Vector3(-1,-1,1));
        
        corners[4] = world.TransformPosition(Vector3(-1,1,-1));
        corners[5] = world.TransformPosition(Vector3(1,1,-1));
        corners[6] = world.TransformPosition(Vector3(1,-1,-1));
        corners[7] = world.TransformPosition(Vector3(-1,-1,-1));
        
        Vector2 texCoords[4];
        texCoords[0]=Vector2(0,0);
        texCoords[1]=Vector2(1,0);
        texCoords[2]=Vector2(1,1);
        texCoords[3]=Vector2(0,1);
        
        static const Vector3 normals[6] =
        {
            Vector3(0,0, 1),
            Vector3(0,0,-1),
            Vector3(0, 1,0),
            Vector3(0,-1,0),
            Vector3( 1,0,0),
            Vector3(-1,0,0),
        };
        static const int triangleIndicies[6][4] = {
            { 0,1,2,3 },
            { 5,4,7,6 },
            { 4,5,1,0 },
            { 3,2,6,7 },
            { 1,5,6,2 },
            { 4,0,3,7 }
        };
        
        for (int i=0; i<6; ++i) {
            Vector3 normal = world.TransformVector(normals[i]);
            size_t index = vertices.size();
            vertices.push_back({corners[triangleIndicies[i][0]], texCoords[0], color, normal });
            vertices.push_back({corners[triangleIndicies[i][1]], texCoords[1], color, normal });
            vertices.push_back({corners[triangleIndicies[i][2]], texCoords[2], color, normal });
            vertices.push_back({corners[triangleIndicies[i][3]], texCoords[3], color, normal });
            
            triangles.push_back((GLshort)index);
            triangles.push_back((GLshort)index+2);
            triangles.push_back((GLshort)index+1);
            triangles.push_back((GLshort)index);
            triangles.push_back((GLshort)index+3);
            triangles.push_back((GLshort)index+2);
        }
    }

    void AddPlane(const Pocket::Vector3 &center, const Pocket::Vector2 &size, const Pocket::Box &textureCoords) {
        AddPlane(center, size, textureCoords, Colour::White());
    }

    void AddPlane(const Pocket::Vector3 &center, const Pocket::Vector2 &size, const Pocket::Box &textureCoords, const Pocket::Colour &color) {
        
        Matrix4x4 world = Matrix4x4::CreateTranslation(center) * Matrix4x4::CreateScale(Vector3(size.x * 0.5f, 1.0f, size.y * 0.5f));
        
        Vector3 corners[4];
        
        corners[0] = world.TransformPosition(Vector3(-1,0,1));
        corners[1] = world.TransformPosition(Vector3(1,0,1));
        corners[2] = world.TransformPosition(Vector3(1,0,-1));
        corners[3] = world.TransformPosition(Vector3(-1,0,-1));
        
        
        Vector2 texCoords[4];
        texCoords[0]=Vector2(textureCoords.left,textureCoords.bottom);
        texCoords[1]=Vector2(textureCoords.right,textureCoords.bottom);
        texCoords[2]=Vector2(textureCoords.right,textureCoords.top);
        texCoords[3]=Vector2(textureCoords.left,textureCoords.top);
        
        Vector3 normal = world.TransformVector(Vector3(0,1,0));
        size_t index = vertices.size();
        vertices.push_back({corners[0], texCoords[0], color, normal});
        vertices.push_back({corners[1], texCoords[1], color, normal});
        vertices.push_back({corners[2], texCoords[2], color, normal});
        vertices.push_back({corners[3], texCoords[3], color, normal});
        
        triangles.push_back((GLshort)index);
        triangles.push_back((GLshort)index+1);
        triangles.push_back((GLshort)index+2);
        triangles.push_back((GLshort)index);
        triangles.push_back((GLshort)index+2);
        triangles.push_back((GLshort)index+3);
    }

    void AddTriangle(
        const Vector3& p1, const Vector3& p2, const Vector3& p3,
        const Vector3& n1, const Vector3& n2, const Vector3& n3,
        const Vector2& texCoord1, const Vector2& texCoord2, const Vector2& texCoord3, 
        const Colour& color, const Matrix4x4& world) {
        GLshort index = vertices.size();

        Vector3 nn1 = world.TransformVector(n1); nn1.Normalize();
        Vector3 nn2 = world.TransformVector(n2); nn2.Normalize();
        Vector3 nn3 = world.TransformVector(n3); nn3.Normalize();

        vertices.push_back(Vertex(world.TransformPosition(p1), texCoord1, nn1, color));
        vertices.push_back(Vertex(world.TransformPosition(p2), texCoord2, nn2, color));
        vertices.push_back(Vertex(world.TransformPosition(p3), texCoord3, nn3, color));
        
        triangles.push_back((GLshort)(index));
        triangles.push_back((GLshort)(index+1));
        triangles.push_back((GLshort)(index+2));

    }

    void AddSegmentedQuad(const Pocket::Matrix4x4 &world, const Pocket::Vector2 &size, const Pocket::Vector2 &blTexCoord, const Pocket::Vector2 &trTexCoord, int segments) {
        
        GLshort triIndex = (GLshort)vertices.size();
        
        Vector3 normal = world.TransformVector(Vector3(0,0,-1));
        Colour color = Colour::White();
        
        for (int y = 0; y<=segments; y++) {
            for (int x = 0; x<=segments; x++) {
                float xPos = x / (float)segments;
                float yPos = y / (float)segments;
                
                
                Vector3 position = Vector3(-size.x * 0.5f + size.x * xPos, -size.y * 0.5f + yPos * size.y,0);
                Vector2 texCoord = Vector2(blTexCoord.x + (trTexCoord.x - blTexCoord.x) * xPos,
                                           blTexCoord.y + (trTexCoord.y - blTexCoord.y) * yPos);
                vertices.push_back({world.TransformPosition(position),texCoord, color, normal});
            }
        }
        
        for (int y = 0; y<segments; y++) {
            for (int x = 0; x<segments; x++) {
                
                GLshort index = triIndex + (GLshort)x + y * (segments + 1);
                GLshort up = index + (GLshort)segments + 1;
                
                triangles.push_back(index);
                triangles.push_back(up+1);
                triangles.push_back(up);
                
                triangles.push_back(index);
                triangles.push_back(index+1);
                triangles.push_back(up+1);
            }
        }
    }

    void AddGeoSphere(const Pocket::Vector3 center, float radius, int segments) {
        
        size_t vertIndex = vertices.size();
        
        Matrix4x4 worlds[6];
        
        worlds[0]=Matrix4x4::CreateTransform(Vector3(0,0,radius), 1, Quaternion::IDENTITY);
        worlds[1]=Matrix4x4::CreateTransform(Vector3(0,0,-radius), 1, Quaternion::Create(ToRadians(180), Vector3(0,1,0)));
        worlds[2]=Matrix4x4::CreateTransform(Vector3(0,radius,0), 1, Quaternion::Create(ToRadians(-90), Vector3(1,0,0)));
        worlds[3]=Matrix4x4::CreateTransform(Vector3(0,-radius,0), 1, Quaternion::Create(ToRadians(90), Vector3(1,0,0)));
        worlds[4]=Matrix4x4::CreateTransform(Vector3(radius,0,0), 1, Quaternion::Create(ToRadians(90), Vector3(0,1,0)));
        worlds[5]=Matrix4x4::CreateTransform(Vector3(-radius,0,0), 1, Quaternion::Create(ToRadians(-90), Vector3(0,1,0)));
        
        
        for (int i=0; i<6; i++) {
            AddSegmentedQuad(worlds[i], radius * 2, 0, 1, segments);
        }
        
        for (size_t v=vertIndex; v<vertices.size(); v++) {
            Vertex& vertex = vertices[v];
            vertex.Position.Normalize();
            vertex.Normal = vertex.Position;
            vertex.Position *= radius;
            vertex.Position += center;
        }
    }
        
    void AddSphere(const Pocket::Vector3 center, float radius, int xSegments, int ySegments) {
        
        size_t vertIndex = vertices.size();
        
        float dy = MathHelper::Pi / ySegments;
        float dx = (2.0f * MathHelper::Pi) / xSegments;
        
        for (int y = 0; y<=ySegments; y++) {
            for(int x = 0; x<=xSegments; x++) {
                Quaternion rot;
                rot.FromEuler({-MathHelper::Pi * 0.5f + dy * y, dx * x, 0});
                Matrix4x4 world = Matrix4x4::CreateTransform(0,1.0f,rot);
                
                Vector3 position = world.TransformPositionAffine(Vector3(0,0,radius));
                
                vertices.push_back({center + position, Vector2(x/(float)xSegments, y/(float)ySegments), Colour::White(), position.Normalized()});
            }
        }
        
        for (int y = 0; y<ySegments; y++) {
            for(int x = 0; x<xSegments; x++) {
                int vert = vertIndex + x + y * (xSegments+1);
                triangles.push_back(vert);
                triangles.push_back(vert+xSegments+2);
                triangles.push_back(vert+1);
                
                triangles.push_back(vert);
                triangles.push_back(vert+xSegments+1);
                triangles.push_back(vert+xSegments +2);
            }
        }
    }
    


    bool IntersectsRay(const Ray& ray,
                             float* pickDistance, float* barycentricU, float* barycentricV, size_t* triangleIndex, Vector3* normal) override {
        if (triangles.empty()) return false;
        
        float minDistance = 10000000.0f;
            
        float distance;
        float u,v;
        bool hit = false;
        
        for (size_t i=0; i<triangles.size(); i+=3) {
            
            if (RayIntersectsTriangle(ray,
                                      vertices[triangles[i]].Position, vertices[triangles[i+1]].Position, vertices[triangles[i+2]].Position,
                                      &distance, &u, &v)) {
            
                if (distance<minDistance) {
                    minDistance = distance;
                    *pickDistance = distance;
                    *barycentricU = u;
                    *barycentricV = v;
                    *triangleIndex = i;
                }
                
                hit = true;
            }
        }
        
        return hit;
    }
    
    static VertexMesh<Vertex> empty;
    
    CONSTRUCTOR_DERIVED(VertexMesh, Vertex)
};

CONSTRUCTOR_DERIVED_INITIALIZER(VertexMesh)

template<class Vertex> class VertexMesh<Vertex> VertexMesh<Vertex>::empty;

}
