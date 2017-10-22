#include "Texture.hpp"
#include "OpenGL.hpp"
#include "ImageLoader.hpp"
#include "RenderTexture.hpp"

using namespace Pocket;

Texture::Texture() :
    width(0), height(0), texture(0), renderTexture(0) {
}

Texture::~Texture() {
	Free();
}

int Texture::GetWidth() {
	return renderTexture ? renderTextureWidth : width;
}

int Texture::GetHeight() {
	return renderTexture ? renderTextureHeight : height;
}

GLuint Texture::GetHandle() {
	return renderTexture ? renderTexture->GetTexture() : texture;
}

void Texture::LoadFromMemory(unsigned char *data, int size) {
    ImageLoader::TryLoadImageFromData(data, size, [this] (unsigned char* pixels, int width, int height) {
        CreateFromBuffer(pixels, width, height, GL_RGBA);
    });
}

void Texture::LoadFromFile(const std::string& filename) {
    ImageLoader::TryLoadImage(filename, [this] (unsigned char* pixels, int width, int height) {
        CreateFromBuffer(pixels, width, height, GL_RGBA);
    });
}

void Texture::CreateFromBuffer(unsigned char *buffer, int width, int height, GLenum pixelFormat) {
    
    Free();
    
    this->width = width;
    this->height = height;
    
    ASSERT_GL(glGenTextures(1, &texture));
	ASSERT_GL(glBindTexture(GL_TEXTURE_2D, texture));
    
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	
	ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    
    
    //ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    //ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_POINTS));
    //ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    //ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    
    ASSERT_GL(glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, buffer));
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    //GenerateMipmaps();
}

void Texture::DisableMipmapping() {
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, texture));
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
}

void Texture::GenerateMipmaps() {
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, texture));
    ASSERT_GL(glGenerateMipmap(GL_TEXTURE_2D));
    ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	ASSERT_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
}

void Texture::Free() {
	if (texture) {
		ASSERT_GL(glDeleteTextures(0, &texture));
		texture = 0;
	}
}

void Texture::SaveToTga(const std::string &path, GLenum pixelFormat) {
#ifdef EMSCRIPTEN
#elif IPHONE
#else
    ASSERT_GL(glBindTexture(GL_TEXTURE_2D, texture));
    unsigned char* pixels = new unsigned char[width * height * 4];
    ASSERT_GL(glGetTexImage(GL_TEXTURE_2D, 0, pixelFormat, GL_UNSIGNED_BYTE, pixels));
    ImageLoader::SaveTga(path, pixels, width, height);
#endif
}

void Texture::SetRenderTexture(Pocket::RenderTexture *renderTexture, int width, int height) {
    this->renderTexture = renderTexture;
    renderTextureWidth = width;
    renderTextureHeight = height;
}

