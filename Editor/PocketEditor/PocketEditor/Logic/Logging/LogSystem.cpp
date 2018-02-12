//
//  LogSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 26/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "LogSystem.hpp"

using namespace Pocket;

void LogSystem::Initialize() {
    logRoot = root->CreateChild();
}

GameObject* LogSystem::Log(const std::string &message) {
    auto logObject = logRoot->CreateChild();
    logObject->AddComponent<LogMessage>()->message = message;
    return logObject;
}

GameObject* LogSystem::LogRoot() {
    return logRoot;
}

void LogSystem::CreateSubSystems(GameStorage &storage) {
    storage.AddComponentType<LogMessage>();
}
