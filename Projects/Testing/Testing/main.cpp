//#include "TimeTests.hpp"
#include "ComponentSystemTests.hpp"
//#include "GameWorld.hpp"
/*
using namespace Pocket;

struct Bomb { int damage; };

struct BombThrower {
    BombThrower() { numberOfBombsToCreate = 5; }
    int numberOfBombsToCreate;
};

struct BombSystem : public GameSystem<Bomb> {
    void ObjectAdded(GameObject* object) {bombs++;}
    void ObjectRemoved(GameObject* object) {bombs--;}
    int bombs = 0;
};

struct BombThrowerSystem : public GameSystem<BombThrower> {
    GameWorld* world;
    void Initialize(GameWorld* world) {
        this->world = world;
    }
    void ObjectAdded(GameObject* object) {
        for (int i=0; i<object->GetComponent<BombThrower>()->numberOfBombsToCreate; i++) {
            GameObject* bomb = world->CreateObject();
            bomb->AddComponent<Bomb>();
            std::cout << "Bomb added "<< std::endl;
        }
        object->Remove();
    }
};
*/

int main() {
    //TimeTests tests;
    //tests.Run();
    
    
    ComponentSystemTests componentSystemTests;
    componentSystemTests.Run();


    /*
        GameWorld world;
        world.CreateSystem<BombThrowerSystem>();
        auto bombSystem = world.CreateSystem<BombSystem>();
        GameObject* bombThrower = world.CreateObject();
        bombThrower->AddComponent<BombThrower>();

        size_t objectsBefore = world.ObjectCount();
        int bombsBefore = bombSystem->bombs;
        world.Update(0);
        size_t objectsAfter = world.ObjectCount();
        int bombsAfter = bombSystem->bombs;
        
        bool succes = objectsBefore == 1 && bombsBefore == 0 && objectsAfter == 5 && bombsAfter == 5;
    
    std::cout << succes << std::endl;
*/
}


