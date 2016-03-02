//
//  Gui.cpp
//  PocketOSX
//
//  Created by Jeppe Nielsen on 01/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Gui.hpp"

using namespace Pocket;


GameObject* Gui::GetAtlas() { return atlas; }

void Gui::Initialize(IGameWorld* world) {
    this->world = world;
    renderer = &world->GetSystem<RenderSystem>();
    textboxSystem = &world->GetSystem<TextBoxSystem>();
    touchSystem = &world->GetSystem<TouchSystem>();
}

void Gui::Setup(const std::string &atlasTexture, const std::string &atlasXml, const Box& viewport, InputManager& inputManager) {

    atlas = world->CreateObject();
    Texture& texture = atlas->template AddComponent<TextureComponent>()->Texture();
    texture.LoadFromPng(atlasTexture);
    atlas->template AddComponent<Atlas>()->Load(atlasXml,Vector2(texture.GetWidth(), texture.GetHeight()));
    //atlas->SetID("Gui.Atlas");
    
    Setup(atlas, viewport, inputManager);
}

void Gui::Setup(GameObject *atlas, const Box &viewport, InputManager &inputManager) {
    
    this->atlas = atlas;
    
    BoundingBox bounds(0, Vector3(viewport.right * 2.0f, viewport.top * 2.0f, 3000.0f));
    renderer->Octree().SetWorldBounds(bounds);
    touchSystem->Octree().SetWorldBounds(bounds);
    touchSystem->Input = &inputManager;
    
    camera = world->CreateObject();
    camera->template AddComponent<Transform>()->Position = Vector3(0,0,1);
    Camera* cam = camera->template AddComponent<Camera>();
    
    cam->Viewport = viewport;
    cam->Orthographic = true;
    cam->Near = 1.0f;
    cam->Far = 2.0f;
    
    textboxSystem->Input = &inputManager;
}

GameObject* Gui::GetCamera() {
    return camera;
}

GameObject* Gui::CreatePivot(GameObject* parent, const Vector2& position) {
    GameObject* pivot = world->CreateObject();
    pivot->Parent = parent;
    pivot->template AddComponent<Transform>()->Position = position;
    pivot->template AddComponent<Orderable>();
    return pivot;
}

GameObject* Gui::CreateControl(GameObject* parent) {
    GameObject* control = CreatePivot(parent, 0);
    control->template AddComponent<Mesh>();
    control->template AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    control->template AddComponent<Touchable>();
    //control->template AddComponent<Colorable>();
    control->template AddComponent<Sizeable>();
    control->template AddComponent<Sprite>();
    control->template AddComponent<class Atlas>(atlas);
    control->template AddComponent<TextureComponent>(atlas);
    return control;
}

GameObject* Gui::CreateControl(GameObject *parent, const std::string& spriteName, const Vector2 &position, const Vector2 &size) {
    
    GameObject* control = CreateControl(parent);
    
    control->template GetComponent<Sizeable>()->Size = size;
    control->template GetComponent<Sprite>()->SpriteName = spriteName;
    control->template GetComponent<Transform>()->Position = position;
    
    return control;
}

GameObject* Gui::CreateControl(GameObject *parent, const std::string &spriteName, const Vector2& size) {
    GameObject* control = CreateControl(parent);
    control->template GetComponent<Sprite>()->SpriteName = spriteName;
    control->template GetComponent<Sizeable>()->Size = size;
    //control->template AddComponent<Layoutable>();
    return control;
}

GameObject* Gui::CreateClipper(GameObject *parent, bool push) {
    GameObject* clipper = CreatePivot(parent, 0);
    clipper->template AddComponent<Mesh>(parent);
    clipper->template AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    clipper->template GetComponent<Material>()->Clip = push ? 1 : 2;
    clipper->template AddComponent<Sizeable>(parent);
    clipper->template AddComponent<Sprite>(parent);
    clipper->template AddComponent<Touchable>();
    return clipper;
}

GameObject* Gui::CreateFont(const std::string& fontFile, const std::string& fontAtlasName) {
    GameObject* font = world->CreateObject();
    font->template AddComponent<Font>()->Load(fontFile);
    font->template GetComponent<Font>()->FontAtlasNode = fontAtlasName;
    fonts.push_back(font);
    return font;
}

GameObject* Gui::CreateLabel(GameObject *parent, const Vector2 &position, const Vector2 &size, GameObject *font, const std::string &text, float fontSize) {
    if (!font && !fonts.empty()) {
        font = fonts[0];
    }
    GameObject* label = CreatePivot(parent, position);
    label->template AddComponent<Font>(font);
    label->template AddComponent<Mesh>();
    label->template AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    //label->template AddComponent<Colorable>();
    label->template AddComponent<Sizeable>()->Size = size;
    label->template AddComponent<class Atlas>(atlas);
    label->template AddComponent<TextureComponent>(atlas);
    label->template AddComponent<Label>()->FontSize = fontSize;
    label->template GetComponent<Label>()->Text = text;
    return label;
}

GameObject* Gui::CreateLabelControl(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize) {
    GameObject* control = CreateControl(parent, spriteName, position, size);
    control->template GetComponent<Transform>()->Anchor = size * 0.5f;
    GameObject* labelGO = CreateLabel(control, 0, size, font, text, fontSize);
    Label* label = labelGO->template GetComponent<Label>();
    label->HAlignment = Font::Center;
    label->VAlignment = Font::Middle;
    
    return control;
}

GameObject* Gui::CreateTextBox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize) {
    GameObject* control = CreateControl(parent, spriteName, position, size);
    GameObject* labelGO = CreateLabel(control, 0, size, font, text, fontSize);
    Label* label = labelGO->template GetComponent<Label>();
    label->HAlignment = Font::Center;
    label->VAlignment = Font::Middle;
    labelGO->template AddComponent<TextBox>()->Text = text;
    labelGO->template AddComponent<Touchable>(control);
    return control;
}

/*
GameObject* CreateMenu(GameObject *parent, const Vector2 &position) {
    GameObject* pivot = CreatePivot(parent);
    pivot->template GetComponent<Transform>()->Position = position;
    pivot->template AddComponent<Menu>();
    return pivot;
}


void AddMenuAnimator(GameObject *control, GameObject *menu, std::string activeMenu, GameObject *animations, const std::string &fadeInAnimation, const std::string &fadeOutAnimation) {
    control->template AddComponent<Menu>(menu);
    control->template AddComponent<TransformAnimator>();
    MenuAnimator* menuAnimator = control->template AddComponent<MenuAnimator>();
    menuAnimator->Menu = activeMenu;
    menuAnimator->FadeInAnimation = fadeInAnimation;
    menuAnimator->FadeOutAnimation = fadeOutAnimation;
    control->template AddComponent<TransformAnimationDatabase>(animations);
}

GameObject* CreateListbox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject** pivot) {
    GameObject* listbox = CreateControl(parent, spriteName, position, size);
    listbox->template AddComponent<Layoutable>();
    listbox->template GetComponent<Touchable>()->ClickThrough = false;
    CreateClipper(listbox, true);
    GameObject* p = CreatePivot(listbox);
    p->template AddComponent<Sizeable>();
    p->template AddComponent<Layoutable>()->ChildLayouting = Layoutable::ChildLayouting::VerticalStackedFit;
    p->template GetComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
    p->template AddComponent<Touchable>(listbox);
    p->template AddComponent<Draggable>()->Movement = Draggable::MovementMode::YAxis;
    p->template AddComponent<DraggableMotion>();
    p->template AddComponent<Velocity>()->MinimumSpeedBeforeStop = 5;
    p->template GetComponent<Velocity>()->Friction = 5;
    Limitable* limitable = p->template AddComponent<Limitable>();
    limitable->Size = p->template GetComponent<Sizeable>();
    limitable->View = listbox->template GetComponent<Sizeable>();
    CreateClipper(listbox, false);
    (*pivot)=p;
    return listbox;
}
*/
