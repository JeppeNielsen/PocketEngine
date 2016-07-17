
#include "Engine.hpp"
#include "GameState.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"

using namespace Pocket;


class Game : public GameState<Game> {
	
	GameWorld world;

	void Initialize() {
		world.CreateSystem<RenderSystem>();
		GameObject* camera = world.CreateObject();
		camera->AddComponent<Transform>()->Position = { 0,0,20 };
		camera->AddComponent<Camera>();

		GameObject* cube = world.CreateObject();
		cube->AddComponent<Transform>();
		cube->AddComponent<Material>();
		cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);

	}

	void Update(float dt) {
		world.Update(dt);
	}

	void Render() {
		world.Render();
	}

};


int main()
{	
	Engine e;
	e.Start<Game>();
    return 0;
}

