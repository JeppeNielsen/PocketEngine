//
//  LogSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 26/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameSystem.hpp"
#include "LogMessage.hpp"

class LogSystem : public Pocket::GameSystem<LogMessage> {
protected:
    void Initialize() override;
private:
    Pocket::GameObject* logRoot;
public:
    Pocket::GameObject* Log(const std::string& message);
    Pocket::GameObject* LogRoot();
    static void CreateSubSystems(Pocket::GameStorage& storage);
};
