//
//  FileSystemListenerSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "FileSystemListenerSystem.hpp"
#include <dirent.h>
#include "GameWorld.hpp"

using namespace Pocket;

void FileSystemListenerSystem::Initialize() {

}

void FileSystemListenerSystem::ObjectAdded(Pocket::GameObject *object) {
    FileSystemListener* listener = object->GetComponent<FileSystemListener>();
    listener->watcher.Start(listener->Path);
    listener->watcher.Changed.Bind(this, &FileSystemListenerSystem::SomethingChanged, object);
    listener->Extension.Changed.Bind(this, &FileSystemListenerSystem::SomethingChanged, object);
    
    SomethingChanged(object);
}

void FileSystemListenerSystem::ObjectRemoved(Pocket::GameObject *object) {
    FileSystemListener* listener = object->GetComponent<FileSystemListener>();
    listener->watcher.Stop();
    listener->watcher.Changed.Unbind(this, &FileSystemListenerSystem::SomethingChanged, object);
    listener->Extension.Changed.Unbind(this, &FileSystemListenerSystem::SomethingChanged, object);
}

void FileSystemListenerSystem::SomethingChanged(Pocket::GameObject *object) {
    for (auto child : object->Hierarchy().Children()) {
        child->Remove();
    }
    FileSystemListener* listener = object->GetComponent<FileSystemListener>();
    FindFilesAtPath(object, listener->Path, listener->Extension);
}

void FileSystemListenerSystem::FindFilesAtPath(GameObject* parent, const std::string &path, const std::string &extension) {
    
    struct dirent *entry;
    DIR *dp;

    dp = opendir(path.c_str());
    if (dp == NULL) {
        return;
    }

    while ((entry = readdir(dp))) {
        //puts(entry->d_name);
        
        std::string filename(entry->d_name);
        if (filename.size()>0 && filename[0]=='.') continue;
        
        if (entry->d_type == DT_DIR ) {
            if (filename!="." && filename!="..") {
                GameObject* go = parent->CreateChild();
                go->AddComponent<FilePath>()->path = path;
                go->GetComponent<FilePath>()->filename = filename;
                go->GetComponent<FilePath>()->isFolder = true;
                
                FindFilesAtPath(go, path +"/"+ filename, extension);
            }
        } else {
            if (filename.find(extension)!=std::string::npos) {
                GameObject* go = parent->CreateChild();
                go->AddComponent<FilePath>()->path = path;
                go->GetComponent<FilePath>()->filename = filename;
                go->GetComponent<FilePath>()->isFolder = false;
            }
        }
    }
    closedir(dp);
}
