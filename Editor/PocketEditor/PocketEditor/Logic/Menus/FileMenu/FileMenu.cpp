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
#include "FileHelper.hpp"


std::string FileMenu::Name() {
    return "File";
}

void FileMenu::OnInitialize() {
}

void FileMenu::OnCreate() {

    menu->AddChild("New Project").Clicked.Bind([this] {
        std::string file = FileReader::ShowSaveFileRequester("");
        if (file!="") {
            if (FileHelper::FileExists(file)) {
                FileReader::ShowMessageBox("Path already exists, cannot create project.");
            } else {
                std::string projectName = FileHelper::GetFileNameFromPath(file);
                //std::cout << "Folder name "<<folderName << std::endl;
                FileReader::RunCommmand("mkdir " + file);
                context->Project().CreateNewWorld(file + "/Scene.json");
                context->Project().CreateSettings(file + "/Project.json", projectName);
                
                context->Project().Open(file);
            }
        }
    });
    
    menu->AddChild("Open Project").Clicked.Bind([this] {
        auto files = FileReader::ShowOpenFileRequester("", false, true);
        if (!files.empty()) {
            context->Project().Open(files[0]);
        }
    });

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
