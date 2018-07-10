//
//  File.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 01/12/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//



#include "File.hpp"
#include <stdlib.h>
#include "FileReader.hpp"
#include "FileArchive.hpp"

using namespace Pocket;

File::File() : size(0), data(0), userData(0) {}
File::File(std::string path) { Load(path); }
File::~File() { delete[] data; data = 0; size = 0; }

bool File::Load(std::string filename) {

	if (fileArchive) {
        return fileArchive->TryLoadData(filename, [this] (void* d, size_t s) {
            this->data = (unsigned char*)d;
            this->size = s;
        });
    }

    FILE *f;
    
    f = fopen(FileReader::GetFile(filename).c_str(), "rb");
    if (!f) {
        return false;
    }
    
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    data = (unsigned char*)malloc(size);
    fseek(f, 0, SEEK_SET);
    fread(data, 1, size, f);
    fclose(f);

    return true;
}

std::string File::GetFullPath(std::string filename) {
    return FileReader::GetFile(filename);
}

FileArchive* File::fileArchive = 0;

void File::SetArchive(Pocket::FileArchive &archive) {
    fileArchive = &archive;
}

void File::RemoveArchive() {
    fileArchive = 0;
}
