#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "BigButtonManager.hpp"
#include "Mesh.hpp"
#include "VertexMesh.hpp"
#include "Gui.hpp"
#include "ColorSystem.hpp"
#include "SoundSystem.hpp"
#include "TransformAnimatorSystem.hpp"
#include "Point.hpp"
#include "Timer.hpp"

using namespace Pocket;

struct Player {
    Player() { Score = 0; }
    int index;
    Property<int> Score;
    std::string name;
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
    GameObject* snake;
};

struct SnakeSystem : public GameSystem<Snake, Moveable, PointTransform> {

    GameWorld* world;
    void Initialize(GameWorld* world) {
        this->world = world;
    }
    
    std::function<void()> OneSnakeLeft;

    void Update(float dt) {
        for(auto o : Objects()) {
            UpdateSnake(o);
        }
        if (Objects().size()<=1 && OneSnakeLeft) {
            OneSnakeLeft();
            OneSnakeLeft = 0;
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
        object->AddComponent<SnakeBody>()->snake = snakeObject;
        object->AddComponent<Blockable>();
        object->AddComponent<Collidable>();
        snake->bodies.push_back(object);
    }
};

struct SnakeBodyRemovalSystem : public GameSystem<SnakeBody> {
    void ObjectRemoved(GameObject* object) {
        Snake* snakeObject = object->GetComponent<SnakeBody>()->snake->GetComponent<Snake>();
        if (!snakeObject) return;
        auto& list = snakeObject->bodies;
        list.erase(std::find(list.begin(), list.end(), object));
    }
};

struct SnakeControlSystem : public GameSystem<Player, Snake, PointTransform> {

    void SetInput(InputManager& input) {
        input.ButtonDown.Bind(this, &SnakeControlSystem::ButtonDown);
    }
    
    void RemoveInput(InputManager& input) {
        input.ButtonDown.Unbind(this, &SnakeControlSystem::ButtonDown);
    }

    void SetBigButtons(BigButtonManager& bigButtons) {
        bigButtons.ButtonDown.Bind(this, &SnakeControlSystem::BigButtonDown);
    }
    
    void RemoveBigButtons(BigButtonManager& bigButtons) {
        bigButtons.ButtonDown.Unbind(this, &SnakeControlSystem::BigButtonDown);
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
            
                Snake* snake = o->GetComponent<Snake>();
                if (snake->bodies.empty()) {
                    o->GetComponent<PointTransform>()->SetRotation(rotation);
                } else {
                    PointTransform* bodyTransform = snake->bodies[snake->bodies.size()-1]->GetComponent<PointTransform>();
                    if (!bodyTransform->IsRotationOpposite(rotation, bodyTransform->rotation)) {
                        o->GetComponent<PointTransform>()->SetRotation(rotation);
                    } else {
                        std::cout<< "Rotation blocked"<<std::endl;
                    }
                }
            }
        }
    }

    void ButtonDown(std::string button) {
        if (button == "a") {
            SetRotation(0, 2);
        } else if (button == "d") {
            SetRotation(0, 0);
        } else if (button == "w") {
            SetRotation(0, 3);
        } else if (button == "s") {
            SetRotation(0, 1);
        }
        
        
        if (button == "j") {
            SetRotation(1, 2);
        } else if (button == "l") {
            SetRotation(1, 0);
        } else if (button == "i") {
            SetRotation(1, 3);
        } else if (button == "k") {
            SetRotation(1, 1);
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
    
    
    GameObject* CreateSnake(int x, int y, int direction, GameObject* player = 0) {
        GameObject* snake = world->CreateObject();
        snake->AddComponent<Transform>();
        snake->AddComponent<PointTransform>()->SetPosition(x, y);
        snake->GetComponent<PointTransform>()->rotation = direction;
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


//-------- SCORE SYSTEMS --------
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

struct SnakeInstantDeathSystem : public GameSystem<Snake> {
     void ObjectAdded(GameObject* object) override {
        object->GetComponent<Snake>()->OnCollision.Bind(this, &SnakeInstantDeathSystem::Collision, object);
    }
    
    void ObjectRemoved(GameObject* object) override {
        object->GetComponent<Snake>()->OnCollision.Unbind(this, &SnakeInstantDeathSystem::Collision, object);
    }

    void Collision(GameObject* object) {
        auto& list = object->GetComponent<Snake>()->bodies;
        for (auto b : list) {
            b->Remove();
        }
        object->Remove();
    }
};


struct SnakeDeathPointSystem : public GameSystem<Snake, Collidable> {

    void ObjectAdded(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision.Bind(this, &SnakeDeathPointSystem::Collision, object);
    }

    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision.Unbind(this, &SnakeDeathPointSystem::Collision, object);
    }
    
    void Collision(Collidable::CollisionEvent e, GameObject* object) {
        if (!e.otherObject->GetComponent<Blockable>()) return;//was not a snake colllision
        Snake* otherSnake = e.otherObject->GetComponent<Snake>();
        SnakeBody* otherSnakeBody = e.otherObject->GetComponent<SnakeBody>();
        Player* player = 0;
        Player* snakePlayer = object->GetComponent<Player>();
        
        if (otherSnake) {
            player = e.otherObject->GetComponent<Player>();
        } else if (otherSnakeBody) {
            player = otherSnakeBody->snake->GetComponent<Player>();
        }
        if (player && player!=snakePlayer) {
            player->Score++;
        } else if (snakePlayer) { // suicide
            if (snakePlayer->Score>0) {
                snakePlayer->Score--;
            }
        }
    }
};

struct TextEffect {
    float delay;
    std::string text;
};


struct TextEffectFactory : public GameConcept {

    GameWorld* world;
    
    Gui* gui;

    TransformAnimation scaleUp;
    
    struct TimedLifeLabels : public GameSystem<TimedLife, Label> {
    
    };
    
    TimedLifeLabels* timedLifeLabels;
    
    void Initialize(GameWorld* world) {
        this->world = world;
        world->CreateSystem<TransformAnimatorSystem>();
        timedLifeLabels = world->CreateSystem<TimedLifeLabels>();
        
        gui = world->CreateSystem<Gui>();
        
        scaleUp.scale.Add(0, 0);
        scaleUp.scale.Add(0.5f, 1.0f);
        //scaleUp.rotation.Add(0, {0,0,2.0f});
        //scaleUp.rotation.Add(0.5f, {0,0,0});
        
    }
    
    GameObject* CreateTextEffect(Vector2 center, std::string text, float fontSize, float time = 0.5f) {
        GameObject* effect = gui->CreateLabel(0, center, 300, 0, text, fontSize);
        effect->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        effect->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        effect->GetComponent<Transform>()->Anchor = {150,150,0};
        effect->AddComponent<TimedLife>()->time = time;
        effect->AddComponent<TransformAnimator>()->Play(&scaleUp);
        return effect;
    }
    
    void ClearAll() {
        for(auto o : timedLifeLabels->Objects()) {
            o->Remove();
        }
    }
    
    
};

struct TimedEvent {
    float time;
    std::function<void()> event;
};

struct TimedEventSystem : public GameSystem<TimedEvent> {
    void Update(float dt) {
        for (auto o : Objects()) {
            auto timedLife = o->GetComponent<TimedEvent>();
            if (timedLife->time>0) {
                timedLife->time -= dt;
            } else {
                timedLife->event();
                o->Remove();
            }
        }
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
    
    TextEffectFactory* textEffect;
    
    GameObject* scoreLabels[4];
    GameObject* title;
    GameObject* activePlayerLabels[4];
    GameObject* numberOfRounds;
    
    struct GameSettings {
        GameSettings() { maxRounds = 1;
            for (int i=0; i<4; i++) {
                activePlayers[i]=false;
            }
        }
        int maxRounds;
        
        bool activePlayers[4];
        
        int ActivePlayers() {
            int count =0;
            for(int i=0; i<4; i++) {
                if (activePlayers[i]) count++;
            }
            return count;
        }
        
    };
    
    struct GameData {
        GameData() : currentRound(0) { }
        GameObject* players[4];
        int currentRound;
        GameObject* currentRoundWinner;
    };
    
    GameSettings settings;
    GameData gameData;
    
    enum class State {
        MainMenu,
        StartNewGame,
        StartRound,
        PlayingRound,
        RoundEnded,
        GameEnded
    };
    
    State currentState;
    
    SnakeSystem* snakeSystem;
    
    GameWorld musicWorld;
    
    Colour GetPlayerColor(int index) {
    static std::vector<Colour> snakeColors {
            Colour::Green(),
            Colour::Red(),
            Colour::Blue(),
            Colour::Yellow()
        };
        return snakeColors[index];
    }
    
    std::string GetPlayerName(int index) {
        static std::vector<std::string> snakeNames = {
            "Green",
            "Red",
            "Blue",
            "Yellow"
        };
        return snakeNames[index];
    }

    bool isGameStarting;
    
    void Initialize() {
    
        isGameStarting = false;
        
        musicWorld.CreateSystem<SoundSystem>();
        
        /*GameObject* music = musicWorld.CreateObject();
        music->AddComponent<Transform>();
        music->AddComponent<SoundEmitter>();
        music->AddComponent<Sound>()->LoadFromWav("Music.wav");
        music->GetComponent<SoundEmitter>()->Looping = true;
        
        GameObject* soundListener = musicWorld.CreateObject();
        soundListener->AddComponent<Transform>();
        soundListener->AddComponent<SoundListener>();
        
        music->GetComponent<SoundEmitter>()->Emit();
        */
    
        currentState = State::MainMenu;
    
        bigButtons.Initialize();
    
        //world.CreateSystem<SnakeCreatorSystem>()->SetBigButtons(bigButtons);
        
        
        
        guiWorld.CreateSystem<PlayerScoreLabelSystem>();
        guiWorld.CreateSystem<TimedLifeSystem>();
        guiWorld.CreateSystem<TimedEventSystem>();
        gui = guiWorld.CreateSystem<Gui>();
        textEffect = guiWorld.CreateSystem<TextEffectFactory>();
        gui->Setup("images.png", "images.xml", Context().Viewport(), Input);
        gui->GetAtlas()->GetComponent<TextureComponent>()->Texture().DisableMipmapping();
        gui->CreateFont("font.fnt", "font");
        
        
        CreateMainMenu();
        
        Input.ButtonDown.Bind(this, &Game::ButtonDown);
    }
    
    GameObject* CreateLabel(Vector2 pos, Vector2 size) {
        auto l =gui->CreateLabel(0, pos, size, 0, "0", 80);
        l->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        l->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        //l->RemoveComponent<Colorable>();
        return l;
    }
    
    GameObject* CreatePlayer(int index) {
        GameObject* player = guiWorld.CreateObject();
        player->AddComponent<Player>()->index = index;
        
        player->AddComponent<Colorable>()->Color = GetPlayerColor(index);
        player->GetComponent<Player>()->name = GetPlayerName(index);
        return player;
    }

    
    void CreateMainMenu() {
        title = gui->CreateLabel(0, {0, Context().ScreenSize().y-300}, {Context().ScreenSize().x,100}, 0, "SNAKE", 180);
        title->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        
        GameObject* activePlayers = gui->CreateLabel(title, {0, -200}, {Context().ScreenSize().x,100}, 0, "Active players:", 30);
        activePlayers->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        
        GameObject* maxRounds = gui->CreateLabel(title, {0, -320}, {Context().ScreenSize().x,100}, 0, "Rounds:", 30);
        maxRounds->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        
        
        Vector2 size = {Context().ScreenSize().x / 4,100};
        
        Vector2 pos = {0,Context().ScreenSize().y/2-200};
        for(int i=0; i<4; i++) {
            activePlayerLabels[i]=gui->CreateLabel(0, pos, size, 0, GetPlayerName(i), 40);
            activePlayerLabels[i]->GetComponent<Colorable>()->Color = Colour::White(0.3f);// GetPlayerColor(i);
            activePlayerLabels[i]->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
            pos.x+=size.x;
        }
        
        std::stringstream s;
        s<<settings.maxRounds;
        numberOfRounds = gui->CreateLabel(0, {0, Context().ScreenSize().y/2-300}, {Context().ScreenSize().x,100}, 0, s.str(), 40);
        numberOfRounds->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        
        
        bigButtons.ButtonDown.Bind(this, &Game::MainMenuButtonDown);
    }
    
    void MainMenuButtonDown(BigButtonEvent e) {
        if (e.Button == BigButtonButton::Back) {
            settings.maxRounds++;
            if (settings.maxRounds>8) {
                settings.maxRounds = 1;
            }
            std::stringstream s;
            s<<settings.maxRounds;
            numberOfRounds->GetComponent<Label>()->Text = s.str();
        } else if (e.Button == BigButtonButton::Start) {
            TryStartGame();
        } else {
            activePlayerLabels[e.PlayerIndex]->GetComponent<Colorable>()->Color = GetPlayerColor(e.PlayerIndex);
            settings.activePlayers[e.PlayerIndex] = true;
        }
    }
    
    void RemoveMainMenu() {
        title->Remove();
        for(int i=0; i<4; i++) {
            activePlayerLabels[i]->Remove();
        }
        numberOfRounds->Remove();
        bigButtons.ButtonDown.Unbind(this, &Game::MainMenuButtonDown);
    }
    
    void CreateNewGame() {
        
        gameData = {};
        
        Vector2 labelPositions[4] = {
            0,
            {Context().ScreenSize().x-200,0},
            {Context().ScreenSize().x-200,Context().ScreenSize().y-200},
            {0,Context().ScreenSize().y-200}
        };
        
        /*
        scoreLabels[0]=CreateLabel(0, 200);
        scoreLabels[1]=CreateLabel({Context().ScreenSize().x-200,0}, 200);
        scoreLabels[2]=CreateLabel({Context().ScreenSize().x-200,Context().ScreenSize().y-200}, 200);
        scoreLabels[3]=CreateLabel({0,Context().ScreenSize().y-200}, 200);
        */
    
        for(int i=0; i<4; i++) {
            gameData.players[i] = CreatePlayer(i);
            
            if (settings.activePlayers[i]) {
                scoreLabels[i]=CreateLabel(labelPositions[i], 200);
                scoreLabels[i]->AddComponent<Player>(gameData.players[i]);
                scoreLabels[i]->GetComponent<Colorable>()->Color = gameData.players[i]->GetComponent<Colorable>()->Color;
            } else {
                scoreLabels[i]=0;
            }
        }
    }
    
    void ClearGame() {
    
        for(int i=0; i<4; i++) {
            if (scoreLabels[i]) {
                scoreLabels[i]->Remove();
            }
            gameData.players[i]->Remove();
            settings.activePlayers[i]=false;
        }
        
        ClearBoard();
    }
    
    void ClearBoard() {
    
        SnakeControlSystem* snakeControlSystem = world.CreateSystem<SnakeControlSystem>();
        snakeControlSystem->RemoveInput(Input);
        snakeControlSystem->RemoveBigButtons(bigButtons);
        
        world = {};
    }
    
    void CreateNewBoard() {
        
        
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
        snakeSystem = world.CreateSystem<SnakeSystem>();
        
        world.CreateSystem<TimedLifeSystem>();
        world.CreateSystem<SnakeBodyRemovalSystem>();
        world.CreateSystem<PointTransformSystem>();
        world.CreateSystem<BlockableSystem>();
        world.CreateSystem<BlinkSystem>();
        world.CreateSystem<SnakeBlinkCollisionSystem>();
        world.CreateSystem<ColorSystem>();
        
        bool collectPowerups = false;
        
        if (collectPowerups) {
            world.CreateSystem<SnakeCollectPowerupScoreSystem>();
        } else {
            world.CreateSystem<SnakeInstantDeathSystem>();
            world.CreateSystem<SnakeDeathPointSystem>();
        }
        
        camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = { 0, 0, 100 };
        camera->AddComponent<Camera>()->FieldOfView = 25;
        
        
        
        
        
        factory->AddBlockRectangle(-20, -20, 40, 40);
        for (int i=0; i<40; ++i) {
            factory->CreatePowerup(-19 + MathHelper::Random(40), -19 + MathHelper::Random(40));
        }
        
        
    }
    
    void CreateSnakes() {
    
        Point positions[4] = {
            {-10,-10},
            {10,-10},
            {10,10},
            {-10,10}
        };
        int dirs[4] = {0,2,2,2};
        
        for(int i=0; i<4; i++) {
            if (settings.activePlayers[i]) {
                factory->CreateSnake(positions[i].x, positions[i].y, dirs[i], gameData.players[i]);
            }
        }
    }
    
    void TryStartGame() {
        if (settings.ActivePlayers()<=1) {
            textEffect->CreateTextEffect(Context().ScreenSize*0.5f, "Too few players", 60);
            return;
        }
        if (isGameStarting) return;
        isGameStarting = true;
        CreateTimedEvent(0, [this]{
            isGameStarting = false;
            SetState(State::StartNewGame);
        });
    }
    
    void ButtonDown(std::string button) {
        if (button == "q") exit(0);
        if (button == " ") {
            if (currentState == State::MainMenu) {
                TryStartGame();
            } else {// if (currentState == State::PlayingRound){
                SetState(State::MainMenu);
            }
        }
    }
    
    void CreateTimedEvent(float time, std::function<void()> event) {
        GameObject* o = guiWorld.CreateObject();
        o->AddComponent<TimedEvent>()->time = time;
        o->GetComponent<TimedEvent>()->event = event;
    }
    
    void SetState(State state) {
        currentState = state;
    
        switch (state) {
            case State::MainMenu:
            {
                ClearGame();
                CreateMainMenu();
                textEffect->ClearAll();
                break;
            }
            case State::StartNewGame:
            {
                RemoveMainMenu();
                CreateNewGame();
                CreateTimedEvent(0, [this] {
                    SetState(State::StartRound);
                });
             
                break;
            }
            case State::StartRound:
            {
                ClearBoard();
                CreateNewBoard();
                gameData.currentRoundWinner = 0;
                std::stringstream s;
                s<<"Round ";
                s<<(gameData.currentRound + 1);
                s<<"/";
                s<<settings.maxRounds;
                textEffect->CreateTextEffect(Context().ScreenSize * 0.5f, s.str(), 130, 1.0f);
                
                for (int i=0; i<5; i++) {
                CreateTimedEvent(1.0f + i*0.5f, [this, i]{
                    int counter = 5 - i;
                    std::stringstream s;
                    s<<counter;
                    textEffect->CreateTextEffect(Context().ScreenSize * 0.5f, s.str(), 200);
                });
                }
                CreateTimedEvent(1.0f + 5 * 0.5f, [this] {
                    textEffect->CreateTextEffect(Context().ScreenSize * 0.5f, "Go!", 200);
                });
                
                CreateTimedEvent(1.0f + 5 * 0.5f + 0.5f, [this] {
                    SetState(State::PlayingRound);
                });
                break;
            }
            case State::PlayingRound:
            {
                CreateSnakes();
                
                snakeSystem->OneSnakeLeft = [this]{
                    for(auto o : snakeSystem->Objects()) {
                        o->RemoveComponent<Moveable>();
                    }
                    if (!snakeSystem->Objects().empty()) {
                        gameData.currentRoundWinner = snakeSystem->Objects()[0];
                    }
                    
                    SetState(State::RoundEnded);
                };
                
                break;
            }
            case State::RoundEnded:
            {
                
                if (gameData.currentRoundWinner) {
                    textEffect->CreateTextEffect(Context().ScreenSize * 0.5f+Vector2(0,250), "Round Winner:", 70, 5.0f);
                    GameObject* winnerText = textEffect->CreateTextEffect(Context().ScreenSize * 0.5f + Vector2(0,-50), gameData.currentRoundWinner->GetComponent<Player>()->name, 150, 5.0f);
                    winnerText->GetComponent<Colorable>()->Color = gameData.currentRoundWinner->GetComponent<Colorable>()->Color;
                    
                    CreateTimedEvent(2.0f, [this]{
                        GameObject* points = textEffect->CreateTextEffect(Context().ScreenSize * 0.5f + Vector2(0,-250), "+2", 150, 0.75f);
                        points->GetComponent<Colorable>()->Color = gameData.currentRoundWinner->GetComponent<Colorable>()->Color;
                        CreateTimedEvent(0.60f, [this] {
                            gameData.currentRoundWinner->GetComponent<Player>()->Score+=2;
                        });
                    });
                } else {
                    textEffect->CreateTextEffect(Context().ScreenSize * 0.5f+Vector2(0,250), "Round Draw", 70, 5.0f);
                }
                
                gameData.currentRound++;
                CreateTimedEvent(5.0f, [this] {
                    SetState(gameData.currentRound>=settings.maxRounds ? State::GameEnded : State::StartRound);
                });
                
                
                //CreateTimedEvent(1.0f + 5 * 0.5f, [this] {
                //    SetState(State::PlayingRound);
                //});
                break;
            }
            case State::GameEnded:
            {
                ClearBoard();
                std::vector<GameObject*> sortedPlayers;
                for(int i=0;i<4;i++) {
                    sortedPlayers.push_back(gameData.players[i]);
                }
                std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](auto a, auto b){
                    return a->template GetComponent<Player>()->Score()>b->template GetComponent<Player>()->Score();
                });
                GameObject* best = sortedPlayers[0];
                std::vector<GameObject*> winners;
                winners.push_back(best);
                for(int i=1; i<4; i++) {
                    if (sortedPlayers[i]->GetComponent<Player>()->Score()==best->GetComponent<Player>()->Score()) {
                        winners.push_back(sortedPlayers[i]);
                    }
                }
                
                textEffect->CreateTextEffect(Context().ScreenSize * 0.5f+Vector2(0,350), "Final result:", 50, 50.0f);
                textEffect->CreateTextEffect(Context().ScreenSize * 0.5f+Vector2(0,250), winners.size() == 1 ? "Winner:" : "Draw between:", 70, 50.0f);
                
                int c = 0;
                for(auto winner : winners) {
                    auto e = textEffect->CreateTextEffect(Context().ScreenSize * 0.5f+Vector2(0,0 - c * 100), winner->GetComponent<Player>()->name, 70, 50.0f);
                    e->GetComponent<Colorable>()->Color = winner->GetComponent<Colorable>()->Color;
                    c++;
                }
                
                CreateTimedEvent(8.0f, [this]{
                    SetState(State::MainMenu);
                });
                break;
            }
        default:
        break;
        }
    }
    
    void Update(float dt) {
        bigButtons.Update(dt);
        world.Update(dt);
        guiWorld.Update(dt);
        musicWorld.Update(dt);
    }
    
    void Render() {
        world.Render();
        guiWorld.Render();
    }
};

int main() {

    Timer timer;

    struct Component1 {
        int x;
    };
    
    struct ComponentSystem : public GameSystem<Component1> {
    
    };
    
    
    timer.Begin();
     for(int i = 0; i<100000; ++i) {
            GameWorld wworld;
            for(int j = 0; j<10; ++j) {
                wworld.CreateObject();
            }
        }
        double time = timer.End();
        std::cout << time<<std::endl;
    

    GameWorld world;
    world.CreateSystem<ComponentSystem>();
    
    std::vector<GameObject*> objects;
    for (int i=0; i<1000000; i++) {
        objects.push_back(world.CreateObject());
        objects[i]->AddComponent<Component1>();
    }
    
    timer.Begin();
    for(int i = 0; i<objects.size(); i++) {
        objects[i]->GetComponent<Component1>()->x++;
    }
    time = timer.End();
    std::cout << time<<std::endl;
    
    
return 0;

    Engine e;
    e.Start<Game>(1920,1080, true);
	//e.Start<Game>();
    return 0;
}