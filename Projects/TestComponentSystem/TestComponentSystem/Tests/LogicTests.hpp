//
//  UnitTests.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 11/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "LogicTest.hpp"
#include "GameStorage.hpp"

class LogicTests : public LogicTest {
protected:
    Pocket::GameStorage storage;
    virtual void RunTests();
};
