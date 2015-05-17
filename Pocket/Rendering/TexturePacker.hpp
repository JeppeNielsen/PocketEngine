#pragma once
#include "RectPacker.hpp"
#include <vector>
#include <string>

namespace Pocket {
	class TexturePacker
	{
	public:
		TexturePacker(void);
		~TexturePacker(void);

		bool CreateAtlas(std::string dir, std::string pngFile, std::string xmlFile, int maxWidth, int maxHeight);

	private:

		struct Image {
			int x;
			int y;
			int width;
			int height;
			bool willFit;
			std::string path;
			std::vector<unsigned char> image;
			void Load(std::string filename);
		};

		int getdir(std::string dir, std::vector<std::string>& files);

		void WritePixels(std::vector<unsigned char>& buffer, int buffer_w, int buffer_h, std::vector<unsigned char>& pixels, int x, int y, int w, int h);

		int ConvertIndex(int buf_w, int buf_h, int x, int y);
		
		void WriteXml(std::string filename, std::vector<Image*>& images);
	
		

		struct TSubRect: public RectPacker::TRect
		{
		  int n;      // Original index of this subrect, before sorting

		  TSubRect() { }
		  TSubRect(int _w, int _h, int _n): TRect(0, 0, _w, _h), n(_n) { }
		};

	};
}
