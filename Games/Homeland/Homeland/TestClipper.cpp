#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "clipper.hpp"
#include "clip2tri.h"

using namespace Pocket;

class TestClipper : public GameState<TestClipper> {
public:
    GameWorld world;
    
    void Initialize() {
        world.CreateSystem<RenderSystem>();
        
        GameObject* camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = {0,0,100};
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        
        
        vector<vector<c2t::Point>> inputPolygons;
        
        for (int i=0; i<10; i++) {
        Vector2 p = {10 + i * 10.0f, 10};
        Vector2 s = {8.0f, 20.0f};
        inputPolygons.push_back({
            {p.x,p.y}, {p.x+s.x,p.y}, {p.x+s.x,p.y+s.y}, {p.x,p.y+s.y},
            {p.x + s.x * 0.5f, p.y + s.y * 0.5f }
        });
        }
        
        vector<c2t::Point> outer { { 0,0 }, {0,100}, {100,100}, {100,0}};
        
        c2t::clip2tri clipper;
        
        vector<c2t::Point> navigationMesh;
        
        clipper.triangulate(inputPolygons, navigationMesh, outer);
        
        GameObject* cube = world.CreateObject();
        cube->AddComponent<Transform>();
        auto& mesh = cube->AddComponent<Mesh>()->GetMesh<Vertex>();
        
        int i=0;
        for (auto p : navigationMesh) {
            Vertex v;
            v.Position = {p.x, p.y,0};
            v.Color = Colour::White();
            mesh.vertices.push_back(v);
            mesh.triangles.push_back(i);
            i++;
        }
        
        cube->AddComponent<Material>();
        
        
        
        /*
        
        ClipperLib::Path outer;
        outer.push_back({0,0});
        outer.push_back({0,1000});
        outer.push_back({1000,1000});
        outer.push_back({1000,0});
        
        ClipperLib::Path inner;
        
        
        
        inner.push_back({250,250});
        inner.push_back({250,750});
        inner.push_back({750,750});
        inner.push_back({750,250});
        
        
        
        ClipperLib::Clipper c;
        c.AddPath(outer, ClipperLib::ptSubject, true);
        c.AddPath(inner, ClipperLib::ptClip, true);
        ClipperLib::Paths solution;
        c.Execute(ClipperLib::ctIntersection, solution);
        
        */
        
        
        /*
        ClipperLib::Paths subj(2), clip(1), solution;
	
	//define outer blue 'subject' polygon
	subj[0] << 
	  ClipperLib::IntPoint(180,200) << ClipperLib::IntPoint(260,200) <<
	  ClipperLib::IntPoint(260,150) << ClipperLib::IntPoint(180,150);
	
	//define subject's inner triangular 'hole' (with reverse orientation)
	subj[1] << 
	  ClipperLib::IntPoint(215,160) << ClipperLib::IntPoint(230,190) << ClipperLib::IntPoint(200,190);
	
	//define orange 'clipping' polygon
	clip[0] << 
	  ClipperLib::IntPoint(190,210) << ClipperLib::IntPoint(240,210) << 
	  ClipperLib::IntPoint(240,130) << ClipperLib::IntPoint(190,130);
	
	
	//perform intersection ...
	ClipperLib::Clipper c;
	c.AddPaths(subj, ClipperLib::ptSubject, true);
	c.AddPaths(clip, ClipperLib::ptClip, true);
	c.Execute(ClipperLib::ctIntersection, solution, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
        */
    }
    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main__hej() {
    Engine e;
    e.Start<TestClipper>();
	return 0;
}