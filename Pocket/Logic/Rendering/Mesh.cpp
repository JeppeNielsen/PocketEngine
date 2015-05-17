#include "Mesh.hpp"
#include "Quaternion.hpp"
#include "MathHelper.hpp"
#include "Ray.hpp"
#include "TextureAtlas.hpp"
#include <math.h>
#include <iostream>
#include "FileReader.hpp"
#include <fstream>
#include "StringHelper.hpp"
#include <vector>

using namespace Pocket;

Mesh::Mesh()
:
LocalBoundingBox(this)
{
	RevertDefaultCalcBoundingBox();
	vertexBufferDirty = false;
	indexBufferDirty = false;
	vertexBuffer = 0;
    indexBuffer = 0;
    customIntersector = 0;
}

Mesh::~Mesh() {
    if (vertexBuffer) {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = 0;
    }
    
    if (indexBuffer) {
        glDeleteBuffers(1, &indexBuffer);
        indexBuffer = 0;
    }
}

void Mesh::RevertDefaultCalcBoundingBox() {
    LocalBoundingBox.Method.Clear();
    LocalBoundingBox.Method+=event_handler(this, &Mesh::CalcBoundingBox);
}

void Mesh::CalcBoundingBox( DirtyProperty<Mesh*, BoundingBox>::EventData& eventData )
{
	BoundingBox& box = *eventData.Value;

	size_t size = vertices.size();
	if (size==0) {
		box.center = Vector3(0,0,0);
		box.extends = Vector3(0,0,0);
        return;
	}

	Vector3 min = vertices[0].Position;
	Vector3 max = min;

	for (int i=1; i<size; i++) {
		Vertex& vertex = vertices[i];
		if (vertex.Position.x<min.x) min.x = vertex.Position.x;
		if (vertex.Position.y<min.y) min.y = vertex.Position.y;
		if (vertex.Position.z<min.z) min.z = vertex.Position.z;

		if (vertex.Position.x>max.x) max.x = vertex.Position.x;
		if (vertex.Position.y>max.y) max.y = vertex.Position.y;
		if (vertex.Position.z>max.z) max.z = vertex.Position.z;
	}

	box.center = (min + max) * 0.5f;
	box.extends = (max - min);
}

Mesh::VerticesList& Mesh::Vertices() {
	LocalBoundingBox.MakeDirty();
	vertexBufferDirty = true;
	return vertices;
}

Mesh::TrianglesList& Mesh::Triangles() {
	indexBufferDirty = true;
	return triangles;
}

const Mesh::VerticesList& Mesh::ReadOnlyVertices() {
	return vertices;
}

const Mesh::TrianglesList& Mesh::ReadOnlyTriangles() {
	return triangles;
}

void Mesh::AddTexturedQuad( Texture* texture, const Box& textureCoords, const Matrix3x3& transform )
{
	Vector2 size = textureCoords.Size() * Vector2(texture->GetWidth(), texture->GetHeight()) * 0.5f;
	//size *= 0.0025f;
    size.y = -size.y;

	Colour color = Colour::White();
	GLshort index = Vertices().size();
	Vector3 normal = Vector3(0,0,1);

	Vertices().push_back(Vertex(transform.TransformPoint(-size),Vector2(textureCoords.left,textureCoords.top), normal, color));
	Vertices().push_back(Vertex(transform.TransformPoint(Vector2(size.x, -size.y)),Vector2(textureCoords.right,textureCoords.top), normal, color));
	Vertices().push_back(Vertex(transform.TransformPoint(size),Vector2(textureCoords.right,textureCoords.bottom), normal,color));
	Vertices().push_back(Vertex(transform.TransformPoint(Vector2(-size.x, size.y)),Vector2(textureCoords.left,textureCoords.bottom), normal,color));

	Triangles().push_back((GLshort)(index));
	Triangles().push_back((GLshort)(index+2));
	Triangles().push_back((GLshort)(index+1));
	Triangles().push_back((GLshort)(index));
	Triangles().push_back((GLshort)(index+3));
	Triangles().push_back((GLshort)(index+2));

}

void Mesh::AddTexturedQuad(TextureAtlas* atlas, const std::string& name ) {
	AddTexturedQuad(atlas, atlas->GetTextureCoords(name), Matrix3x3::CreateIdentity());
}

void Mesh::AddTexturedQuad( TextureAtlas* atlas, const std::string& name, float scale ) {
	AddTexturedQuad(atlas, atlas->GetTextureCoords(name), Matrix3x3::CreateScale(scale));
}

void Mesh::AddTexturedQuad( TextureAtlas* atlas, const std::string& name, float scale, Vector2 center, float rotation ) {
	AddTexturedQuad(atlas, atlas->GetTextureCoords(name), Matrix3x3::CreateScale(scale) * Matrix3x3::CreateRotation(rotation) * Matrix3x3::CreateTranslation(center));
}

 
void Mesh::AddQuad( Vector2 center, Vector2 size, Colour color )
{
	Vector2 sizeHalf = size * 0.5f;

	GLshort index = Vertices().size();
	Vector3 normal = Vector3(0,0,1);

	Vertices().push_back(Vertex(center-sizeHalf,Vector2(0,0), normal, color));
	Vertices().push_back(Vertex(center+Vector2(sizeHalf.x, -sizeHalf.y),Vector2(1,0), normal, color));
	Vertices().push_back(Vertex(center+sizeHalf,Vector2(1,1), normal,color));
	Vertices().push_back(Vertex(center+Vector2(-sizeHalf.x, sizeHalf.y),Vector2(0,1), normal,color));

	Triangles().push_back((GLshort)(index));
	Triangles().push_back((GLshort)(index+1));
	Triangles().push_back((GLshort)(index+2));
	Triangles().push_back((GLshort)(index));
	Triangles().push_back((GLshort)(index+2));
	Triangles().push_back((GLshort)(index+3));
}

void Mesh::AddTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Colour color) {
    
    GLshort index = Vertices().size();
	Vector3 normal = Vector3(0,0,1);

    Vertices().push_back(Vertex(p1,Vector2(0,0), normal, color));
    Vertices().push_back(Vertex(p2,Vector2(0,0), normal, color));
    Vertices().push_back(Vertex(p3,Vector2(0,0), normal, color));
    
    Triangles().push_back((GLshort)(index));
	Triangles().push_back((GLshort)(index+1));
	Triangles().push_back((GLshort)(index+2));
}

void Mesh::SetColor(const Colour& color) {
	for(int i=0; i<vertices.size(); i++) {
		Vertex& vertex = vertices[i];
		vertex.Color = color;
	}
}

void Mesh::SetAlpha(const float& alpha) {
	for(int i=0; i<vertices.size(); i++) {
		Vertex& vertex = vertices[i];
		vertex.Color.a = alpha;
	}
}

void Mesh::Clear() {
    Vertices().clear();
    Triangles().clear();
}

void Mesh::Flip() {
    TrianglesList& triangles = Triangles();
    for (int i=0; i<triangles.size(); i+=3) {
        int temp = triangles[i + 1];
        triangles[i + 1] = triangles[i + 2];
        triangles[i + 2] = temp;
    }
}

void Mesh::AddCube(Vector3 center, Vector3 size) {
    AddCube(center, size, Colour::White());
}

void Mesh::AddCube(Vector3 center, Vector3 size, Colour color) {
    
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
    
    Vector3 normals[6] =
    {
        Vector3(0,0, 1),
        Vector3(0,0,-1),
        Vector3(0, 1,0),
        Vector3(0,-1,0),
        Vector3( 1,0,0),
        Vector3(-1,0,0),
    };
    int triangleIndicies[6][4] = {
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
        vertices.push_back(Vertex(corners[triangleIndicies[i][0]], texCoords[0], normal, color));
        vertices.push_back(Vertex(corners[triangleIndicies[i][1]], texCoords[1], normal, color));
        vertices.push_back(Vertex(corners[triangleIndicies[i][2]], texCoords[2], normal, color));
        vertices.push_back(Vertex(corners[triangleIndicies[i][3]], texCoords[3], normal, color));
        
        triangles.push_back((GLshort)index);
        triangles.push_back((GLshort)index+2);
        triangles.push_back((GLshort)index+1);
        triangles.push_back((GLshort)index);
        triangles.push_back((GLshort)index+3);
        triangles.push_back((GLshort)index+2);
    }
}

void Mesh::AddPlane(const Pocket::Vector3 &center, const Pocket::Vector2 &size, const Pocket::Box &textureCoords) {
    AddPlane(center, size, textureCoords, Colour::White());
}

void Mesh::AddPlane(const Pocket::Vector3 &center, const Pocket::Vector2 &size, const Pocket::Box &textureCoords, const Pocket::Colour &color) {
    
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
    vertices.push_back(Vertex(corners[0], texCoords[0], normal, color));
    vertices.push_back(Vertex(corners[1], texCoords[1], normal, color));
    vertices.push_back(Vertex(corners[2], texCoords[2], normal, color));
    vertices.push_back(Vertex(corners[3], texCoords[3], normal, color));
    
    triangles.push_back((GLshort)index);
    triangles.push_back((GLshort)index+1);
    triangles.push_back((GLshort)index+2);
    triangles.push_back((GLshort)index);
    triangles.push_back((GLshort)index+2);
    triangles.push_back((GLshort)index+3);
}

void Mesh::AddTriangle(
	const Vector3& p1, const Vector3& p2, const Vector3& p3,
	const Vector3& n1, const Vector3& n2, const Vector3& n3,
	const Vector2& texCoord1, const Vector2& texCoord2, const Vector2& texCoord3, 
	const Colour& color, const Matrix4x4& world) {
	GLshort index = Vertices().size();

	Vector3 nn1 = world.TransformVector(n1); nn1.Normalize();
	Vector3 nn2 = world.TransformVector(n2); nn2.Normalize();
	Vector3 nn3 = world.TransformVector(n3); nn3.Normalize();

    Vertices().push_back(Vertex(world.TransformPosition(p1), texCoord1, nn1, color));
    Vertices().push_back(Vertex(world.TransformPosition(p2), texCoord2, nn2, color));
    Vertices().push_back(Vertex(world.TransformPosition(p3), texCoord3, nn3, color));
    
    Triangles().push_back((GLshort)(index));
	Triangles().push_back((GLshort)(index+1));
	Triangles().push_back((GLshort)(index+2));

}

void Mesh::AddSegmentedQuad(const Pocket::Matrix4x4 &world, const Pocket::Vector2 &size, const Pocket::Vector2 &blTexCoord, const Pocket::Vector2 &trTexCoord, int segments) {
    
    GLshort triIndex = (GLshort)Vertices().size();
    
    Vector3 normal = world.TransformVector(Vector3(0,0,-1));
    Colour color = Colour::White();
    
    for (int y = 0; y<=segments; y++) {
        for (int x = 0; x<=segments; x++) {
            float xPos = x / (float)segments;
            float yPos = y / (float)segments;
            
            
            Vector3 position = Vector3(-size.x * 0.5f + size.x * xPos, -size.y * 0.5f + yPos * size.y,0);
            Vector2 texCoord = Vector2(blTexCoord.x + (trTexCoord.x - blTexCoord.x) * xPos,
                                       blTexCoord.y + (trTexCoord.y - blTexCoord.y) * yPos);
            Vertices().push_back(Vertex(world.TransformPosition(position),texCoord, normal, color));
        }
    }
    
    for (int y = 0; y<segments; y++) {
        for (int x = 0; x<segments; x++) {
            
            GLshort index = triIndex + (GLshort)x + y * (segments + 1);
            GLshort up = index + (GLshort)segments + 1;
            
            Triangles().push_back(index);
            Triangles().push_back(up+1);
            Triangles().push_back(up);
            
            Triangles().push_back(index);
            Triangles().push_back(index+1);
            Triangles().push_back(up+1);
        }
    }
}

void Mesh::AddGeoSphere(const Pocket::Vector3 center, float radius, int segments) {
    
    size_t vertIndex = Vertices().size();
    
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
    
    
    for (size_t v=vertIndex; v<Vertices().size(); v++) {
        Vertex& vertex = Vertices()[v];
        vertex.Position.Normalize();
        vertex.Normal = vertex.Position;
        vertex.Position *= radius;
        vertex.Position += center;
    }
}

void Mesh::Bind() {
    
    if (!vertexBuffer) {
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &indexBuffer);
    }

	if (vertexBufferDirty) {
		vertexBufferDirty = false;

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * VerticesSize(), &vertices[0], GL_STATIC_DRAW);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	}

	if (indexBufferDirty) {
		indexBufferDirty = false;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * TrianglesSize(), &triangles[0], GL_STATIC_DRAW);
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	}
}

void Mesh::Draw() {
	glDrawElements(GL_TRIANGLES, (int)TrianglesSize(), GL_UNSIGNED_SHORT, 0);
}

bool Mesh::RayIntersectsTriangle(const Ray& ray,
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


bool Mesh::IntersectsRay(const Ray& ray,
                         float* pickDistance, float* barycentricU, float* barycentricV, size_t* triangleIndex, Vector3* normal) {
    
    if (triangles.empty()) return false;
    
    if (customIntersector) {
        return customIntersector->IntersectsRay(*this, ray, pickDistance, barycentricU, barycentricV, triangleIndex, normal);
    }
    
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
void Mesh::Reset() {
    
    Clear();
    
    vertexBufferDirty = true;
    indexBufferDirty = true;
    customVerticesSize = 0;
    customTrianglesSize = 0;
    
    if (vertexBuffer) {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = 0;
    }
    
    if (indexBuffer) {
        glDeleteBuffers(1, &indexBuffer);
        indexBuffer = 0;
    }
}

void Mesh::Clone(const Mesh& source) {
    Reset();
    vertices = source.vertices;
    triangles = source.triangles;
    customVerticesSize = source.customVerticesSize;
    customTrianglesSize = source.customTrianglesSize;
}

void Mesh::SetBufferSizes(size_t verticesSize, size_t trianglesSize) {
    if (customVerticesSize==verticesSize && customTrianglesSize == trianglesSize) return;
    customVerticesSize = verticesSize;
    customTrianglesSize = trianglesSize;
    vertexBufferDirty = true;
    indexBufferDirty = true;
}

size_t Mesh::VerticesSize() const {
    return customVerticesSize == 0 || customVerticesSize>vertices.size() ? vertices.size() : customVerticesSize;
}

size_t Mesh::TrianglesSize() const {
    return customTrianglesSize == 0 || customVerticesSize>triangles.size() ? triangles.size() : customTrianglesSize;
}

bool Mesh::AddObjFile(std::string objFile) {
    
    std::string fileName = FileReader::GetFile(objFile);
    
    std::ifstream file;
    std::string line;
    
    file.open(fileName.c_str());
    
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> textureCoords;
    
    typedef std::map<size_t, size_t> CreatedVerts;
    CreatedVerts createdVerts;
    
    if (!file.is_open()) return false;
    
    Vertices();
    
    while(getline(file, line) )
    {
        std::string header = line.substr(0,2);
        
        if (header == "") {
            continue;
        } else if (header == "v ") {
            positions.push_back(Vector3::Deserialize(line.substr(2), ' '));
        } else if (header == "vn") {
            normals.push_back(Vector3::Deserialize(line.substr(3), ' '));
        } else if (header == "vt") {
            textureCoords.push_back(Vector2::Deserialize(line.substr(3), ' '));
        } else if (header == "f ") {
            
            std::vector<std::string> s;
            StringHelper::split(line.substr(2), ' ', s);
            if (s.size()<3) continue;
            
            std::vector<int> v;
            for (int i=0; i<3; i++) {
                StringHelper::splitToInts(s[i], '/', v);
                if (v.size()<3) continue;
                
                size_t hash = v[0] + v[1] * positions.size() + v[2] * (positions.size() + textureCoords.size());
                
                CreatedVerts::iterator it = createdVerts.find(hash);
                size_t index;
                if (it==createdVerts.end()) {
                    index = vertices.size();
                    
                    vertices.push_back(Vertex(
                        v[0]>0 ? positions[v[0]-1] : Vector3(0,0,0),
                        v[1]>0 ? textureCoords[v[1]-1] : Vector2(0,0),
                        v[2]>0 ? normals[v[2]-1] : Vector3(0,0,0)
                    ));
                    createdVerts[hash] = index;
                } else {
                    index = it->second;
                }
                
                triangles.push_back((GLshort)(index));
                v.clear();
            }
        }
    }
    
    file.close();
    return true;
}

void Mesh::Center() {
    Vector3 center = LocalBoundingBox.GetValue()->center;
    Vertices();
    for (size_t i = 0; i<vertices.size(); ++i) {
        vertices[i].Position -= center;
    }
}


