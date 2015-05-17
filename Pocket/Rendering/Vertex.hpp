#pragma once
#include "Vector3.hpp"
#include "Colour.hpp"

namespace Pocket {
	class Vertex {
	public:
		Vertex();
		~Vertex();
		Vertex(const Vector3& position);
		Vertex(const Vector3& position, const Colour& color);
		Vertex(const Vector3& position, const Vector2& textureCoords, const Vector3& normal);
		Vertex(const Vector3& position, const Vector2& textureCoords, const Vector3& normal, const Colour& color);

		Vector3 Position;
		Vector2 TextureCoords;
		Colour Color;
		Vector3 Normal;
	};
}

