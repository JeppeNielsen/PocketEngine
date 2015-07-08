#include "Engine.hpp"
#include "GameState.hpp"
#include "OpenGL.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"

#include <android/log.h>


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))



using namespace Pocket;

class Test : public GameState<Test> {

    GameWorld world;
    GameObject* cube;
    
    Shader<Vertex> colorShader;

    void Initialize() {
    
        std::string vertexShader =
            "attribute vec4 Position;\n"
            "attribute vec2 TexCoords; \n"
            "attribute vec4 Color; \n"
            "attribute vec3 Normal; \n"
            "uniform mat4 ViewProjection;\n"
            "varying vec2 DestinationTexCoords;\n"
            "varying vec4 DestinationColor;\n"
            "varying vec3 DestinationNormal;\n"
            "void main(void) {\n"
            "	DestinationTexCoords = TexCoords;\n"
            "   DestinationColor = Color; \n"
            "   DestinationNormal = Normal; \n"
            "	gl_Position = Position * ViewProjection;\n"
            "}\n";
        
        std::string fragmentShader =
            "varying vec2 DestinationTexCoords;"
            "varying vec4 DestinationColor;"
            "varying vec3 DestinationNormal;"
            "uniform vec3 LightDirection;"
            "uniform vec4 AmbientLight;"
            "uniform sampler2D Texture;"
            "void main(void) {"
            "   float n = clamp(dot(LightDirection, DestinationNormal),0.0,1.0); "
            "   vec4 color = DestinationColor;"//texture2D(Texture, DestinationTexCoords) * DestinationColor; "
            "   vec4 fullColor = AmbientLight + color * n; "
            "	gl_FragColor = vec4(fullColor.r, fullColor.g, fullColor.b, DestinationColor.a);\n"
            "}";
        
        if (!colorShader.Create(vertexShader,fragmentShader)) {
            LOGI("SHADER FAILED");
        }
    
        colorShader.SetValue("LightDirection", Vector3(1,1,1).Normalized());
    
    
    
    
        world.CreateSystem<RenderSystem>();

        GameObject* cameraObject = world.CreateObject();
        cameraObject->AddComponent<Transform>()->Position = {0,0,5};
        cameraObject->AddComponent<Camera>()->Viewport = Manager().Viewport();

        Box viewPort = Manager().Viewport();

        cube = world.CreateObject();
        cube->AddComponent<Transform>();
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube->AddComponent<Material>()->Shader = &colorShader;
    }

    void Update(float dt) {
        static float rot = 0;
        rot += dt;
        cube->GetComponent<Transform>()->Rotation = Quaternion(rot, Vector3(0.3f,1,0.6f).Normalized());
        world.Update(dt);
    }

    void Render() {
        glClearColor(0,0,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<Test>();
    return 0;
}
