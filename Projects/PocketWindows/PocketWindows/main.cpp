
#include "Engine.hpp"
#include "GameState.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "ShaderComponent.hpp"

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
	GameObject* root;
	void Initialize() {

		root = world.CreateRoot();

		root->CreateSystem<RenderSystem>();
		root->CreateSystem<RotationSystem>();
		root->CreateSystem<FirstPersonMoverSystem>();
		GameObject* camera = root->CreateObject();
		camera->AddComponent<Transform>()->Position = { 0,0,10 };
		camera->AddComponent<Camera>();
		camera->AddComponent<FirstPersonMover>();

		GameObject* cube = root->CreateObject();
		cube->AddComponent<Transform>();
		cube->AddComponent<Renderable>();
		Shader<Vertex>& LitColored = cube->AddComponent<ShaderComponent>()->GetShader<Vertex>();

		LitColored.Load(
			SHADER_SOURCE(
				attribute vec4 Position;                   
				attribute vec4 Color;                      
				attribute vec3 Normal;                     
				uniform mat4 ViewProjection;               
				varying vec4 vColor;                       
				varying vec3 vNormal;                      
				void main() {                              
				   vColor = Color;                         
				   vNormal = Normal;                       
					gl_Position = Position * ViewProjection;
				}                                          
			)
			,
			SHADER_SOURCE(
				varying vec4 vColor;                       
				varying vec3 vNormal;                      
				uniform vec3 LightDirection;               
				uniform vec4 AmbientLight;                 
				void main() {                              
				   float n = clamp(dot(LightDirection, vNormal),0.0,1.0);              
				   vec4 color = (AmbientLight + vColor * n);
					gl_FragColor = vec4(color.r, color.g, color.b, vColor.a);
				}
			)
		);
		LitColored.SetValue("LightDirection", Vector3(1, 1, 1).Normalized());
		LitColored.SetValue("AmbientLight", Colour(0.0f));

		
		cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
		cube->AddComponent<Rotator>()->rot = { 3,1,0 };

		cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({ -4,0,0 }, 1);
		cube->AddComponent<Rotator>()->rot = { 3,1,0 };



		auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
		int counter = 0;
		for (auto& v : verts) {
			v.Color = Colour::HslToRgb(20 * counter, 1.0, 1.0, 1.0);
				counter++;
		}

		Input.ButtonDown.Bind(this, &Game::KeyDown);
	}

	void KeyDown(ButtonEvent key) {
		if (key.Id == "Esc") {
			exit(0);
		}
	}

	void Update(float dt) {
		Context().InputDevice().UpdateInputManager(&world.Input());
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

