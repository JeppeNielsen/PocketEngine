//
//  TranslationUnitUpdater.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 23/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TranslationUnitUpdater.hpp"


void TranslationUnitUpdater::Initialize() {
    watcher = 0;
}

void TranslationUnitUpdater::Destroy() {
    if (watcher) {
        watcher->FileModified.Unbind(this, &TranslationUnitUpdater::FileChanged);
    }
}

void TranslationUnitUpdater::SetFileWatcher(Pocket::FileSystemWatcher *watcher) {
    this->watcher = watcher;
    watcher->FileModified.Bind(this, &TranslationUnitUpdater::FileChanged);
}

void TranslationUnitUpdater::CodeFileChanged(CodeFile *codeFile) {
    worker.DoTask([codeFile] () {
        while (codeFile->IsTranslationUnitInUse) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
        }
        codeFile->IsTranslationUnitInUse = true;
        return codeFile->ParseTranslationUnit();
    }, [codeFile] (CXTranslationUnit unit) {
        if (codeFile->translationUnit) {
            clang_disposeTranslationUnit(codeFile->translationUnit);
        }
        codeFile->translationUnit = unit;
        codeFile->TranslationUnitUpdated();
        codeFile->IsTranslationUnitInUse = false;
    });
}

void TranslationUnitUpdater::FileChanged(std::string filename) {
    for(auto o : Objects()) {
        CodeFile* codeFile = o->GetComponent<CodeFile>();
        if (codeFile->path == filename) {
           CodeFileChanged(codeFile);
        }
    }
}

void TranslationUnitUpdater::ObjectAdded(Pocket::GameObject *object) {
    CodeFileChanged(object->GetComponent<CodeFile>());
}

void TranslationUnitUpdater::Update(float dt) {
    worker.Update();
}
