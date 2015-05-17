#pragma once
#include <string>
#include <map>
#include "Box.hpp"
#include "Vector2.hpp"
#include "Texture.hpp"

namespace Pocket {
	class TextureAtlas : public Texture
	{
	public:
		TextureAtlas();
		~TextureAtlas();

		void Load(std::string pngFile, std::string textureFile);
		
		void Free();

		Box GetTextureCoords(std::string name);

	private:
		typedef std::map<std::string, Box> Rectangles;
		Rectangles rectangles;
		
		Vector2 String2Vector(std::string s); 


	};
}

