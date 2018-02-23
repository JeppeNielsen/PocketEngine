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
    static void InitializeStorage(Pocket::GameStorage& storage);
    static void AddGameSystems(Pocket::GameObject& scene);
    static void AddEditorSystems(Pocket::GameObject& scene);
    static bool IsComponentEditorSpecific(int componentType);
};
