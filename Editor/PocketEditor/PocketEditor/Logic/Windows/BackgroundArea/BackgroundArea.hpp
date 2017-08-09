//
//  BackgroundArea.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "BaseWindow.hpp"
#include "SelectableCollection.hpp"
#include "EditorObject.hpp"
#include "FilePath.hpp"

using namespace Pocket;

class BackgroundArea : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
    bool CreateBar() override;
private:
    void ScreenSizeChanged();
};
