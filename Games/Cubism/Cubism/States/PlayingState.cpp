#include "PlayingState.hpp"
#include "BlockMeshSystem.hpp"
#include "MathHelper.hpp"
#include "DraggableSystem.hpp"
#include "TouchSystem.hpp"
#include "BlockRotaterSystem.hpp"
#include "BlockTransformer.hpp"
#include "PatchMeshSystem.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "TransformHierarchy.hpp"
#include "BlockPatchSnapSystem.hpp"
#include "BlockMoverSystem.hpp"
#include "BlockParentSystem.hpp"
#include "BlockBlockedSystem.hpp"
#include "PuzzleCompletionChecker.hpp"

using namespace Pocket;

void PlayingState::Initialize() {
    
    renderer = world.CreateSystem<RenderSystem>();
    world.CreateSystem<BlockMeshSystem>();
    world.CreateSystem<PatchMeshSystem>();
    world.CreateSystem<TouchSystem>();
    //world.CreateSystem<DraggableSystem>();
    world.CreateSystem<BlockRotatorSystem>();
    world.CreateSystem<BlockTransformer>();
    world.CreateSystem<TransformHierarchy>();
    puzzleSystem = world.CreateSystem<PuzzleSystem>();
    world.CreateSystem<PuzzleCompletionChecker>()->PuzzleComplete += event_handler(this, &PlayingState::PuzzleIsComplete);
    puzzleDestructorSystem = world.CreateSystem<PuzzleDestructorSystem>();
    
    BlockMoverSystem* blockMoverSystem = world.CreateSystem<BlockMoverSystem>();
    BlockPatchSnapSystem* blockPatchSnapSystem = world.CreateSystem<BlockPatchSnapSystem>();
    BlockParentSystem* blockParentSystem = world.CreateSystem<BlockParentSystem>();
    world.CreateSystem<BlockBlockedSystem>();
    blockParentSystem->moverSystem = blockMoverSystem;
    
    world.CreateSystem<FirstPersonMoverSystem>();
    
    cameraPivot = world.CreateObject();
    cameraPivot->AddComponent<Transform>()->Position = {0,0,0};
    FirstPersonMover* firstPersonMover = cameraPivot->AddComponent<FirstPersonMover>();
    firstPersonMover->MovementSpeed = 0;
    firstPersonMover->RotationSpeed *= 2;
    
    camera = world.CreateObject();
    camera->Parent = cameraPivot;
    camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
    camera->GetComponent<Camera>()->FieldOfView = 64;
    camera->AddComponent<Transform>()->Position = { 0, 0, 16 };
    
    
    GameObject* blockTable = world.CreateObject();
    blockTable->Parent = cameraPivot;
    blockTable->AddComponent<Transform>()->Position = {0,0,16};
    
    blockPatchSnapSystem->camera = camera->GetComponent<Camera>();
    blockPatchSnapSystem->cameraObject = camera;
    
    blockMoverSystem->space = blockTable;
    blockParentSystem->parent = blockTable;
    
    
    MathHelper::SeedRandom();
    
    
    
    Input.ButtonDown += event_handler(this, &PlayingState::KeyDown);
    
    
    
    
    
    Gui* gui = Parent().gui;
    Menu* menu = Parent().menu;
    GameObject* menuObject = Parent().menuObject;
    
    {
        GameObject* inGameMenu = gui->CreatePivot(0, Manager().ScreenSize() * 0.5f);
        gui->AddMenuAnimator(inGameMenu, menuObject, "Playing", Parent().animations, "FadeInTop", "FadeOutTop");
    
        
        GameObject* exitButton = Parent().CreateButton(inGameMenu, "Button", Manager().ScreenSize() * 0.5f - Vector2(50,50), {100,100}, "X", 20);
        exitButton->GetComponent<Touchable>()->Click += event_handler(this, &PlayingState::BackClicked);
        
        levelName = gui->CreateLabel(inGameMenu, {0,Manager().ScreenSize().y * 0.5f - 20}, 0, Parent().font, "", 20);
        levelName->GetComponent<Label>()->HAlignment = Font::Center;
        levelName->GetComponent<Label>()->VAlignment = Font::Middle;

    }
    
    {
        GameObject* puzzleComplete = gui->CreatePivot(0, Manager().ScreenSize() * 0.5f);
        gui->AddMenuAnimator(puzzleComplete, menuObject, "PuzzleComplete", Parent().animations, "FadeInTop", "FadeOutTop");
        
        GameObject* label = gui->CreateLabel(puzzleComplete, {0,200}, 0, Parent().font, "PUZZLE COMPLETE!", 50);
        label->GetComponent<Label>()->HAlignment = Font::Center;
        label->GetComponent<Label>()->VAlignment = Font::Middle;
        
        GameObject* nextButton = Parent().CreateButton(puzzleComplete, "Button", {300,-100}, {300,150}, "NEXT", 20);
        nextButton->GetComponent<Touchable>()->Click += event_handler(this, &PlayingState::NextClicked);
        
        GameObject* exitButton = Parent().CreateButton(puzzleComplete, "Button", {-300,-100}, {200,100}, "MENU", 20);
        exitButton->GetComponent<Touchable>()->Click += event_handler(this, &PlayingState::BackClicked);
        
    }
    
    lastPlayedPuzzleIndex = -1;
}

void PlayingState::Enabled() {
    if (Parent().puzzleProgress.CurrentLevelIndex() != lastPlayedPuzzleIndex) {
        CreateNewPuzzle();
    }
    
    Parent().menu->ActiveMenu.Changed += event_handler(this, &PlayingState::MenuChanged);
    Parent().menu->ActiveMenu = "Playing";
}

void PlayingState::Disabled() {
    Parent().menu->ActiveMenu = "Levels";
    Parent().menu->ActiveMenu.Changed -= event_handler(this, &PlayingState::MenuChanged);
}

void PlayingState::MenuChanged(Pocket::Menu *menu) {
    InputManager* input =((menu->ActiveMenu() == "Playing") ? &Input : 0);
    world.GetSystem<TouchSystem>()->Input = input;
    world.GetSystem<FirstPersonMoverSystem>()->Input = input;
}

void PlayingState::BackClicked(Pocket::TouchData d) {
    Parent().CurrentState = "Menu";
}

void PlayingState::NextClicked(Pocket::TouchData d) {
    if (Parent().puzzleProgress.IsThereNextLevel()) {
        CreateNewPuzzle();
        Parent().menu->ActiveMenu = "Playing";
    } else {
        BackClicked(d);
    }
}

void PlayingState::KeyDown(std::string button) {
    if (button == "n") {
        CreateNewPuzzle();
    } else if (button == "d") {
        puzzleDestructorSystem->DestroyPuzzle();
    }
}

void PlayingState::CreateNewPuzzle() {
    const Puzzle* puzzle = Parent().puzzleProgress.CurrentLevel();
    lastPlayedPuzzleIndex = Parent().puzzleProgress.CurrentLevelIndex();
    camera->GetComponent<Transform>()->Rotation = Quaternion::LookAt(camera->GetComponent<Transform>()->Position, 0, {0,1,0});
    puzzleDestructorSystem->DestroyPuzzle();
    world.Update(0.001f);
    puzzleSystem->CreatePuzzle(*puzzle);
    puzzleSystem->MoveBlocks(camera);
    levelName->GetComponent<Label>()->Text = puzzle->Name;
}

void PlayingState::Update(float dt) {
    world.Update(dt);
}

void PlayingState::Render() {
    renderer->Render();
    Parent().world.Render();
}

void PlayingState::PuzzleIsComplete(bool dummy) {
    Parent().puzzleProgress.UnlockNextLevel();
    std::cout<<"Puzzle is complete"<<std::endl;
    Parent().menu->ActiveMenu = "PuzzleComplete";
}



