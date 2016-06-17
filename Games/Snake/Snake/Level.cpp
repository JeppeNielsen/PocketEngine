//
//  Level.cpp
//  Snake
//
//  Created by Jeppe Nielsen on 04/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Level.hpp"
#include "File.hpp"
#include "../PngLib/lodepng.h"
#include <iostream>

void Level::LoadFromPng(std::string pngFile) {

    File file;
    if (!file.Load(pngFile)) {
        return;
    }
    LodePNG::Decoder decoder;
	std::vector<unsigned char> image;
	decoder.decode(image, (const unsigned char*)file.Data(), file.Size());


    for(int i=0; i<image.size();i+=4) {
        int pixelIndex = i/4;
    
        unsigned char r = image[i];
        unsigned char g = image[i+1];
        unsigned char b = image[i+2];
        unsigned char a = image[i+3];
        
        /*
        "Green",
        "Red",
        "Blue",
        "Yellow"
        */

        
        if (r==255 && g == 0 && b == 255) continue; //magenta is nothing
        
        int x = pixelIndex % decoder.getWidth();
        int y = pixelIndex / decoder.getWidth();
        
        Point p = {x,y};
        allPoints.insert(p);
        
        
        if (r==255 && g == 255 && b == 255) {
            blockable.insert(p);
        } else if (r == 0 && g == 0 && b == 0) {
            spawnable.insert(p);
        } else if (r==0 && g == 255 && b == 0) {
            colorSpawnPoints[0].insert(p);
        }else if (r==255 && g == 0 && b == 0) {
            colorSpawnPoints[1].insert(p);
        }else if (r==0 && g == 0 && b == 255) {
            colorSpawnPoints[2].insert(p);
        }else if (r==255 && g == 255 && b == 0) {
            colorSpawnPoints[3].insert(p);
        }
    }
    
    if (allPoints.empty()) return;
    
    Point min = *allPoints.begin();
    Point max = min;
    
    for(auto p : allPoints) {
        if (p.x>max.x) max.x = p.x;
        if (p.x<min.x) min.x = p.x;
        
        if (p.y>max.y) max.y = p.y;
        if (p.y<min.y) min.y = p.y;
    }
    
    Point mid = (max - min)/2;
    Point size = (max - min)/2 ;
    
    ModifyList(blockable, -mid - size);
    ModifyList(spawnable, -mid-size);
}

void Level::ModifyList(std::set<Point> &list, Pocket::Point amount) {
    std::set<Point> temp;
    for(auto& p : list) {
        temp.insert(p + amount);
    }
    list.clear();
    for(auto p : temp) {
        list.insert(p);
    }
}
