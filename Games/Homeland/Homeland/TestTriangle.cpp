#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "triangle.h"

using namespace Pocket;


class TestTriangle : public GameState<TestTriangle> {
public:
    GameWorld world;
    bool wireframe;
    
    void Initialize() {
        RenderSystem* renderSystem = world.CreateSystem<RenderSystem>();
        
        GameObject* camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = {0,0,10};
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        
        std::vector<double> outer = {
        0,0,
        3,0,
        0,3,
        3,3,
        };
        
        std::vector<int> segments = {
            0,1,
            1,3,
            3,2,
            2,0,
            
        };
        
        std::vector<double> holes;
        
        AddHole(outer, segments, holes, 0, 1);
        AddHole(outer, segments, holes, {1,0}, 1);
        AddHole(outer, segments, holes, {1.5f,1.5f}, 1);
        
        
    /*
    
       void triangulate(char *triswitches, struct triangulateio *in,
                 struct triangulateio *out, struct triangulateio *vorout);
      */
      
        triangulateio in;
        memset(&in, 0, sizeof(triangulateio));
        in.numberofpoints = outer.size()/2;
        in.pointlist = &outer[0];
        
        in.holelist = &holes[0];
        in.numberofholes = holes.size()/2;
        
        in.segmentlist = &segments[0];
        //in.segmentmarkerlist = &segmentMarker[0];
        in.numberofsegments = segments.size()/2;
        
        triangulateio out;
        memset(&out, 0, sizeof(triangulateio));
        
        triangulate("zVp", &in, &out, NULL );
        
        GameObject* go = world.CreateObject();
        go->AddComponent<Transform>();
        go->AddComponent<Material>();
        auto& mesh = go->AddComponent<Mesh>()->GetMesh<Vertex>();
        
        for (int i=0; i<out.numberofpoints; i++) {
            double* p = &out.pointlist[i*2];
            std::cout<<p[0]<<","<<p[1]<<std::endl;
             Vertex v;
                v.Position = {(float)p[0], (float)p[1], 0};
                v.Color = Colour::White();
                mesh.vertices.push_back(v);
        }
        std::cout<<"-------------"<<std::endl;
        for (int i=0; i<out.numberoftriangles; i++) {
            int* triangle = &out.trianglelist[i*3];
            std::cout<<triangle[0]<<", "<<triangle[1]<<", " <<triangle[2]<<std::endl;
            mesh.triangles.push_back(triangle[0]);
            mesh.triangles.push_back(triangle[1]);
            mesh.triangles.push_back(triangle[2]);
        }
        std::cout<<"-------------"<<std::endl;
        
        
        Input.ButtonDown += event_handler(this, &TestTriangle::ButtonDown);
        wireframe = false;
    }
    
    void AddHole(std::vector<double>& points, std::vector<int>& segments, std::vector<double>& holes, Vector2 p, Vector2 size) {
        int index = points.size()/2;
        points.push_back(p.x);
        points.push_back(p.y);
        
        points.push_back(p.x+size.x);
        points.push_back(p.y);
        
        points.push_back(p.x);
        points.push_back(p.y+size.y);
        
        points.push_back(p.x+size.x);
        points.push_back(p.y+size.y);
        
        segments.push_back(index+0);
        segments.push_back(index+1);
        
        segments.push_back(index+1);
        segments.push_back(index+3);
        
        segments.push_back(index+3);
        segments.push_back(index+2);
        
        segments.push_back(index+2);
        segments.push_back(index+0);
        
        holes.push_back(p.x + size.x * 0.5f);
        holes.push_back(p.y + size.y * 0.5f);
        
    }
    
    void ButtonDown(std::string b) {
        if (b=="w") {
            wireframe = !wireframe;
        }
    }

    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        if (wireframe) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<TestTriangle>();
	return 0;
}