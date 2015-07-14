//
//  Gui.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/12/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Gui.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "OctreeSystem.hpp"
#include "HierarchyOrder.hpp"
#include "TextBoxLabelSystem.hpp"
#include "Colorable.hpp"

using namespace Pocket;

Gui::Gui() { atlas = 0; }
Gui::~Gui() {}

GameObject* Gui::Atlas() { return atlas; }

void Gui::Initialize() {
    renderer = CreateSystem<RenderSystem>();
    CreateSystem<TransformHierarchy>();
    touchSystem = CreateSystem<TouchSystem>();
    CreateSystem<LabelMeshSystem>();
    CreateSystem<SpriteMeshSystem>();
    CreateSystem<SpriteTextureSystem>();
    CreateSystem<HierarchyOrder>();
    CreateSystem<DraggableSystem>();
    CreateSystem<LayoutSystem>();
    textboxSystem = CreateSystem<TextBoxSystem>();
    CreateSystem<TextBoxLabelSystem>();
    CreateSystem<MenuSystem>();
    CreateSystem<MenuButtonSystem>();
}

void Gui::Setup(const std::string &atlasTexture, const std::string &atlasXml, const Box& viewport, InputManager& inputManager) {

    atlas = World()->CreateObject();
    Texture& texture = atlas->AddComponent<TextureComponent>()->Texture();
    texture.LoadFromPng(atlasTexture);
    atlas->AddComponent<class Atlas>()->Load(atlasXml,Vector2(texture.GetWidth(), texture.GetHeight()));
    
    Setup(atlas, viewport, inputManager);
}

void Gui::Setup(Pocket::GameObject *atlas, const Pocket::Box &viewport, Pocket::InputManager &inputManager) {
    
    this->atlas = atlas;
    
    BoundingBox bounds(0, Vector3(viewport.right * 2.0f, viewport.top * 2.0f, 3000.0f));
    renderer->Octree().SetWorldBounds(bounds);
    touchSystem->Octree().SetWorldBounds(bounds);
    touchSystem->Input = &inputManager;
    
    camera = World()->CreateObject();
    camera->AddComponent<Transform>()->Position = Vector3(0,0,1);
    Camera* cam = camera->AddComponent<Camera>();
    
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
    GameObject* pivot = World()->CreateObject();
    pivot->Parent = parent;
    pivot->AddComponent<Transform>()->Position = position;
    pivot->AddComponent<Orderable>();
    return pivot;
}

GameObject* Gui::CreateControl(GameObject* parent) {
    GameObject* control = CreatePivot(parent);
    control->AddComponent<Mesh>();
    control->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    control->AddComponent<Touchable>();
    control->AddComponent<Colorable>();
    control->AddComponent<Sizeable>();
    control->AddComponent<Sprite>();
    control->AddComponent<class Atlas>(atlas);
    control->AddComponent<TextureComponent>(atlas);
    return control;
}

GameObject* Gui::CreateControl(Pocket::GameObject *parent, const std::string& spriteName, const Pocket::Vector2 &position, const Pocket::Vector2 &size) {
    
    GameObject* control = CreateControl(parent);
    
    control->GetComponent<Sizeable>()->Size = size;
    control->GetComponent<Sprite>()->SpriteName = spriteName;
    control->GetComponent<Transform>()->Position = position;
    
    return control;
}

GameObject* Gui::CreateControl(Pocket::GameObject *parent, const std::string &spriteName) {
    GameObject* control = CreateControl(parent);

    control->GetComponent<Sprite>()->SpriteName = spriteName;
    control->AddComponent<Layoutable>();

    return control;
}

GameObject* Gui::CreateClipper(Pocket::GameObject *parent, bool push) {
    GameObject* clipper = CreatePivot(parent);
    clipper->AddComponent<Mesh>(parent);
    clipper->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    clipper->GetComponent<Material>()->Clip = push ? 1 : 2;
    clipper->AddComponent<Sizeable>(parent);
    clipper->AddComponent<Sprite>(parent);
    clipper->AddComponent<Touchable>();
    return clipper;
}

GameObject* Gui::CreateFont(const std::string& fontFile, const std::string& fontAtlasName) {
    GameObject* font = World()->CreateObject();
    font->AddComponent<Font>()->Load(fontFile);
    font->GetComponent<Font>()->FontAtlasNode = fontAtlasName;
    return font;
}

GameObject* Gui::CreateLabel(Pocket::GameObject *parent, const Pocket::Vector2 &position, const Pocket::Vector2 &size, Pocket::GameObject *font, const std::string &text, float fontSize) {
    GameObject* label = CreatePivot(parent);
    label->GetComponent<Transform>()->Position = position;
    label->AddComponent<Font>(font);
    label->AddComponent<Mesh>();
    label->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
    label->AddComponent<Colorable>();
    label->AddComponent<Sizeable>()->Size = size;
    label->AddComponent<class Atlas>(atlas);
    label->AddComponent<TextureComponent>(atlas);
    label->AddComponent<Label>()->FontSize = fontSize;
    label->GetComponent<Label>()->Text = text;
    return label;
}

GameObject* Gui::CreateLabelControl(Pocket::GameObject *parent, const std::string &spriteName, const Pocket::Vector2 &position, const Pocket::Vector2 &size, Pocket::GameObject *font, std::string text, float fontSize) {
    GameObject* control = CreateControl(parent, spriteName, position, size);
    control->GetComponent<Transform>()->Anchor = size * 0.5f;
    GameObject* labelGO = CreateLabel(control, 0, size, font, text, fontSize);
    Label* label = labelGO->GetComponent<Label>();
    label->HAlignment = Font::Center;
    label->VAlignment = Font::Middle;
    
    return control;
}

GameObject* Gui::CreateMenu(Pocket::GameObject *parent, const Pocket::Vector2 &position) {
    GameObject* pivot = CreatePivot(parent);
    pivot->GetComponent<Transform>()->Position = position;
    pivot->AddComponent<Menu>();
    return pivot;
}


void Gui::AddMenuAnimator(Pocket::GameObject *control, Pocket::GameObject *menu, std::string activeMenu, Pocket::GameObject *animations, const std::string &fadeInAnimation, const std::string &fadeOutAnimation) {
    control->AddComponent<Menu>(menu);
    control->AddComponent<TransformAnimator>();
    MenuAnimator* menuAnimator = control->AddComponent<MenuAnimator>();
    menuAnimator->Menu = activeMenu;
    menuAnimator->FadeInAnimation = fadeInAnimation;
    menuAnimator->FadeOutAnimation = fadeOutAnimation;
    control->AddComponent<TransformAnimationDatabase>(animations);
}

/*

GameObject* Gui::CreateSubMenu(GameObject* parent, Pocket::Menu* parentMenu, const Pocket::Vector2 &position, const std::string &name, GameObject* animations, const std::string& fadeInAnimation, const std::string& fadeOutAnimation) {
    GameObject* pivot = CreatePivot(parent);
    pivot->GetComponent<Transform>()->Position = position;
    Menu* menu = pivot->AddComponent<Menu>();
    menu->MenuName = name;
    menu->ParentMenu = parentMenu;
    menu->FadeInAnimation = fadeInAnimation;
    menu->FadeOutAnimation = fadeOutAnimation;
    pivot->AddComponent<TransformAnimationDatabase>(animations);
    pivot->AddComponent<TransformAnimator>();
    return pivot;
}

*/

