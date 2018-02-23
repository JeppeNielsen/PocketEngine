//
//  Gui.cpp
//  PocketOSX
//
//  Created by Jeppe Nielsen on 01/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "Gui.hpp"
#include "FontTextureSystem.hpp"
#include "FileReader.hpp"
#include <fstream>
#include "GameObjectJsonSerializer.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"

using namespace Pocket;

GameObject* Gui::GetAtlas() { return atlas; }

void Gui::CreateSubSystems(Pocket::GameStorage &storage) {
    storage.AddSystemType<RenderSystem>();
    storage.AddSystemType<TextBoxSystem>();
    storage.AddSystemType<TouchSystem>();
    
    storage.AddSystemType<TransformHierarchy>();
    
    storage.AddSystemType<SpriteMeshSystem>();
    storage.AddSystemType<SpriteTextureSystem>();
    storage.AddSystemType<HierarchyOrder>();
    storage.AddSystemType<DraggableSystem>();
    storage.AddSystemType<LayoutSystem>();
    storage.AddSystemType<ColorSystem>();
    storage.AddSystemType<DraggableMotionSystem>();
    storage.AddSystemType<VelocitySystem>();
    storage.AddSystemType<LimitableSystem>();
    storage.AddSystemType<SelectedColorerSystem>();
    storage.AddSystemType<DroppableSystem>();
    storage.AddSystemType<DraggableMotionSystem>();
    storage.AddSystemType<LayoutSystem>();
    storage.AddSystemType<FontTextureSystem>();
    storage.AddSystemType<LabelMeshSystem>();
    storage.AddSystemType<TextBoxLabelSystem>();
    
    storage.AddSystemType<PanelDropSystem>();
    storage.AddSystemType<PanelSystem>();
    storage.AddSystemType<PanelAreaSystem>();
    storage.AddSystemType<TouchCancelSystem>();
    
    storage.AddSystemType<HoverSystem>();
    storage.AddSystemType<ScrollWheelMoverSystem>();
    storage.AddSystemType<VirtualTreeListSystem>();
    storage.AddSystemType<VirtualTreeListSpawnerSystem>();
}

void Gui::Initialize() {
    renderer = root->CreateSystem<RenderSystem>();
    textboxSystem = root->CreateSystem<TextBoxSystem>();
    touchSystem = root->CreateSystem<TouchSystem>();
    
    root->CreateSystem<TransformHierarchy>();
    
    root->CreateSystem<SpriteMeshSystem>();
    root->CreateSystem<SpriteTextureSystem>();
    root->CreateSystem<HierarchyOrder>()->Order = 1000;
    root->CreateSystem<DraggableSystem>();
    root->CreateSystem<LayoutSystem>();
    root->CreateSystem<ColorSystem>();
    root->CreateSystem<DraggableMotionSystem>();
    root->CreateSystem<VelocitySystem>();
    root->CreateSystem<LimitableSystem>();
    root->CreateSystem<SelectedColorerSystem>();
    root->CreateSystem<DroppableSystem>();
    root->CreateSystem<DraggableMotionSystem>();
    root->CreateSystem<LayoutSystem>();
    root->CreateSystem<FontTextureSystem>()->Order = -1;
    root->CreateSystem<LabelMeshSystem>();
    root->CreateSystem<TextBoxLabelSystem>();
    
    root->CreateSystem<PanelDropSystem>()->Order = -1;
    root->CreateSystem<PanelSystem>()->Order = -1;
    root->CreateSystem<PanelAreaSystem>()->Order = -1;
    root->CreateSystem<TouchCancelSystem>();
    
    hoverSystem = root->CreateSystem<HoverSystem>();
    root->CreateSystem<ScrollWheelMoverSystem>();
    root->CreateSystem<VirtualTreeListSystem>();
    root->CreateSystem<VirtualTreeListSpawnerSystem>();
}

void Gui::Setup(const std::string &atlasTexture, const std::string &atlasXml, const Rect& viewport) {

    auto atlasFile = FileReader::GetFile(atlasXml);
    std::ifstream file;
    file.open(atlasFile);
    GameObjectJsonSerializer serializer;
    atlas = root->CreateChild();
    serializer.Deserialize(atlas, file);
    file.close();
    Texture& texture = atlas->AddComponent<TextureComponent>()->Texture();
    texture.LoadFromFile(atlasTexture);
    texture.DisableMipmapping();
    
    Setup(atlas, viewport);
}

void Gui::Setup(GameObject *atlas, const Rect &viewport) {
    
    this->atlas = atlas;
    
    BoundingBox bounds(0, Vector3(viewport.width * 2.0f, viewport.height * 2.0f, 3000.0f));
    renderer->Octree().SetWorldBounds(bounds);
    touchSystem->Octree().SetWorldBounds(bounds);
    hoverSystem->Octree().SetWorldBounds(bounds);
    
    camera = root->CreateObject();
    camera->AddComponent<Transform>()->Position = Vector3(0,0,1);
    Camera* cam = camera->AddComponent<Camera>();
    
    cam->Orthographic = true;
    cam->Near = 1.0f;
    cam->Far = 2.0f;
    cam->FieldOfView = 0.0f;
}

GameObject* Gui::GetCamera() {
    return camera;
}

GameObject* Gui::CreatePivot() {
    return CreatePivot(root, {0,0});
}

GameObject* Gui::CreatePivot(GameObject *parent) {
    return CreatePivot(parent ? parent : root,{0,0});
}

GameObject* Gui::CreatePivot(GameObject* parent, const Vector2& position) {
    if (!parent) parent = root;
    GameObject* pivot = parent->CreateChild();
    pivot->AddComponent<Transform>()->Position = position;
    pivot->AddComponent<Orderable>();
    return pivot;
}

GameObject* Gui::CreateControl(GameObject* parent) {
    GameObject* control = CreatePivot(parent, 0);
    control->AddComponent<Mesh>();
    control->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    control->AddComponent<Touchable>();
    control->AddComponent<Colorable>();
    control->AddComponent<Sizeable>();
    control->AddComponent<Sprite>();
    control->AddComponent<class Atlas>(atlas);
    control->AddComponent<TextureComponent>(atlas);
    return control;
}

GameObject* Gui::CreateControl(GameObject *parent, const std::string& spriteName) {
    
    GameObject* control = CreateControl(parent);
    control->GetComponent<Sprite>()->SpriteName = spriteName;
    
    return control;
}

GameObject* Gui::CreateControl(GameObject *parent, const std::string& spriteName, const Vector2 &position, const Vector2 &size) {
    
    GameObject* control = CreateControl(parent, spriteName);
    
    control->GetComponent<Sizeable>()->Size = size;
    control->GetComponent<Transform>()->Position = position;
    
    return control;
}

GameObject* Gui::CreateControl(GameObject *parent, const std::string &spriteName, const Vector2& size) {
    GameObject* control = CreateControl(parent);
    control->GetComponent<Sprite>()->SpriteName = spriteName;
    control->GetComponent<Sizeable>()->Size = size;
    return control;
}

GameObject* Gui::CreateClipper(GameObject *parent, bool push) {
    GameObject* clipper = CreatePivot(parent, 0);
    clipper->AddComponent<Mesh>(parent);
    clipper->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    clipper->GetComponent<Renderable>()->Clip = push ? 1 : 2;
    clipper->AddComponent<Sizeable>(parent);
    clipper->AddComponent<Sprite>(parent);
    clipper->AddComponent<Touchable>();
    return clipper;
}

GameObject* Gui::CreateFont(const std::string& fontFile) {
    GameObject* font = root->CreateObject();
    font->AddComponent<Font>()->LoadTTF(fontFile);
    font->GetComponent<Font>()->CharacterSetEverySize = 6;
    font->GetComponent<Font>()->maxTextureWidth = 1024;
    font->GetComponent<Font>()->maxTextureHeight = 1024;
    
    font->AddComponent<TextureComponent>();
    //font->SetID(fontFile);
    fonts.push_back(font);
    return font;
}

GameObject* Gui::CreateLabel(GameObject *parent, const Vector2 &position, const Vector2 &size, GameObject *font, const std::string &text, float fontSize) {
    if (!font && !fonts.empty()) {
        font = fonts[0];
    }
    GameObject* label = CreatePivot(parent, position);
    label->AddComponent<Font>(font);
    label->AddComponent<Mesh>();
    label->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    label->AddComponent<Colorable>();
    label->AddComponent<Sizeable>()->Size = size;
    label->AddComponent<TextureComponent>(font);
    label->AddComponent<Label>()->FontSize = fontSize;
    label->GetComponent<Label>()->Text = text;
    return label;
}

GameObject* Gui::CreateLabelControl(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize) {
    GameObject* control = CreateControl(parent, spriteName, position, size);
    GameObject* labelGO = CreateLabel(control, 0, size, font, text, fontSize);
    Label* label = labelGO->GetComponent<Label>();
    label->HAlignment = Font::HAlignment::Center;
    label->VAlignment = Font::VAlignment::Middle;
    labelGO->ReplaceComponent<Sizeable>(control);
    
    return control;
}

GameObject* Gui::CreateTextBox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject *font, std::string text, float fontSize) {
    GameObject* control = CreateControl(parent, spriteName, position, size);
    //GameObject* labelGO = CreateLabel(control, 0, size, font, text, fontSize);
    
    if (!font && !fonts.empty()) {
        font = fonts[0];
    }

    GameObject* labelGO = CreatePivot(control);
    labelGO->AddComponent<Font>(font);
    labelGO->AddComponent<Mesh>();
    labelGO->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    labelGO->AddComponent<Colorable>();
    labelGO->AddComponent<Sizeable>(control);
    labelGO->AddComponent<TextureComponent>(font);
    labelGO->AddComponent<Label>()->FontSize = fontSize;
    labelGO->GetComponent<Label>()->Text = text;

    Label* label = labelGO->GetComponent<Label>();
    label->HAlignment = Font::HAlignment::Center;
    label->VAlignment = Font::VAlignment::Middle;
    labelGO->AddComponent<TextBox>()->Text = text;
    labelGO->AddComponent<Touchable>(control);
    labelGO->GetComponent<Colorable>()->Color = Colour::Black();
    return control;
}

GameObject* Gui::CreateListbox(GameObject *parent, const std::string &spriteName, const Vector2 &position, const Vector2 &size, GameObject** pivot) {
    GameObject* listbox = CreateControl(parent, spriteName, position, size);
    listbox->AddComponent<Layouter>()->ChildrenLayoutMode = Layouter::LayoutMode::Vertical;
    listbox->GetComponent<Touchable>()->ClickThrough = false;
    listbox->AddComponent<Hoverable>();
    CreateClipper(listbox, true);
    GameObject* p = CreatePivot(listbox);
    p->AddComponent<Sizeable>(listbox);
    p->AddComponent<Touchable>(listbox);
    //p->AddComponent<Draggable>()->Movement = Draggable::MovementMode::YAxis;
    //p->AddComponent<DraggableMotion>();
    //p->AddComponent<Velocity>()->MinimumSpeedBeforeStop = 5;
    //p->GetComponent<Velocity>()->Friction = 5;
    p->AddComponent<Hoverable>(listbox);
    p->AddComponent<ScrollWheelMover>()->Movement = {0,-100,0};
    p->GetComponent<ScrollWheelMover>()->Speed = 1.0f;
    
    //Limitable* limitable = p->AddComponent<Limitable>();
    //limitable->Size = p->GetComponent<Sizeable>();
    //limitable->View = listbox->GetComponent<Sizeable>();
    CreateClipper(listbox, false);
    (*pivot)=p;
    return listbox;
}

const Gui::Fonts& Gui::GetFonts() const {
    return fonts;
}

GameObject* Gui::CreateLayout(GameObject *parent, const Vector2 &minSize, const Vector2 &desiredSize, const Vector2 &maxSize, Layouter::LayoutMode layoutMode) {
    GameObject* control = CreatePivot(parent);
    control->AddComponent<Sizeable>();
    Layouter* l = control->AddComponent<Layouter>();
    l->Min = minSize;
    l->Desired = desiredSize;
    l->Max = maxSize;
    l->ChildrenLayoutMode = layoutMode;
    return control;
}

GameObject* Gui::CreateLayoutControl(GameObject *parent, const std::string &spriteName, const Vector2 &minSize, const Vector2 &desiredSize, const Vector2 &maxSize, Layouter::LayoutMode layoutMode) {
    GameObject* control = CreateControl(parent, spriteName);
    Layouter* l = control->AddComponent<Layouter>();
    l->Min = minSize;
    l->Desired = desiredSize;
    l->Max = maxSize;
    l->ChildrenLayoutMode = layoutMode;
    return control;
}

void Gui::AddLayouter(GameObject* object, const Vector2& minSize, const Vector2& desiredSize, const Vector2& maxSize, Layouter::LayoutMode layoutMode) {
    if (!object->GetComponent<Sizeable>()) {
        object->AddComponent<Sizeable>();
    }
    
    Layouter* l = object->AddComponent<Layouter>();
    l->Min = minSize;
    l->Desired = desiredSize;
    l->Max = maxSize;
    l->ChildrenLayoutMode = layoutMode;
}
