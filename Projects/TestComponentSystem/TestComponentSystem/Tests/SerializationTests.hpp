//
//  SerializationTests.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 02/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "LogicTest.hpp"
#include "GameStorage.hpp"
#include "FileWorld.hpp"

class SerializationTests : public LogicTest {
protected:
    Pocket::GameStorage storage;
    Pocket::FileWorld fileWorld;
    virtual void RunTests();
};
