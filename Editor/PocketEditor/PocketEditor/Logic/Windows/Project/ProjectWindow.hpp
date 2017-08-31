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
#include "FilePath.hpp"
#include "EditorGui.hpp"

using namespace Pocket;

namespace Pocket {
    class FileSystemListener;
    class VirtualTreeList;
}

class ProjectWindow : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
    bool CreateBar() override;
    void SetProjectPath(const std::string& path);
private:
    void UpdateFileWorld();
    
    struct ClickedNodeInfo {
        GameObject* fileObject;
        GameObject* guiNodeObject;
        FilePath* filePath;
    };
    
    void Clicked(TouchData d, ClickedNodeInfo nodeInfo);
    GameObject* fileRoot;
    GameObject* listBox;
    AppMenu popupMenu;
    ClickedNodeInfo selectedNode;
    FileSystemListener* fileSystemListener;
    VirtualTreeList* treeView;
    
    FilePath projectFilePath;
    EditorGui* editorGui;
};
