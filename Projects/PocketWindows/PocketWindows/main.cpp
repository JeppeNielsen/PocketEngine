
#include "Engine.hpp"
#include "GameState.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "FirstPersonMoverSystem.hpp"

using namespace Pocket;

struct Rotator {
	Vector3 rot;
};

struct RotationSystem : public GameSystem<Transform, Rotator> {

	void Update(float dt) override {
		for (auto o : Objects()) {
			o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Rotator>()->rot * dt;
		}
	}

};


class Game : public GameState<Game> {
	
	GameWorld world;

	void Initialize() {
		world.CreateSystem<RenderSystem>();
		world.CreateSystem<RotationSystem>();
		world.CreateSystem<FirstPersonMoverSystem>()->Input = &Input;
		GameObject* camera = world.CreateObject();
		camera->AddComponent<Transform>()->Position = { 0,0,10 };
		camera->AddComponent<Camera>();
		camera->AddComponent<FirstPersonMover>();

		GameObject* cube = world.CreateObject();
		cube->AddComponent<Transform>();
		cube->AddComponent<Material>();
		cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
		cube->AddComponent<Rotator>()->rot = { 3,1,0 };

		auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
		int counter = 0;
		for (auto& v : verts) {
			v.Color = Colour::HslToRgb(20 * counter, 1.0, 1.0, 1.0);
				counter++;
		}

		Input.ButtonDown.Bind(this, &Game::KeyDown);
	}

	void KeyDown(std::string key) {
		if (key == "Esc") {
			exit(0);
		}
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

