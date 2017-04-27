//
//  TranslationUnitTextEditorUpdater.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "CodeFile.hpp"
#include "Worker.hpp"
#include "TextEditor.hpp"
#include <set>

using namespace Pocket;

class TranslationUnitTextEditorUpdater : public GameSystem<CodeFile,TextEditor> {
public:
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void Update(float dt) override;
private:
    void LinesChanged(GameObject* object);
    Worker worker;
    std::set<GameObject*> workingObjects;
};
