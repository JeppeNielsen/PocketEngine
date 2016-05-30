#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "BigButtonManager.hpp"
#include "Mesh.hpp"
#include "VertexMesh.hpp"
#include "Gui.hpp"
#include "ColorSystem.hpp"

using namespace Pocket;

struct Player {
    Player() { Score = 0; }
    int index;
    Property<int> Score;
};

struct TimedLife {
    float time;
};

struct Blockable {

};

struct Collidable;

struct TimedLifeSystem : public GameSystem<TimedLife> {
    void Update(float dt) {
        for (auto o : Objects()) {
            auto timedLife = o->GetComponent<TimedLife>();
            if (timedLife->time>0) {
                timedLife->time -= dt;
            } else {
                o->Remove();
            }
        }
    }
};

struct PointTransform {
    PointTransform() { x=0; y=0; rotation=0; }
    int x;
    int y;
    int rotation;
    void SetRotation(int targetRotation) {
        if (IsRotationOpposite(rotation, targetRotation)) {
            return;
        }
        rotation = targetRotation;
    }
    void SetPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool IsRotationOpposite(int rotation1, int rotation2) {
        if (rotation1<0) rotation1+=4;
        rotation1 %= 4;
        if (rotation2<0) rotation2+=4;
        rotation2 %= 4;
        int delta = rotation1 - rotation2;
        if (delta<0) delta =-delta;
        return delta == 2;
    }
};

struct Moveable {
    Moveable() { timer = 0; timeBetweenMoves = 0.4f; }
    float timeBetweenMoves;
    float timer;
    
    Event<Moveable*> Moved;
    
    void ModifyValues(int& x, int& y, int direction) {
        struct dir {
            int dx; int dy;
        };
        const dir directions[4] { { 1,0}, {0,-1}, {-1,0}, {0,1}};
        int normalizedDirection = direction % 4;
        x += directions[normalizedDirection].dx;
        y += directions[normalizedDirection].dy;
    }
    
};

struct MoveSystem : public GameSystem<PointTransform, Moveable> {
    

    void Update(float dt) {
        for (auto o : Objects()) {
            auto movable = o->GetComponent<Moveable>();
            auto transform = o->GetComponent<PointTransform>();
            if (movable->timer>movable->timeBetweenMoves) {
                movable->timer = 0;
                movable->ModifyValues(transform->x, transform->y, transform->rotation);
                movable->Moved(movable);
            } else {
                movable->timer += dt;
            }
        }
    }
};

struct PointTransformSystem : public GameSystem<PointTransform, Transform> {
    void Update(float dt) {
        for (auto o : Objects()) {
            auto p = o->GetComponent<PointTransform>();
            o->GetComponent<Transform>()->Position = Vector2(p->x, p->y);
        }
    }
};

struct Snake {
    int prevX;
    int prevY;
    float tailLife;
    std::vector<GameObject*> bodies;
    Event<> OnCollision;
};

struct SnakeBody {
    Snake* snake;
};

struct SnakeSystem : public GameSystem<Snake, Moveable, PointTransform> {

    GameWorld* world;
    void Initialize(GameWorld* world) {
        this->world = world;
    }

    void Update(float dt) {
        for(auto o : Objects()) {
            UpdateSnake(o);
        }
    }
    
    void UpdateSnake(GameObject* snakeObject) {
        Snake* snake = snakeObject->GetComponent<Snake>();
        PointTransform* transform = snakeObject->GetComponent<PointTransform>();
        if (transform->x!=snake->prevX || transform->y!=snake->prevY) {
            snake->prevX = transform->x;
            snake->prevY = transform->y;
            SpawnNewTailPiece(snakeObject, snake);
        }
    }

    void SpawnNewTailPiece(GameObject* snakeObject, Snake* snake) {
        GameObject* object = world->CreateObject();
        object->AddComponent<Transform>();
        object->CloneComponent<PointTransform>(snakeObject);
        object->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1.0f, snakeObject->GetComponent<Mesh>()->GetMesh<Vertex>().vertices[0].Color);
        object->AddComponent<Material>();
        object->AddComponent<TimedLife>()->time = snakeObject->GetComponent<Snake>()->tailLife;
        object->AddComponent<SnakeBody>()->snake = snake;
        object->AddComponent<Blockable>();
        object->AddComponent<Collidable>();
        snake->bodies.push_back(object);
    }
};

struct SnakeBodyRemovalSystem : public GameSystem<SnakeBody> {
    void ObjectRemoved(GameObject* object) {
        auto& list = object->GetComponent<SnakeBody>()->snake->bodies;
        list.erase(std::remove(list.begin(), list.end(), object));
    }
};

struct SnakeControlSystem : public GameSystem<Player, Snake, PointTransform> {

    void SetInput(InputManager& input) {
        input.ButtonDown.Bind(this, &SnakeControlSystem::ButtonDown);
    }

    void SetBigButtons(BigButtonManager& bigButtons) {
        bigButtons.ButtonDown.Bind(this, &SnakeControlSystem::BigButtonDown);
    }

    void BigButtonDown(BigButtonEvent e) {
        static std::map<BigButtonButton, int> buttonToRotation = {
         { BigButtonButton::Right, 0},
         { BigButtonButton::Down, 1},
         { BigButtonButton::Left, 2},
         { BigButtonButton::Up, 3},
        };
        SetRotation(e.PlayerIndex, buttonToRotation[e.Button]);
    }

    void SetRotation(int index, int rotation) {
        for(auto o : Objects()) {
            if (o->GetComponent<Player>()->index == index) {
                o->GetComponent<PointTransform>()->SetRotation(rotation);
            }
        }
    }

    void ButtonDown(std::string button) {
        if (button == "a") {
            for(auto o : Objects()) {
                o->GetComponent<PointTransform>()->SetRotation(2);
            }
        } else if (button == "d") {
            for(auto o : Objects()) {
                o->GetComponent<PointTransform>()->SetRotation(0);
            }
        } else if (button == "w") {
            for(auto o : Objects()) {
                o->GetComponent<PointTransform>()->SetRotation(3);
            }
        } else if (button == "s") {
            for(auto o : Objects()) {
                o->GetComponent<PointTransform>()->SetRotation(1);
            }
        }
    }
};

struct Collidable {
    struct CollisionEvent {
        Collidable* collidable;
        Collidable* other;
        GameObject* object;
        GameObject* otherObject;
    };

    Event<CollisionEvent> OnCollision;
};

template<typename ...T>
struct CollidableSystem : public GameSystem<Collidable, T...> {
};

struct CollisionSystem : public GameSystem<Moveable, Collidable, PointTransform> {
    CollidableSystem<PointTransform>* collidables;

    void Initialize(GameWorld* world) {
        collidables = world->CreateSystem<CollidableSystem<PointTransform>>();
    }

    void ObjectAdded(GameObject* object) {
        object->GetComponent<Moveable>()->Moved.Bind(this, &CollisionSystem::Moved, object);
    }
    
    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Moveable>()->Moved.Unbind(this, &CollisionSystem::Moved, object);
    }

    void Moved(Moveable* movable, GameObject* object) {
        std::vector<Collidable::CollisionEvent> events;
       PointTransform* t = object->GetComponent<PointTransform>();
       for (auto other : collidables->Objects()) {
            if (other==object) continue;
            PointTransform* t2 = other->GetComponent<PointTransform>();
            if (t->x == t2->x && t->y == t2->y) {
                events.push_back({ object->GetComponent<Collidable>(), other->GetComponent<Collidable>(), object, other });
                events.push_back({ other->GetComponent<Collidable>(), object->GetComponent<Collidable>(), other, object });
            }
       }
       for (auto& e : events) {
            e.collidable->OnCollision(e);
       }
    }
};

struct Powerup {
    enum class PowerupType {
        Grow,
    };
    PowerupType type;
};

struct PowerupSystem : public GameSystem<Powerup, Collidable> {

    void Initialize(GameWorld* world) override {
    }

    void ObjectAdded(GameObject* object) override {
        object->GetComponent<Collidable>()->OnCollision.Bind(this, &PowerupSystem::Collision);
    }
    
    void ObjectRemoved(GameObject* object) override {
        object->GetComponent<Collidable>()->OnCollision.Unbind(this, &PowerupSystem::Collision);
    }

    void Collision(Collidable::CollisionEvent e) {
        auto powerupType = e.object->GetComponent<Powerup>()->type;
        switch (powerupType) {
            case Powerup::PowerupType::Grow: {
                Snake* snake = e.otherObject->GetComponent<Snake>();
                if (snake) {
                    float amount = 0.15f*5;
                    snake->tailLife += amount;
                    for(auto body : snake->bodies) {
                        body->GetComponent<TimedLife>()->time += amount;
                    }
                }
                e.object->Remove();
                break;
            }
        }
    }
};


struct SnakeCollisionSystem : public GameSystem<Snake, Collidable> {

    void ObjectAdded(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision.Bind(this, &SnakeCollisionSystem::Collision, object);
    }

    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision.Unbind(this, &SnakeCollisionSystem::Collision, object);
    }
    
    void Collision(Collidable::CollisionEvent e, GameObject* object) {
        if (!e.otherObject->GetComponent<Blockable>()) return;//was not a snake colllision
        Snake* snake = object->GetComponent<Snake>();
        float amount = -0.15f;
        snake->tailLife += amount;
        if (snake->tailLife<0) snake->tailLife = 0;
        snake->OnCollision();
        
        //for(auto body : snake->bodies) {
            //body->GetComponent<TimedLife>()->time += amount;
        //}
    }

};

struct BlockableCollisionSystem : public GameSystem<Moveable, Collidable, PointTransform> {

    void ObjectAdded(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision.Bind(this, &BlockableCollisionSystem::Collision, object);
    }

    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision.Unbind(this, &BlockableCollisionSystem::Collision, object);
    }
    
    void Collision(Collidable::CollisionEvent e, GameObject* object) {
        if (!e.otherObject->GetComponent<Blockable>()) return;
        Moveable* movable = object->GetComponent<Moveable>();
        PointTransform* transform = object->GetComponent<PointTransform>();
        movable->ModifyValues(transform->x, transform->y, transform->rotation + 2); // reverse direction
    }
};


struct Blinkable {
    
    Blinkable() : time(0) {}
    
    Colour originalColor;
    Colour blinkColor;
    float blinkSpeed;
    int blinkCount;
    float time;
};

struct BlinkSystem : public GameSystem<Mesh, Blinkable> {

    //void ObjectAdded(GameObject* object) {
        //auto& vertices = object->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        //object->GetComponent<Blinkable>()->originalColor = vertices[0].Color;
    //}
    
    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(object->GetComponent<Blinkable>()->originalColor);
    }

    void Update(float dt) {
        for(auto o : Objects()) {
            auto& vertices = o->GetComponent<Mesh>()->GetMesh<Vertex>();
            Blinkable* blinkable = o->GetComponent<Blinkable>();
            if (blinkable->time<blinkable->blinkSpeed) {
                blinkable->time += dt;
            } else {
                blinkable->blinkCount++;
                vertices.SetColor((blinkable->blinkCount & 1) ? blinkable->originalColor : blinkable->blinkColor );
            }
        }
    }
};


struct SnakeBlinkCollisionSystem : public GameSystem<Snake, Colorable> {
    
    GameWorld* world;
    
    void Initialize(GameWorld* world) {
        this->world = world;
    }
    
    void ObjectAdded(GameObject* object) {
        object->GetComponent<Snake>()->OnCollision.Bind(this, &SnakeBlinkCollisionSystem::Collision, object);
    }

    void ObjectRemove(GameObject* object) {
        object->GetComponent<Snake>()->OnCollision.Unbind(this, &SnakeBlinkCollisionSystem::Collision, object);
    }

    void Collision(GameObject* object) {
        Snake* snake = object->GetComponent<Snake>();
        Colorable* colorable = object->GetComponent<Colorable>();
        CreateBlinkEffect(object, colorable->Color);
        for(auto o : snake->bodies) {
            CreateBlinkEffect(o, colorable->Color);
        }
    }
    
    void CreateBlinkEffect(GameObject* object,Colour color) {
        GameObject* blinkEffect = world->CreateObject();
        blinkEffect->AddComponent<Mesh>(object);
        auto b = blinkEffect->AddComponent<Blinkable>();
        b->originalColor = color;
        b->blinkColor = Colour::White();
        b->blinkSpeed = 0.075f;
        blinkEffect->AddComponent<TimedLife>()->time = b->blinkSpeed * 2;
    }

};

/*
struct SnakeCreatorSystem : public GameSystem<Snake> {

    void SetBigButtons(BigButtonManager& bigButtons) {
        bigButtons.ButtonDown.Bind(this, &SnakeCreatorSystem::BigButtonDown);
    }

    void BigButtonDown(BigButtonEvent e) {
        if (DoesPlayerIndexExist(e.PlayerIndex)) return;
        CreateSnake(e.PlayerIndex, 0, 0);
    }
    
    void Initialize(GameWorld* world) {
        this->world = world;
    }

private:
    GameWorld* world;
    bool DoesPlayerIndexExist(int playerIndex) {
        for(auto o : Objects()) {
            if (o->GetComponent<Snake>()->index == playerIndex) return true;
        }
        return false;
    }
public:
    
};
*/

struct SnakeFactory : GameConcept {

    GameWorld* world;
    void Initialize(GameWorld* world) {
        this->world = world;
    }
    
    GameObject* CreatePlayer(int index) {
        GameObject* player = world->CreateObject();
        player->AddComponent<Player>()->index = index;
        
        static std::vector<Colour> snakeColors {
            Colour::Green(),
            Colour::Red(),
            Colour::Blue(),
            Colour::Yellow()
        };
        player->AddComponent<Colorable>()->Color = snakeColors[index];
        return player;
    }

    GameObject* CreateSnake(int x, int y, GameObject* player = 0) {
        GameObject* snake = world->CreateObject();
        snake->AddComponent<Transform>();
        snake->AddComponent<PointTransform>()->SetPosition(x, y);
        snake->AddComponent<Snake>()->tailLife = 0.6f;
        snake->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1, Colour::White());
        snake->AddComponent<Material>();
        snake->AddComponent<Moveable>()->timeBetweenMoves = 0.15f;
        snake->AddComponent<Collidable>();
        snake->AddComponent<Blockable>();
        
        if (player) {
            snake->AddComponent<Player>(player);
            snake->AddComponent<Colorable>(player);
        }
        
        return snake;
    }
    
    GameObject* CreateBlock(int x, int y) {
        GameObject* object = world->CreateObject();
        object->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1, Colour::White());
        object->AddComponent<Transform>();
        object->AddComponent<Material>();
        object->AddComponent<PointTransform>()->x = x;
        object->GetComponent<PointTransform>()->y = y;
        object->AddComponent<Collidable>();
        object->AddComponent<Blockable>();
        return object;
    }
    
    GameObject* CreatePowerup(int x, int y) {
        GameObject* powerup = world->CreateObject();
        powerup->AddComponent<Transform>();
        powerup->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 0.8f, Colour::Yellow());
        powerup->AddComponent<Material>();
        powerup->AddComponent<PointTransform>()->SetPosition(x,y);
        powerup->AddComponent<Powerup>();
        powerup->AddComponent<Collidable>();
        return powerup;
    }
    
    void AddBlockRectangle(int x, int y, int width, int height) {
        for (int i=0; i<=width; i++) {
            CreateBlock(x + i, y);
        }

        for (int i=0; i<=width; i++) {
            CreateBlock(x + i, y+height);
        }
        
        for (int i=1; i<=height-1; i++) {
            CreateBlock(x, y+i);
        }
        
        for (int i=1; i<=height-1; i++) {
            CreateBlock(x+width, y+i);
        }
    }


};

struct SnakeCollectPowerupScoreSystem : public GameSystem<Snake, Player, Collidable> {

    void ObjectAdded(GameObject* object) override {
        object->GetComponent<Collidable>()->OnCollision.Bind(this, &SnakeCollectPowerupScoreSystem::Collision);
    }
    
    void ObjectRemoved(GameObject* object) override {
        object->GetComponent<Collidable>()->OnCollision.Unbind(this, &SnakeCollectPowerupScoreSystem::Collision);
    }

    void Collision(Collidable::CollisionEvent e) {
        auto powerup = (e.otherObject->GetComponent<Powerup>());
        if (!powerup) return;
        if (powerup->type!=Powerup::PowerupType::Grow) return;
        e.object->GetComponent<Player>()->Score++;
    }
};




struct BlockableSystem : public GameSystem<Blockable> { };

struct PlayerScoreLabelSystem : public GameSystem<Label, Player> {
    
    void ObjectAdded(GameObject* object) {
        object->GetComponent<Player>()->Score.Changed.Bind(this, &PlayerScoreLabelSystem::ScoreChanged, object);
    }

    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Player>()->Score.Changed.Unbind(this, &PlayerScoreLabelSystem::ScoreChanged, object);
    }

    void ScoreChanged(GameObject* object) {
        std::stringstream s;
        s<<object->GetComponent<Player>()->Score();
        object->GetComponent<Label>()->Text = s.str();
    }
};


class Game : public GameState<Game> {
public:
    GameWorld world;
    RenderSystem* renderer;
    GameObject* camera;
    BigButtonManager bigButtons;
    SnakeFactory* factory;
    GameWorld guiWorld;
    Gui* gui;
    
    GameObject* scoreLabels[4];
    bool playing;
    
    void Initialize() {
        playing = false;
    
        bigButtons.Initialize();
    
        //world.CreateSystem<SnakeCreatorSystem>()->SetBigButtons(bigButtons);
        factory = world.CreateSystem<SnakeFactory>();
        renderer = world.CreateSystem<RenderSystem>();
        SnakeControlSystem* snakeControlSystem = world.CreateSystem<SnakeControlSystem>();
        snakeControlSystem->SetInput(Input);
        snakeControlSystem->SetBigButtons(bigButtons);
        world.CreateSystem<PowerupSystem>();
        world.CreateSystem<MoveSystem>();
        world.CreateSystem<BlockableCollisionSystem>();
        world.CreateSystem<SnakeCollisionSystem>();
        world.CreateSystem<CollisionSystem>();
        world.CreateSystem<SnakeSystem>();
        
        world.CreateSystem<TimedLifeSystem>();
        world.CreateSystem<SnakeBodyRemovalSystem>();
        world.CreateSystem<PointTransformSystem>();
        world.CreateSystem<BlockableSystem>();
        world.CreateSystem<BlinkSystem>();
        world.CreateSystem<SnakeBlinkCollisionSystem>();
        world.CreateSystem<ColorSystem>();
        world.CreateSystem<SnakeCollectPowerupScoreSystem>();
        
        
        guiWorld.CreateSystem<PlayerScoreLabelSystem>();
        gui = guiWorld.CreateSystem<Gui>();
        gui->Setup("images.png", "images.xml", Context().Viewport(), Input);
        gui->GetAtlas()->GetComponent<TextureComponent>()->Texture().DisableMipmapping();
        gui->CreateFont("font.fnt", "font");
        gui->CreateLabel(0, {0, Context().ScreenSize().y-100}, {Context().ScreenSize().x,100}, 0, "SNAKE", 32)->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        
        scoreLabels[0]=CreateLabel(0, 200);
        scoreLabels[1]=CreateLabel({Context().ScreenSize().x-200,0}, 200);
        scoreLabels[2]=CreateLabel({Context().ScreenSize().x-200,Context().ScreenSize().y-200}, 200);
        scoreLabels[3]=CreateLabel({0,Context().ScreenSize().y-200}, 200);
        
        Input.ButtonDown.Bind(this, &Game::ButtonDown);
    }
    
    GameObject* CreateLabel(Vector2 pos, Vector2 size) {
        auto l =gui->CreateLabel(0, pos, size, 0, "0", 80);
        l->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        l->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        //l->RemoveComponent<Colorable>();
        return l;
    }
    
    void ClearGame() {
        world.Clear();
        for (int i=0; i<4; ++i) {
            scoreLabels[i]->RemoveComponent<Player>();
            scoreLabels[i]->GetComponent<Label>()->Text = "0";
        }
    }
    
    void CreateNewGame() {
    
        camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = { 0, 0, 100 };
        camera->AddComponent<Camera>()->FieldOfView = 25;
        
        GameObject* players[4];
        
        for(int i=0; i<4; i++) {
            players[i] = factory->CreatePlayer(i);
            scoreLabels[i]->AddComponent<Player>(players[i]);
            scoreLabels[i]->GetComponent<Colorable>()->Color = players[i]->GetComponent<Colorable>()->Color;
        }
        
        factory->CreateSnake(0,0, players[0]);
        factory->CreateSnake(0,10, players[1]);
        
        
        factory->AddBlockRectangle(-20, -20, 40, 40);
        for (int i=0; i<40; ++i) {
            factory->CreatePowerup(i*5, i*5);
        }
    }
    
    void ButtonDown(std::string button) {
        if (button == "q") exit(0);
        if (button == " ") {
            playing = !playing;
            if (playing) {
                CreateNewGame();
            } else {
                ClearGame();
            }
        }
    }
    
    void Update(float dt) {
        bigButtons.Update(dt);
        world.Update(dt);
        guiWorld.Update(dt);
    }
    
    void Render() {
        world.Render();
        guiWorld.Render();
    }
};

int main() {
    Engine e;
    //e.Start<Game>(1920,1080, true);
	e.Start<Game>();
    return 0;
}