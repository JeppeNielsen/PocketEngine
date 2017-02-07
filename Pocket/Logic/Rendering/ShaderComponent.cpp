//
//  ShaderComponent.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ShaderComponent.hpp"
#include "File.hpp"
using namespace Pocket;

void ShaderComponent::LoadAsset(const std::string &path) {
    
    File file;
    file.Load(path);
    
    std::string textAsset = file.GetDataAsText();
    
    std::string vertexShader;
    
    int fragmentOffset = 0;
    {
        int startIndex = 0;
        int endIndex = 0;
        if (!TryParseShader(textAsset, 0, startIndex, endIndex)) {
            std::cout << "Could not parse shader file"<< std::endl;
            return;
        }
        vertexShader = textAsset.substr(startIndex+1, (endIndex - startIndex)-2);
        fragmentOffset = endIndex + 1;
    }
    
    std::string fragmentShader;
    {
        int startIndex = 0;
        int endIndex = 0;
        if (!TryParseShader(textAsset, fragmentOffset, startIndex, endIndex)) {
            std::cout << "Could not parse shader file"<< std::endl;
            return;
        }
        fragmentShader = textAsset.substr(startIndex+1, (endIndex - startIndex)-2);
    }

    /*
    std::cout << "Vertex Shader: "<< std::endl;
    std::cout << vertexShader << std::endl;
    std::cout << "Pixel Shader: "<< std::endl;
    std::cout << fragmentShader << std::endl;
    */
    GetShader<Vertex>().Load(vertexShader, fragmentShader);
}

bool ShaderComponent::TryParseShader(const std::string &shader, int startOffset, int &startIndex, int &endIndex) {
    const char startToken = '{';
    const char endToken = '}';

    int depth = 0;
    for (int i=startOffset; i<shader.size(); ++i) {
        if (shader[i]==startToken) {
            ++depth;
            if (depth == 1) {
                startIndex = i;
            }
        } else if (shader[i]==endToken) {
            --depth;
            if (depth == 0) {
                endIndex = i;
                return true;
            }
        }
        //std::cout << shader[i];
    }
    return false;
}
