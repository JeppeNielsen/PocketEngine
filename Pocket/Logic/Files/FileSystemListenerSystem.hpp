//
//  FileSystemListenerSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "FileSystemListener.hpp"
#include "FilePath.hpp"

namespace Pocket {
    class FileSystemListenerSystem : public GameSystem<FileSystemListener> {
        protected:
            void Initialize();
            void ObjectAdded(GameObject* object);
            void ObjectRemoved(GameObject* object);
            void SomethingChanged(GameObject* object);

        private:
            void FindFilesAtPath(GameObject* parent, const std::string& path, const std::string& extension);
    };
}
