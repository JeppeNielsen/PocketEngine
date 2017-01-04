//
//  GameMenu.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameMenu.hpp"
#include "Timer.hpp"

std::string GameMenu::Name() {
    return "Game";
}

void GameMenu::OnInitialize() {
}

void GameMenu::OnCreate() {
    menu->AddChild("Compile").Clicked.Bind([this] {
        Timer timer;
        std::cout << "Compilation started..."<< std::endl;
        timer.Begin();
        context->Project().Compile();
        double time = timer.End();
        std::cout << "Compilation finished, time = " << time << "s"<< std::endl;
    });
    menu->AddChild("Build").Clicked.Bind([this] {
        Timer timer;
        std::cout << "Build started..."<< std::endl;
        timer.Begin();
        context->Project().Build();
        double time = timer.End();
        std::cout << "Build finished, time = " << time << "s"<< std::endl;
    });
}