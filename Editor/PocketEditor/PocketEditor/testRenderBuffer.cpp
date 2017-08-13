//
//  testRenderBuffer.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "Gui.hpp"
#include "GameWorldViewportSystem.hpp"

using namespace Pocket;

/*
struct RenderTexture
    {
        GLuint framebuffer;
        GLuint tex;
        GLint old_fbo;

        int w;
        int h;

        void Initialize(GLuint width, GLuint height)
        {
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);

            glGenFramebuffers(1, &framebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            
            
            // The texture we're going to render to
glGenTextures(1, &tex);

// "Bind" the newly created texture : all future texture functions will modify this texture
glBindTexture(GL_TEXTURE_2D, tex);

// Give an empty image to OpenGL ( the last "0" )
glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

// Poor filtering. Needed !
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);




// The depth buffer
GLuint depthrenderbuffer;
glGenRenderbuffers(1, &depthrenderbuffer);
glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


// Set "renderedTexture" as our colour attachement #0
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

// Set the list of draw buffers.
GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


            glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            w = width;
            h = height;
        }

        void begin()
        {
            //glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            //glViewport(0,0,200,200);
        }

        void end()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
        }
    };

*/
//
//struct WorldCanvas {
//    WorldCanvas() : world(0) {}
//    GameWorld* world;
//    Point maxSize;
//    RenderTexture renderTexture;
//};
//
//struct WorldCanvasSystem : public GameSystem<WorldCanvas, Transform, Sizeable, Mesh, TextureComponent> {
//
//    InputDevice* inputDevice;
//
//    void ObjectAdded(GameObject* object) {
//        auto canvas = object->GetComponent<WorldCanvas>();
//        canvas->renderTexture.Initialize(canvas->maxSize.x, canvas->maxSize.y);
//        object->GetComponent<TextureComponent>()->Texture().SetCustomTexture(canvas->renderTexture.GetTexture(), canvas->maxSize.x, canvas->maxSize.y);
//    }
//
//    void Update(float dt) {
//        for(auto o : Objects()) {
//            UpdateObject(o, dt);
//        }
//    }
//    
//    void Render() {
//        for(auto o : Objects()) {
//            RenderObject(o);
//        }
//    }
//
//    void UpdateObject(GameObject* object, float dt) {
//        auto canvas = object->GetComponent<WorldCanvas>();
//        if (!canvas->world) return;
//        
//        Sizeable* sizeable = object->GetComponent<Sizeable>();
//        auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
//        
//        if (mesh.vertices.size()!=4) {
//            mesh.vertices.resize(4);
//        }
//        if (mesh.triangles.size()!=6) {
//            mesh.triangles.resize(6);
//            
//            mesh.triangles[0]=0;
//            mesh.triangles[1]=1;
//            mesh.triangles[2]=2;
//            
//            mesh.triangles[3]=0;
//            mesh.triangles[4]=2;
//            mesh.triangles[5]=3;
//        }
//        
//        Vector2 size = sizeable->Size;
//        
//        //Vector2 uv = { size.x / canvas->maxSize.x, size.y / canvas->maxSize.y };
//        //uv *= 2.0f;
//
//        Vector2 uv = 1;
//        
//        mesh.vertices[0].Position = {0,0,0};
//        mesh.vertices[1].Position = {size.x,0,0};
//        mesh.vertices[2].Position = {size.x,size.y,0};
//        mesh.vertices[3].Position = {0,size.y,0};
//        
//        for(int i=0; i<4; ++i) {
//            mesh.vertices[i].Color = Colour::White();
//        }
//        
//        mesh.vertices[0].TextureCoords = {0,0};
//        mesh.vertices[1].TextureCoords = {uv.x,0};
//        mesh.vertices[2].TextureCoords = uv;
//        mesh.vertices[3].TextureCoords = {0,uv.y};
//        
//        Vector2 worldPosition = object->GetComponent<Transform>()->World().TransformPosition(0);
//        
//        Vector2 scale = { size.x / canvas->maxSize.x, size.y / canvas->maxSize.y };
//        //scale *= 2.0f;
//        
//        Matrix4x4 mat = Matrix4x4::CreateTranslation(worldPosition) * Matrix4x4::CreateScale({scale.x, scale.y,1});
//        mat = mat.Invert();
//        
//        Vector2 test = Engine::Context().InputDevice().GetTouchPosition(0);
//        
//        Vector2 output = mat.TransformPosition(test);
//        
//        std::cout << test << "   -  " << output << std::endl;
//        
//        Vector2 oldScreenSize = Engine::Context().ScreenSize;
//        float oldScalingFactor = Engine::Context().ScreenScalingFactor;
//        Engine::Context().ScreenSize = Vector2(canvas->maxSize.x, canvas->maxSize.y);
//        Engine::Context().ScreenScalingFactor = 1.0f;
//        
//        canvas->world->Input().SetTransformationMatrix(mat);
//        inputDevice->UpdateInputManager(&canvas->world->Input());
//       
//        canvas->world->Update(dt);
//        Engine::Context().ScreenSize = oldScreenSize;
//        Engine::Context().ScreenScalingFactor = oldScalingFactor;
//    }
//
//    void RenderObject(GameObject* object) {
//        auto canvas = object->GetComponent<WorldCanvas>();
//        if (!canvas->world) return;
//        
//        canvas->renderTexture.Begin();
//        glClearColor(1,0,0, 1);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        
//        Vector2 oldScreenSize = Engine::Context().ScreenSize;
//        float oldScalingFactor = Engine::Context().ScreenScalingFactor;
//        Engine::Context().ScreenSize = Vector2(canvas->maxSize.x, canvas->maxSize.y);
//        Engine::Context().ScreenScalingFactor = 1.0f;
//        canvas->world->Render();
//        Engine::Context().ScreenSize = oldScreenSize;
//        Engine::Context().ScreenScalingFactor = oldScalingFactor;
//        
//        canvas->renderTexture.End();
//    }
//};




class TestRenderBuffer : public GameState<TestRenderBuffer> {
public:
    GameWorld world;
    GameWorld cubeWorld;
    GameObject* viewport;
    
    void Initialize() {
        
        GameObject* root = world.CreateRoot();
        Gui* gui = root->CreateSystem<Gui>();
        gui->Setup("images.png", "images.xml", Context().Viewport());
        root->CreateSystem<GameWorldViewportSystem>();
        
        viewport = gui->CreatePivot(root, 100);
        viewport->AddComponent<GameWorldViewport>()->RenderSize = {1024,1024};
        viewport->GetComponent<GameWorldViewport>()->World = &cubeWorld;
        viewport->AddComponent<Mesh>();
        viewport->AddComponent<Sizeable>()->Size = {500,300};
        viewport->AddComponent<TextureComponent>();
        viewport->AddComponent<Renderable>();
        
        
        auto cubeRoot = cubeWorld.CreateRoot();
        cubeRoot->CreateSystem<RenderSystem>();
        cubeRoot->CreateSystem<TouchSystem>();
        cubeRoot->CreateSystem<DraggableSystem>();
        
        auto cube = cubeRoot->CreateObject();
        cube->AddComponent<Transform>()->Position = {0,0,0};
        cube->AddComponent<Renderable>();
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube->AddComponent<Touchable>()->Click.Bind([cube] (TouchData e) {
            auto& mesh = cube->GetComponent<Mesh>()->GetMesh<Vertex>();
            for(auto& v : mesh.vertices) {
                v.Color = Colour::HslToRgb(MathHelper::Random() * 360, 1.0, 1.0, 1.0);
            }
        });
        cube->AddComponent<Draggable>();
        
        auto camera = cubeRoot->CreateObject();
        camera->AddComponent<Transform>()->Position = {0,0,10};
        camera->AddComponent<Camera>();
    }

    void Update(float dt) {
        Context().InputDevice().UpdateInputManager(&world.Input());
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<TestRenderBuffer>();
	return 0;
}
