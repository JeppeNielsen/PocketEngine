//
//  AnimationWindow.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "BaseWindow.hpp"
#include "SelectableCollection.hpp"
#include "EditorObject.hpp"
#include "VirtualTreeList.hpp"
#include "Animation.hpp"
#include "Animator.hpp"
#include "Touchable.hpp"

class AnimationWindow : public BaseWindow {
protected:
    void OnInitialize() override;
    void OnCreate() override;
    std::string Name() override;
    Vector2 Size() override;
    void ActiveWorldChanged(OpenWorld* old, OpenWorld* current) override;
    
    void EditorRootChanged(OpenWorld *world);
    void ChangeEditorRoot(Pocket::GameObject *old, Pocket::GameObject *current);
    void SelectionChanged(SelectableCollection<EditorObject>* selectables);
    void Clear();
    
private:

    float TimeToPosition(float time);
    float PositionToTime(float position);
    void TimeChanged();
    void UpdateTime(float dt, int index);
    void Record(float dt);
    void StoreValues();
    void Refresh();
    
    VirtualTreeList* treeView;
    GameObject* rootItem;
    GameObject* animationObject;
    Animation* animation;
    GameObject* animationRoot;
    Animator* animator;
    Property<float> Zoom;
    Property<bool> IsRecording;
    bool isScrupping;
    
    GameObject* timeIndicator;
    GameObject* timeline;
    GameObject* recordButton;
    
    struct TimeLineInfo {
        TimeLineInfo() : timeline(0) {}
        Animation::Path path;
        std::string name;
        IFieldInfoTimeline* timeline;
    };
    
    std::map<GameObject*, TimeLineInfo> timelines;
    
    SelectableCollection<EditorObject>* selectables;

    std::vector<std::string> previousValues;
    
    std::vector<int> nonRecordableComponents;
};
