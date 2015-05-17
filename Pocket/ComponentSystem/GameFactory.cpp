//
//  GameFactory.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/21/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "GameFactory.hpp"

using namespace Pocket;

GameFactory::GameFactory() {}
GameFactory::~GameFactory() {}

GameWorld* GameFactory::World() { return world; }