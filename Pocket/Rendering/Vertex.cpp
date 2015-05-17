#include "Vertex.hpp"

using namespace Pocket;

Vertex::Vertex(){
}

Vertex::~Vertex(){
}

Vertex::Vertex(const Vector3& position ) {
	Position = position;
	TextureCoords = Vector2(0,0);
	Color =  Colour(1.0f,1.0f,1.0f,1.0f);
}


Vertex::Vertex( const Vector3& position, const Colour& color ) {
	Position = position;
	TextureCoords = Vector2(0,0);
	Normal = Vector3(0,0,1);
	Color = color;
}

Vertex::Vertex( const Vector3& position, const Vector2& textureCoords, const Vector3& normal ) {
	Position = position;
	TextureCoords = textureCoords;
	Normal = normal;
	Color = Colour(1.0f,1.0f,1.0f,1.0f);
}

Vertex::Vertex(const Vector3& position, const Vector2& textureCoords, const Vector3& normal, const Colour& color) {
	Position = position;
	TextureCoords = textureCoords;
	Normal = normal;
	Color = color;
}
