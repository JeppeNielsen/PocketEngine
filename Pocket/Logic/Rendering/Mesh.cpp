//
//  Mesh.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 21/04/2018.
//  Copyright © 2018 Jeppe Nielsen. All rights reserved.
//

#include "Mesh.hpp"
#include "File.hpp"
#include "FileHelper.hpp"
using namespace Pocket;

void Mesh::LoadAsset(const std::string& path) {
    std::cout << "Trying to load mesh at path: " << path << std::endl;
    File file;
    if (!file.Load(path)) {
        return;
    }
    
    auto& mesh = GetMesh<Vertex>();
    mesh.Clear();
    
    int vertexIndex = 0;
    int normalIndex = 0;
    FileHelper::ParseBuffer(file.Data(), file.Size(), [&, this] (const std::string& line) {
        auto& mesh = GetMesh<Vertex>();
        if (line.substr(0,2) == "v ") {
            if (vertexIndex>=mesh.vertices.size()) {
                mesh.vertices.resize(vertexIndex + 1);
            }
            mesh.vertices[vertexIndex].Position = Vector3::Deserialize(line.substr(2,line.size() - 2), ' ');
            vertexIndex++;
        } else if (line.substr(0,3) == "vn ") {
            if (normalIndex>=mesh.vertices.size()) {
                mesh.vertices.resize(normalIndex + 1);
            }
            mesh.vertices[normalIndex].Normal = Vector3::Deserialize(line.substr(3,line.size() - 3), ' ');
            normalIndex++;
        } else if (line.substr(0,2) == "f ") {
        
            std::string substr = line.substr(2, line.size() - 2);
            
            std::vector<std::string> elems;
            StringHelper::split(substr, ' ', elems);
            
            for(auto& e : elems) {
                std::vector<int> ints;
                StringHelper::splitToInts(e, '/', ints);
                mesh.triangles.push_back((short)ints[0] - 1);
            }
            
        }
    });
}
