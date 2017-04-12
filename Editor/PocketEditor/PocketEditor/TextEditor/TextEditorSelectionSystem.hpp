//
//  TextEditorSelectionSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 05/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "GameSystem.hpp"
#include "TextEditor.hpp"
#include "Sizeable.hpp"
#include "TextEditorRenderer.hpp"
#include "Transform.hpp"
#include "Touchable.hpp"
#include "Font.hpp"
#include <set>

struct TextEditorSelectionSystem : public GameSystem<Transform, TextEditor, TextEditorRenderer, Sizeable, Touchable, Font> {

    void Initialize();
    
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
    void Down(TouchData e, GameObject* object);
    void Up(TouchData e, GameObject* object);
    
    void Update(float dt);

    int GetCursorPosition(GameObject* object, Vector3 worldPosition);
    
    
    struct DownObject {
        GameObject* object;
        TouchData touchData;
    };
    
    bool isDown;
    DownObject downObject;
};
