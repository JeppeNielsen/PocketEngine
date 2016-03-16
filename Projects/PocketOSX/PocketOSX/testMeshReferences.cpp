#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "TouchSystem.hpp"

struct TimerDestroyer {
    float time;
};

template<typename T>
struct TimerDestroyerSystem : GameSystem<T, TimerDestroyer> {
    void ObjectAdded(GameObject<T>* o) {}
    void ObjectRemoved(GameObject<T>* o) {}
    void Update(float dt) {
        for(auto o : this->Objects()) {
            TimerDestroyer* d = o->template GetComponent<TimerDestroyer>();
            d->time -= dt;
            if (d->time<0) {
                o->Remove();
            }
        }
    }
};

struct Spinner {
    Vector3 speed;
};

template<typename T>
struct SpinnerSystem : GameSystem<T, Spinner, Transform> {
    void ObjectAdded(GameObject<T>* o) {}
    void ObjectRemoved(GameObject<T>* o) {}
    void Update(float dt) {
        for(auto o : this->Objects()) {
            Transform* t = o->template GetComponent<Transform>();
            Spinner* s = o->template GetComponent<Spinner>();
            Quaternion rot = Quaternion(s->speed * dt);
            Quaternion val = t->Rotation * rot;
            t->Rotation = val;
        }
    }
};


struct GameWorldSettings :
    GameSettings<
        RenderSystem<GameWorldSettings>,
        TimerDestroyerSystem<GameWorldSettings>,
        SpinnerSystem<GameWorldSettings>,
        TouchSystem<GameWorldSettings>
    > {
};


using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld<GameWorldSettings> world;
    
    using GameObject = GameObject<GameWorldSettings>;
    
    GameObject* camera;
    GameObject* cube;
    float rotation;
    GameObject* meshObject;
    
    void Initialize() {
    
        world.Initialize();
        
        camera = world.CreateObject();
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = { 50, 25, 100 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        
        camera->ToJson(std::cout, [] (GameObject* object, size_t type) -> bool {
            return true;
        });
        
        meshObject = world.CreateObject();
        meshObject->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {0.4f,20,0.4f});
        
        auto& verts = meshObject->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
    }
    
    void CreateObjects() {
    
        for(int x = 0; x<750; x++) {
            
            cube = world.CreateObject();
            cube->AddComponent<Transform>()->Position = {fmodf(x, 40)*2.5f,floorf(x / 40.0f)*2.5f,0};
            cube->AddComponent<Mesh>(meshObject);
            cube->AddComponent<Material>();
            
            //cube->AddComponent<TimerDestroyer>()->time = 0.13f + x * 0.015f;
            cube->AddComponent<Spinner>()->speed = {1+0.001f*x,2,0};
        }
    }
    
    void Update(float dt) {
        world.Update(dt);
        
        if (world.ObjectCount()==2) {
            CreateObjects();
        }
    }
    
    void Render() {
        world.Render();
    }
};

int main_nono() {
    Engine e;
    e.Start<Game>();
	return 0;
}