//
//  testIconSaving.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 14/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include <vector>
#include <stdio.h>
#include "ImageLoader.hpp"
#include "ImageHelper.hpp"
#include <string>
#include "FileReader.hpp"

using namespace Pocket;

struct ImageSizeFormat {
    std::string name;
    int size;
};

int main_iconSaving() {

    unsigned char* pixels;
    int width;
    int height;
    
    ImageLoader::TryLoadImage("/Projects/PocketEngine/EditorProjects/Pong/Assets/Ball.psd", [&] (unsigned char* p, int w, int h) {
        pixels = p;
        width = w;
        height = h;
    });
    
//    sips -z 16 16     Icon1024.png --out MyIcon.iconset/icon_16x16.png
//sips -z 32 32     Icon1024.png --out MyIcon.iconset/icon_16x16@2x.png
//sips -z 32 32     Icon1024.png --out MyIcon.iconset/icon_32x32.png
//sips -z 64 64     Icon1024.png --out MyIcon.iconset/icon_32x32@2x.png
//sips -z 128 128   Icon1024.png --out MyIcon.iconset/icon_128x128.png
//sips -z 256 256   Icon1024.png --out MyIcon.iconset/icon_128x128@2x.png
//sips -z 256 256   Icon1024.png --out MyIcon.iconset/icon_256x256.png
//sips -z 512 512   Icon1024.png --out MyIcon.iconset/icon_256x256@2x.png
//sips -z 512 512   Icon1024.png --out MyIcon.iconset/icon_512x512.png

    std::vector<ImageSizeFormat> imageFormats = {
        {"icon_16x16.png", 16},
        {"icon_16x16@2x.png", 32},
        {"icon_32x32.png", 32},
        {"icon_32x32@2x.png", 64},
        {"icon_128x128.png", 128},
        {"icon_128x128@2x.png", 256},
        {"icon_256x256.png", 256},
        {"icon_256x256@2x.png", 512},
        {"icon_512x512.png", 512},
        {"icon_512x512@2x.png", 1024},
    };
    
    
    for (auto i : imageFormats) {
    
    unsigned char* scaledPixels = ImageHelper::ScaleImage(pixels, width, height, i.size, i.size);

    ImageHelper::SavePng("/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/Assets/Icon.iconset/"+i.name, scaledPixels, i.size, i.size);
    
    delete [] scaledPixels;
    
    }

    FileReader::RunCommmand("iconutil -c icns /Projects/PocketEngine/Editor/PocketEditor/PocketEditor/Assets/Icon.iconset");


//
//
//
//    icns_family_t    *iconFamily;
//    icns_set_print_errors(1);
//    icns_create_family(&iconFamily);
//
//    add_png_to_family(&iconFamily, "");
//
//    FILE *icnsfile;
//
//    const char* outputFile = "/Projects/PocketEngine/Editor/PocketEditor/PocketEditor/Assets/OutputIcon.icns";
//
//    icnsfile = fopen (outputFile, "wb+");
//    if (icnsfile == NULL)
//    {
//        //fprintf (stderr, "Could not open '%s' for writing: %s\n", outputFile, strerror(errno));
//
//        exit(1);
//    }
//
//    if (icns_write_family_to_file(icnsfile, iconFamily) != ICNS_STATUS_OK)
//    {
//        fprintf(stderr, "Failed to write icns file\n");
//        fclose(icnsfile);
//
//        exit(1);
//    }
//
    












    return 0;
}
