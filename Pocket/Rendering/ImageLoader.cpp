//
//  ImageLoader.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 30/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ImageLoader.hpp"
#include "File.hpp"
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include <fstream>

using namespace Pocket;

bool ImageLoader::TryLoadImage(const std::string &path, const std::function<void (unsigned char *, int, int)> &onLoaded) {
    
    File file;
    if (!file.Load(path)) {
        return false;
    }
    
    return TryLoadImageFromData(file.Data(), (int)file.Size(), onLoaded);
}

bool ImageLoader::TryLoadImageFromData(unsigned char *data, int size, const std::function<void (unsigned char *, int, int)> &onLoaded) {

    int width, height, bpp;
    unsigned char* pixels = stbi_load_from_memory(data, size, &width, &height, &bpp, 4 );
    if (pixels) {
        onLoaded(pixels, width, height);
        return  true;
    }
    return false;
}


bool ImageLoader::SaveTga(const std::string &path, unsigned char *pixels, int width, int height) {
 
    std::ofstream o(path.c_str(), std::ios::out | std::ios::binary);

	//Write the header
	o.put(0);
   	o.put(0);
   	o.put(2);                         /* uncompressed RGB */
   	o.put(0); 		o.put(0);
   	o.put(0); 	o.put(0);
   	o.put(0);
   	o.put(0); 	o.put(0);           /* X origin */
   	o.put(0); 	o.put(0);           /* y origin */
   	o.put((width & 0x00FF));
   	o.put((width & 0xFF00) / 256);
   	o.put((height & 0x00FF));
   	o.put((height & 0xFF00) / 256);
   	o.put(32);                        /* 24 bit bitmap */
   	o.put(0);
   	
	//Write the pixel data
    
    for (int y=height-1; y>=0; y--) {
        for (int x=0; x<width; x++) {
    
            int index = (y * height + x)*4;
            o.put(pixels[index]);
            o.put(pixels[index+1]);
            o.put(pixels[index+2]);
            o.put(pixels[index+3]);
        }
    }
    
//	for (int i=0;i<height*width*4;i+=4) {
//		o.put(pixels[i]);
//		o.put(pixels[i+1]);
//		o.put(pixels[i+2]);
//		o.put(pixels[i+3]);
//	}   
	
	//close the file
	o.close();

    return true;
}
