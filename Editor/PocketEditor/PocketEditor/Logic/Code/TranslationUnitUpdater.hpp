//
//  TranslationUnitUpdater.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 23/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "CodeFile.hpp"
#include "FileSystemWatcher.hpp"
#include "Worker.hpp"

using namespace Pocket;

class TranslationUnitUpdater : public GameSystem<CodeFile> {
public:
    void SetFileWatcher(FileSystemWatcher* watcher);
    
    void Initialize() override;
    void Destroy() override;
    void ObjectAdded(GameObject* object) override;
    void Update(float dt) override;
    
private:
    void FileChanged(std::string filename);
    void CodeFileChanged(CodeFile* codeFile);
    
    FileSystemWatcher* watcher;
    Worker worker;
};
