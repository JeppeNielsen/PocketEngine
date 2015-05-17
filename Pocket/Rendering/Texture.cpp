#include "Texture.hpp"
#include "../PngLib/lodepng.h"
#include "FileReader.hpp"
#include "jpeg_decoder.h"
#include <iostream>

using namespace Pocket;

Texture::Texture() {
	width = 0;
	height = 0;
	texture = 0;
}

Texture::~Texture() {
	Free();
}

int Texture::GetWidth() {
	return width;
}

int Texture::GetHeight() {
	return height;
}

GLuint Texture::GetHandle() {
	return texture;
}



void Texture::LoadFromPng(std::string filename) {
	std::vector<unsigned char> buffer;
	LodePNG::loadFile(buffer, FileReader::GetFile(filename));
	if (buffer.empty()) return;
	LodePNG::Decoder decoder;
	std::vector<unsigned char> image;
	decoder.decode(image, buffer);
	CreateFromBuffer(&image[0], (int)decoder.getWidth(), (int)decoder.getHeight(), GL_RGBA);
}


void Texture::LoadFromJpeg(std::string filename) {
    
    size_t size;
    char *buf;
    FILE *f;
    
    
    f = fopen(FileReader::GetFile(filename).c_str(), "rb");
    if (!f) {
        printf("Error opening the input file.\n");
        return;
    }
    
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    buf = (char*)malloc(size);
    fseek(f, 0, SEEK_SET);
    fread(buf, 1, size, f);
    fclose(f);
    
    /*
    fseek (f, 0, SEEK_END);
    size=ftell (f);
    fseek (f, 0, SEEK_SET);
    char* data = new char[size];
    fread(data,size,1,f);
    delete data;
    fclose(f);
     */
    
    jpgd::jpeg_decoder_mem_stream memoryStream;
    memoryStream.open((unsigned char*)buf, (unsigned int)size);
    
    int actualComps;
    
    unsigned char* pixels = jpgd::decompress_jpeg_image_from_stream(&memoryStream, &width, &height, &actualComps, 3);
    
    std::cout<< " trying to load jpeg image w: " << width << " h: " << height << std::endl;
    std::cout<< " pixels: " << pixels << std::endl;
    
    CreateFromBuffer(pixels, width, height, GL_RGB);
}

void Texture::CreateFromJpegData(void *data, int size) {
    
    //unsigned char* temp = new unsigned char[size];
    //memcpy(temp, data, size);
    
    jpgd::jpeg_decoder_mem_stream memoryStream;
    memoryStream.open((unsigned char*)data, (unsigned int)size);
    
    int actualComps;
    
    unsigned char* pixels = jpgd::decompress_jpeg_image_from_stream(&memoryStream, &width, &height, &actualComps, 3);
    
    std::cout<< " trying to load jpeg image w: " << width << " h: " << height << std::endl;
    //std::cout<< " pixels: " << pixels << std::endl;
    
    CreateFromBuffer(pixels, width, height, GL_RGB);
}


void Texture::CreateFromBuffer(unsigned char *buffer, int width, int height, GLenum pixelFormat) {
    
    this->width = width;
    this->height = height;
    
    glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_POINTS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, buffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    GenerateMipmaps();
}


void Texture::GenerateMipmaps() {
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Free() {
	if (texture) {
		glDeleteTextures(0, &texture);
		texture = 0;
	}
}

