//
//  AssetManager.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 22/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"
#include "FileSystemWatcher.hpp"

namespace Pocket {
    class AssetManager : public GameConcept {
    public:
        void SetFileWatcher(FileSystemWatcher* watcher);
    protected:
        void Initialize();
    };
}
