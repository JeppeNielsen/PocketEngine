#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "BigButtonManager.hpp"

using namespace Pocket;


Component(TimedLife)
    float time;
};

SYSTEM(TimedLifeSystem, TimedLife)
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

Component(PointTransform)
    void Reset() { x=0; y=0; rotation=0; }
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

Component(Moveable)
    void Reset() { timer = 0; timeBetweenMoves = 0.4f; }
    float timeBetweenMoves;
    float timer;
    Event<Moveable*> Moved;
};

SYSTEM(MoveSystem, PointTransform, Moveable)
    struct dir {
        int dx; int dy;
    };

    void Update(float dt) {
        const dir directions[4] { { 1,0}, {0,-1}, {-1,0}, {0,1}};
        for (auto o : Objects()) {
            auto movable = o->GetComponent<Moveable>();
            auto transform = o->GetComponent<PointTransform>();
            if (movable->timer>movable->timeBetweenMoves) {
                movable->timer = 0;
                int direction = transform->rotation % 4;
                transform->x += directions[direction].dx;
                transform->y += directions[direction].dy;
                movable->Moved(movable);
            } else {
                movable->timer += dt;
            }
        }
    }
};

SYSTEM(PointTransformSystem, PointTransform, Transform)
    void Update(float dt) {
        for (auto o : Objects()) {
            auto p = o->GetComponent<PointTransform>();
            o->GetComponent<Transform>()->Position = Vector2(p->x, p->y);
        }
    }
};

Component(Snake)
    int index;
    float tailLife;
};

Component(SnakeBody)
    Pointer<Snake> snake;
};

SYSTEM(SnakeSystem, Snake, Moveable, PointTransform)
    void ObjectAdded(GameObject* object) {
        object->GetComponent<Moveable>()->Moved += event_handler(this, &SnakeSystem::Moved, object);
    }
    
    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Moveable>()->Moved -= event_handler(this, &SnakeSystem::Moved, object);
    }

    void Moved(Moveable* movable, GameObject* snakeObject) {
        Snake* snake = snakeObject->GetComponent<Snake>();
        GameObject* object = World()->CreateObject();
        object->AddComponent<Transform>();
        object->CloneComponent<PointTransform>(snakeObject);
        object->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1.0f, snakeObject->GetComponent<Mesh>()->GetMesh<Vertex>().vertices[0].Color);
        object->AddComponent<Material>();
        object->AddComponent<TimedLife>()->time = snakeObject->GetComponent<Snake>()->tailLife;
        object->AddComponent<SnakeBody>()->snake = snake;
    }
};

SYSTEM(SnakeControlSystem, Snake, PointTransform)



    void SetInput(InputManager& input) {
        input.ButtonDown += event_handler(this, &SnakeControlSystem::ButtonDown);
    }

    void SetBigButtons(BigButtonManager& bigButtons) {
        bigButtons.ButtonDown += event_handler(this, &SnakeControlSystem::BigButtonDown);
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
            if (o->GetComponent<Snake>()->index == index) {
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

Component(Collidable)
    struct CollisionEvent {
        Collidable* collidable;
        Collidable* other;
        GameObject* object;
        GameObject* otherObject;
    };

    Event<CollisionEvent> OnCollision;
};

SYSTEM(CollidableSystem, Collidable)

};

SYSTEM(CollisionSystem, Moveable, Collidable, PointTransform)
    CollidableSystem* collidables;

    void AddedToWorld(GameWorld& world) {
        collidables = world.CreateSystem<CollidableSystem>();
        collidables->AddComponent<PointTransform>();
    }

    void ObjectAdded(GameObject* object) {
        object->GetComponent<Moveable>()->Moved += event_handler(this, &CollisionSystem::Moved, object);
    }
    
    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Moveable>()->Moved -= event_handler(this, &CollisionSystem::Moved, object);
    }

    void Moved(Moveable* movable, GameObject* object) {
       PointTransform* t = object->GetComponent<PointTransform>();
       for (auto other : collidables->Objects()) {
            if (other==object) continue;
            PointTransform* t2 = other->GetComponent<PointTransform>();
            if (t->x == t2->x && t->y == t2->y) {
                object->GetComponent<Collidable>()->OnCollision( { object->GetComponent<Collidable>(), other->GetComponent<Collidable>(), object, other });
                other->GetComponent<Collidable>()->OnCollision( { other->GetComponent<Collidable>(), object->GetComponent<Collidable>(), other, object });
            }
       }
    }
};

Component(Powerup)
    enum class PowerupType {
        Grow,
    };

    PowerupType type;
};

SYSTEM(BodySystem, SnakeBody)
};

SYSTEM(PowerupSystem, Powerup, Collidable)

    BodySystem* bodies;

    void AddedToWorld(GameWorld& world) {
        bodies = world.CreateSystem<BodySystem>();
        bodies->AddComponent<TimedLife>();
    }

    void ObjectAdded(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision += event_handler(this, &PowerupSystem::Collision);
    }
    
    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision -= event_handler(this, &PowerupSystem::Collision);
    }

    void Collision(Collidable::CollisionEvent e) {
        auto powerupType = e.object->GetComponent<Powerup>()->type;
        switch (powerupType) {
            case Powerup::PowerupType::Grow: {
                Snake* snake = e.otherObject->GetComponent<Snake>();
                if (snake) {
                    float amount = 0.5f;
                    snake->tailLife += amount;
                    for(auto body : bodies->Objects()) {
                        body->GetComponent<TimedLife>()->time += amount;
                    }
                }
                e.object->Remove();
                break;
            }
        }
    }
};

SYSTEM(SnakeCreatorSystem, Snake)

    std::vector<Colour> snakeColors {
        Colour::Green(),
        Colour::Red(),
        Colour::Blue(),
        Colour::Yellow()
    };

    void SetBigButtons(BigButtonManager& bigButtons) {
        bigButtons.ButtonDown += event_handler(this, &SnakeCreatorSystem::BigButtonDown);
    }

    void BigButtonDown(BigButtonEvent e) {
        if (DoesPlayerIndexExist(e.PlayerIndex)) return;
        CreateSnake(e.PlayerIndex, 0, 0);
    }

private:
    bool DoesPlayerIndexExist(int playerIndex) {
        for(auto o : Objects()) {
            if (o->GetComponent<Snake>()->index == playerIndex) return true;
        }
        return false;
    }

    void CreateSnake(int index, int x, int y) {
        GameObject* snake = World()->CreateObject();
        snake->AddComponent<Transform>();
        snake->AddComponent<PointTransform>()->SetPosition(x, y);
        snake->AddComponent<Snake>()->tailLife = 0.6f;
        snake->GetComponent<Snake>()->index = index;
        snake->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 1, snakeColors[index]);
        snake->AddComponent<Material>();
        snake->AddComponent<Moveable>()->timeBetweenMoves = 0.15f;
        snake->AddComponent<Collidable>();
    }


};

class Game : public GameState<Game> {
public:
    GameWorld world;
    RenderSystem* renderer;
    GameObject* camera;
    BigButtonManager bigButtons;
    
    std::vector<Colour> colors {
        Colour::Green(),
        Colour::Red(),
        Colour::Blue(),
    };
    
    void Initialize() {
    
        bigButtons.Initialize();
    
        world.CreateSystem<SnakeCreatorSystem>()->SetBigButtons(bigButtons);
        renderer = world.CreateSystem<RenderSystem>();
        SnakeControlSystem* snakeControlSystem = world.CreateSystem<SnakeControlSystem>();
        snakeControlSystem->SetInput(Input);
        snakeControlSystem->SetBigButtons(bigButtons);
        world.CreateSystem<PowerupSystem>();
        world.CreateSystem<SnakeSystem>();
        world.CreateSystem<MoveSystem>();
        world.CreateSystem<TimedLifeSystem>();
        world.CreateSystem<PointTransformSystem>();
        world.CreateSystem<CollisionSystem>();
        
        
        camera = world.CreateObject();
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = { 0, 0, 100 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        
        for (int i=0; i<20; i++) {
            CreatePowerup(-20 + MathHelper::Random(40), -10 + MathHelper::Random(20));
        }
    }
    
    void CreatePowerup(int x, int y) {
        GameObject* powerup = world.CreateObject();
        powerup->AddComponent<Transform>();
        powerup->AddComponent<Mesh>()->GetMesh<Vertex>().AddQuad(0, 0.8f, Colour::Yellow());
        powerup->AddComponent<Material>();
        powerup->AddComponent<PointTransform>()->SetPosition(x,y);
        powerup->AddComponent<Powerup>();
        powerup->AddComponent<Collidable>();
    }
    
    
    void Update(float dt) {
        bigButtons.Update(dt);
        world.Update(dt);
    }
    
    void Render() {
        renderer->Render();
    }
};

int main() {
    Engine e;
    e.Start<Game>();
	return 0;
}