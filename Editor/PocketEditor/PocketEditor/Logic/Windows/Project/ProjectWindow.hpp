//
//  ProjectWindow.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "BaseWindow.hpp"
#include "SelectableCollection.hpp"
#include "EditorObject.hpp"

class FilePath;
class FileSystemListener;

class ProjectWindow : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
    bool CreateBar() override;
private:
    void ScreenSizeChanged();
    void Clicked(TouchData d, GameObject* object);
    GameObject* fileRoot;
    GameObject* listBox;
    AppMenu popupMenu;
    FilePath* selectedPath;
    FileSystemListener* fileSystemListener;
};