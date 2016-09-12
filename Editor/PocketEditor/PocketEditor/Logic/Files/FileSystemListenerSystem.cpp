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

void FileSystemListenerSystem::ObjectAdded(Pocket::GameObject *object) {
    auto& watcher = object->GetComponent<FileSystemListener>()->watcher;
    watcher.Start(object->GetComponent<FileSystemListener>()->path);
    watcher.Changed.Bind(this, &FileSystemListenerSystem::SomethingChanged, object);
    SomethingChanged(object);
}

void FileSystemListenerSystem::ObjectRemoved(Pocket::GameObject *object) {
    auto& watcher = object->GetComponent<FileSystemListener>()->watcher;
    watcher.Stop();
    watcher.Changed.Unbind(this, &FileSystemListenerSystem::SomethingChanged, object);
}

void FileSystemListenerSystem::SomethingChanged(Pocket::GameObject *object) {
    for (auto child : object->Children()) {
        child->Remove();
    }
    FindFilesAtPath(object, object->GetComponent<FileSystemListener>()->path, "cpp");
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
        
        if (entry->d_type == DT_DIR ) {
            if (filename!="." && filename!="..") {
                GameObject* go = world->CreateObject();
                go->Parent() = parent;
                go->AddComponent<FilePath>()->path = path +"/"+ filename;
                go->GetComponent<FilePath>()->filename = filename;
                FindFilesAtPath(go, path +"/"+ filename, extension);
            }
        } else {
            if (filename.find(extension)!=std::string::npos) {
                GameObject* go = world->CreateObject();
                go->Parent() = parent;
                go->AddComponent<FilePath>()->path = path+ "/"+ filename;
                go->GetComponent<FilePath>()->filename = filename;
            }
        }
    }
    closedir(dp);
}
