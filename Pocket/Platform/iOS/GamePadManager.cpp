//
//  GamePadManager.cpp
//  NanoIOS
//
//  Created by Jeppe Nielsen on 4/13/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GamePadManager.hpp"

using namespace Nano;

GamePadManager::GamePadManager() { }
GamePadManager::~GamePadManager() { Destroy(); }

void GamePadManager::Initialize() { }

void GamePadManager::Destroy() { }

void GamePadManager::Print() { }

void GamePadManager::SetButtonState(int gamePadID, GamePadButton button, bool down) { }

void GamePadManager::SetAnalogState(int gamePadID, GamePadButton button, Vector2 value) { }

Vector2 GamePadManager::GetAnalogState(int gamePadID, GamePadButton button) { return 0; }

void GamePadManager::Update() { }
void GamePadManager::UpdateButtons() { }

void GamePadManager::UpdateAnalogs() { }

void GamePadManager::UpdateAddedRemovedGamePads() { }

void GamePadManager::AddGamePad(int gamePadID) { }

void GamePadManager::RemoveGamePad(int gamePadID) { }

void GamePadManager::GamePadAdded(int gamePadID) { }

void GamePadManager::GamePadRemoved(int gamePadID) { }

void GamePadManager::GamePad::UpdateButtons(Nano::GamePadManager &manager) { }

void GamePadManager::GamePad::UpdateAnalogs(GamePadManager& manager) { }