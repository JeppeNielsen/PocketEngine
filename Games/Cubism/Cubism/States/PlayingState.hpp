#include "GameState.hpp"
#include "Game.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "PuzzleSystem.hpp"
#include "PuzzleDestructorSystem.hpp"

using namespace Pocket;

class PlayingState : public GameState<Game> {
private:
    GameWorld world;
    RenderSystem* renderer;
    GameObject* cameraPivot;
    GameObject* camera;
    PuzzleSystem* puzzleSystem;
    PuzzleDestructorSystem* puzzleDestructorSystem;
    
    void PuzzleIsComplete(bool dummy);
    
    void CreateNewPuzzle();
    
    void KeyDown(std::string button);
    
    void MenuChanged(Menu* menu);
    
    void BackClicked(TouchData d);
    void NextClicked(TouchData d);
    
    int lastPlayedPuzzleIndex;
    GameObject* levelName;
    
public:
    void Initialize();
    void Update(float dt);
    void Render();
    void Enabled();
    void Disabled();
};
