//
//  GameMenu.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "FileMenu.hpp"
#include "Timer.hpp"
#include "FileReader.hpp"

std::string FileMenu::Name() {
    return "File";
}

void FileMenu::OnInitialize() {
}

void FileMenu::OnCreate() {
    menu->AddChild("New World").Clicked.Bind([this] {
    
        std::string path = FileReader::GetExecutablePath();
        
        std::string newWorldPath = FileReader::ShowSaveFileRequester(path);
    
        if (newWorldPath!="") {
            context->Project().CreateNewWorld(newWorldPath);
        }
    });
    
    menu->AddChild("Save World", "s").Clicked.Bind([this] {
        context->Project().SaveWorld();
    });
}