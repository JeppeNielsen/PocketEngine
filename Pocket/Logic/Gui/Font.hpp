#pragma once
#include <string>
#include <vector>
#include "Vector2.hpp"


namespace Pocket {
	struct Font {
		Font();
		~Font();
        
        void Reset();
        void Clone(const Font& source);
        void Save();
        void Load();
		
		static const int MaxCharacters = 256;

		//<char id="32" x="996" y="64" width="17" height="17" xoffset="-8" yoffset="39" xadvance="16" page="0" chnl="15" />

        void Load(std::string filename, std::string atlasNode="");

		struct Character {
			Character();
			float textureX;
			float textureY;
			float textureWidth;
			float textureHeight;
			float width;
			float height;
			float xoffset;
			float yoffset;
			float xadvance;
		};

		Character characters[MaxCharacters];
    
        std::string FontAtlasNode;
    
        float lineHeight;
		float baseSize;

		struct Letter {
			float x;
			float y;
			float width;
			float height;

			float u1;
			float v1;
			float u2;
			float v2;
		};

		enum HAlignment { Left, Center, Right };
		enum VAlignment { Top, Middle, Bottom };

		void CreateText(std::vector<Letter>& sentence, std::string text, Vector2 size, float fontSize, HAlignment hAlign, VAlignment vAlign, bool wordWrap, bool flipY) const;

        

	private:
		void split(const std::string &s, char delim, std::vector<std::string> &elems) const;
	};
}

