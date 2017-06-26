//
//  GameMenu.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameMenu.hpp"
#include "Timer.hpp"
#include "FileReader.hpp"
#include "FileHelper.hpp"
#include "ProjectBuilder.hpp"

std::string GameMenu::Name() {
    return "Game";
}

void GameMenu::OnInitialize() {
}

void GameMenu::OnCreate() {
    menu->AddChild("Compile").Clicked.Bind([this] {
        
        context->Project().Compile();
        //context->Log().Log(error);
        
    });
    menu->AddChild("Create Executable").Clicked.Bind([this] {
    
        auto files = FileReader::ShowOpenFileRequester("", false, true);
        if (!files.empty()) {
                Timer timer;
                std::cout << "Build started... at path: "<< files[0] << std::endl;
                timer.Begin();
                context->Project().BuildExecutable(files[0], [this] (const std::string& output) {
                    context->Log().Log(output);
                });
                double time = timer.End();
                std::cout << "Build finished, time = " << time << "s"<< std::endl;
        }
        
    });
    
    menu->AddChild("Create IOS executable").Clicked.Bind([this] {
    
        auto files = FileReader::ShowOpenFileRequester("", false, true);
        if (!files.empty()) {
            
        
    
            Timer timer;
            std::cout << "Build started... at path: "<< files[0] << std::endl;
            timer.Begin();
            
            
            context->Project().BuildExecutable(files[0], [this] (const std::string& output) {
                    context->Log().Log(output);
                });
            
            //context->Project().Builder().IOS.Build(files[0], "/Projects/PocketEngine/Projects/Libraries/iOS/PocketEngine/Build/Build/Products/Debug-iphoneos/libPocketEngine.a");
            double time = timer.End();
            std::cout << "Build finished, time = " << time << "s"<< std::endl;
        }
        
    });
}
