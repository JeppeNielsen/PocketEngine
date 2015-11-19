//
//  PuzzleProgress.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 9/15/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PuzzleProgress.hpp"
#include <iostream>

PuzzleProgress::PuzzleProgress() {
    currentLevel = 0;
    unlockedLevel = 0;
    database = 0;
}

PuzzleProgress::~PuzzleProgress() {}

void PuzzleProgress::SetDatabase(PuzzleDatabase *database) {
    this->database = database;
}

const Puzzle* PuzzleProgress::CurrentLevel() {
    if (!database) return 0;
    if (database->Puzzles().size()<=0) return 0;
    int index = currentLevel;
    if (index>=database->Puzzles().size()) {
        index = (int)database->Puzzles().size() - 1;
    }
    return &database->Puzzles()[currentLevel];
}

void PuzzleProgress::SetLevelIndex(int level) {
    currentLevel = level;
}

void PuzzleProgress::UnlockNextLevel() {
    currentLevel++;
    if (currentLevel>unlockedLevel) {
        unlockedLevel = currentLevel;
    }
}

bool PuzzleProgress::IsThereNextLevel() {
    if (!database) return false;
    return currentLevel<database->Puzzles().size();
}

int PuzzleProgress::CurrentLevelIndex() { return currentLevel; }

bool PuzzleProgress::IsLevelUnlocked(int levelIndex) {
    return levelIndex<=unlockedLevel;
}

void PuzzleProgress::UpdateFromJSON(std::string json) {
    
    /*
    const char* jsonCString = json.c_str();
    
    minijson::value root;
    
    minijson::error e = minijson::parse(jsonCString, root);
    if (e == minijson::no_error) {
        unlockedLevel = (int)atoi(root.get<minijson::array>()[0].get<minijson::object>()["Level"].get<std::string>().c_str());
    } else {
        std::cout<<"Error parsing json:"<<std::endl<<minijson::errstr(e)<<std::endl<<json<<std::endl;
    }
    */
}



