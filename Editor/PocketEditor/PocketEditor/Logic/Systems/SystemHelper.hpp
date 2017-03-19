//
//  SystemHelper.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 19/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"

struct SystemHelper {
    static void AddGameSystems(Pocket::GameObject& root);
    static void AddEditorSystems(Pocket::GameObject& root);
    static bool IsComponentEditorSpecific(int componentType);
};
