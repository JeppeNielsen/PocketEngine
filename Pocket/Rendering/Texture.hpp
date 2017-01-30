#pragma once
#include <string>

namespace Pocket {
	class Texture {
	public:
        using GLuint = uint32_t;
        using GLenum = uint32_t;
    
		Texture();
		~Texture();

        void LoadFromFile(const std::string& filename);
        void LoadFromMemory(unsigned char* data, int size);
        
        void GenerateMipmaps();

        void CreateFromBuffer(unsigned char* buffer, int width, int height, GLenum pixelFormat);
        
		int GetWidth();
		int GetHeight();
        
		GLuint GetHandle();

		virtual void Free();
        
        void DisableMipmapping();
        
        void SaveToPng(const std::string& path, GLenum pixelFormat);

	protected:
		GLuint texture;
		int width;
		int height;

	};
}
