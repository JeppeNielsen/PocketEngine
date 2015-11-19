//
//  PuzzleProgress.h
//  Cubism
//
//  Created by Jeppe Nielsen on 9/15/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "PuzzleDatabase.hpp"

class PuzzleProgress {
public:
    PuzzleProgress();
    ~PuzzleProgress();
    
    void SetDatabase(PuzzleDatabase* database);
    
    const Puzzle* CurrentLevel();
    
    void SetLevelIndex(int level);
    void UnlockNextLevel();
    bool IsThereNextLevel();
    
    int CurrentLevelIndex();
    
    bool IsLevelUnlocked(int levelIndex);
    
    void UpdateFromJSON(std::string json);
    
private:
    PuzzleDatabase* database;
    
    int currentLevel;
    int unlockedLevel;
    
};