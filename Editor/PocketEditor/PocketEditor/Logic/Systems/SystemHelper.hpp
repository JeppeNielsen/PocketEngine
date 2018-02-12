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
    static void AddGameSystems(Pocket::GameStorage& storage);
    static void AddEditorSystems(Pocket::GameStorage& storage);
    static bool IsComponentEditorSpecific(int componentType);
};
