#pragma once
#include "OpenGL.hpp"
#include <string>

namespace Pocket {
	class Texture
	{
	public:
		Texture();
		~Texture();

        void LoadFromPng(std::string filename);
        
        void LoadFromJpeg(std::string filename);
        
        void CreateFromJpegData(void* data, int size);
        
        void GenerateMipmaps();

        void CreateFromBuffer(unsigned char* buffer, int width, int height, GLenum pixelFormat);
        
		int GetWidth();
		int GetHeight();
        
        
        
		GLuint GetHandle();

		virtual void Free();

	protected:
		GLuint texture;
		int width;
		int height;

	};
}