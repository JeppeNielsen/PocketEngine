#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "clipper.hpp"
#include "clip2tri.h"
//#include "gpc.h"
#include "tesselator.h"

using namespace Pocket;


class TestClipper : public GameState<TestClipper> {
public:
    GameWorld world;
    bool wireframe;
    
    void Initialize() {
        RenderSystem* renderSystem = world.CreateSystem<RenderSystem>();
        
        GameObject* camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = {0,0,20};
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        
        /*
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
        */
        
        
        
        ClipperLib::Path outer;
        outer.push_back({0,0});
        outer.push_back({0,1000});
        outer.push_back({1000,1000});
        outer.push_back({2000,0});
        
        ClipperLib::Path inner;
        
        inner.push_back({250,250});
        inner.push_back({750,250});
        inner.push_back({750,750});
        inner.push_back({250,750});
        
         ClipperLib::Path inner2;
        
        int off = 600;
        int offY = 10;
        
        inner2.push_back({250+off,250+offY});
        inner2.push_back({750+off,250+offY});
        inner2.push_back({750+off,750+offY});
        inner2.push_back({250+off,750+offY});
        
        ClipperLib::Clipper c;
        c.AddPath(outer, ClipperLib::ptSubject, true);
        c.AddPath(inner, ClipperLib::ptClip, true);
        //c.AddPath(inner2, ClipperLib::ptClip, true);
        
        
        ClipperLib::Paths solution;
        c.Execute(ClipperLib::ctDifference, solution);
        
        
        
        
        
        
        
        TESStesselator* tess = tessNewTess(0);
        
        for (ClipperLib::Path& path : solution) {
            std::vector<float> contour;
            for (int i=0; i<path.size(); i++) {
                contour.push_back(path[i].X);
                contour.push_back(path[i].Y);
            }
            tessAddContour(tess, 2, &contour[0], sizeof(float)*2, (int)path.size());
        }
        
        int polySize = 3;
        //int error = tessTesselate(tess, TESS_WINDING_NONZERO, polySize, 3, 2, NULL);
//        ( TESStesselator *tess, int windingRule, int elementType, int polySize, int vertexSize, const TESSreal* normal );


        int error = tessTesselate(tess, TESS_WINDING_ODD, TESS_CONNECTED_POLYGONS, polySize, 2, NULL);
        
         GameObject* go = world.CreateObject();
        go->AddComponent<Transform>();
        go->AddComponent<Material>()->Shader = &renderSystem->Shaders.Colored;
        auto& mesh = go->AddComponent<Mesh>()->GetMesh<Vertex>();
        
        const TESSreal* verts = tessGetVertices( tess );
        const int nelems = tessGetElementCount(tess);
        const TESSindex* elems = tessGetElements(tess);

        int indexCounter = 0;
        for (int i = 0; i < nelems; i++) {
            const TESSindex* poly = &elems[i * polySize*2];
            const TESSindex* nei = &poly[polySize];
            
            //glBegin(GL_POLYGON);
            for (int j = 0; j < polySize; j++) {
                // if (poly[j] == TESS_UNDEF) break;

                const TESSreal* pos = &verts[poly[j]*2];

                std::cout<<pos[0]<<", "<<pos[1]<<std::endl;

                Vertex v;
                v.Position = {pos[0]*0.01f, pos[1]*0.01f,0};
                v.Color = Colour::HslToRgb(i*20, 1, 1, 1);
                mesh.vertices.push_back(v);
                mesh.triangles.push_back(indexCounter++);

                //glVertex2fv(&);
            }
            std::cout<<" neighbors: " << nei[0]<<std::endl;
            std::cout<<" neighbors: " << nei[1]<<std::endl;
            std::cout<<" neighbors: " << nei[2]<<std::endl;
            std::cout<<"-----"<<std::endl;
            //glEnd();
        }
        
        
        
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
        
        
        /*
        std::vector<gpc_vertex> subjectVertices = {
            { 0,0 },
            {10,0},
            {10,10},
            {0,10},
        };
        
        std::vector<gpc_vertex> clipVertices = {
            {-2.5,1.5},
            {7.5,7.5},
            {9.5,2.5},
            
           // {-2.5,7.5},
        };
        
        
        gpc_vertex_list subjectContour;
        subjectContour.vertex = &subjectVertices[0];
        subjectContour.num_vertices = (int)subjectVertices.size();
        
        gpc_polygon subject;
        subject.num_contours = 1;
        subject.contour = &subjectContour;
        //subject.hole = 0;
        
        gpc_vertex_list clipContour;
        clipContour.vertex = &clipVertices[0];
        clipContour.num_vertices = (int)clipVertices.size();
        
        gpc_polygon clip;
        clip.num_contours = 1;
        clip.contour = &clipContour;
        //clip.hole = 0;
        
        gpc_polygon clipped;
        gpc_polygon_clip(GPC_DIFF, &subject, &clip, &clipped);
        
        gpc_tristrip output;
        
        //gpc_tristrip_clip(GPC_DIFF, &subject, &clip, &output);
        gpc_polygon_to_tristrip(&clipped, &output);
        
        {
            GameObject* go = world.CreateObject();
            go->AddComponent<Transform>();
            go->AddComponent<Material>()->Shader = &renderSystem->Shaders.Colored;
            
            auto& mesh = go->AddComponent<Mesh>()->GetMesh<Vertex>();
            
            int triIndex = 0;
            for (int i=0; i<output.num_strips; i++) {
                gpc_vertex_list& strip = output.strip[i];
                for(int v = 0; v<strip.num_vertices; v++) {
                    std::cout << "" << strip.vertex[v].x<< "," << strip.vertex[v].y<<std::endl;
                }
                std::cout << "-------"<<std::endl;
                
                for (int v = 0; v < strip.num_vertices-2; v++) {
                    if (v & 1){
                        AddToMesh(mesh, strip.vertex[v]);
                        AddToMesh(mesh, strip.vertex[v+1]);
                        AddToMesh(mesh, strip.vertex[v+2]);
                    }
                    else{
                        AddToMesh(mesh,strip.vertex[v]);
                        AddToMesh(mesh,strip.vertex[v+2]);
                        AddToMesh(mesh,strip.vertex[v+1]);
                    }
                    
                    mesh.triangles.push_back(triIndex);
                    mesh.triangles.push_back(triIndex+2);
                    mesh.triangles.push_back(triIndex+1);
                    triIndex+=3;
                }
                
            }
        }
        
        */
        
        Input.ButtonDown += event_handler(this, &TestClipper::ButtonDown);
        wireframe = false;
    }
    
    
    void ButtonDown(std::string b) {
        if (b=="w") {
            wireframe = !wireframe;
        }
    }
    
    /*
    void AddToMesh(VertexMesh<Vertex>& mesh, gpc_vertex& vertex) {
        Vertex ve;
        ve.Position = {(float)vertex.x, (float)vertex.y, 0};
        ve.Color = Colour::HslToRgb(mesh.vertices.size(), 1, 1, 1.0);
        mesh.vertices.push_back(ve);
    }
    */
    
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

int main_wefwef() {
    Engine e;
    e.Start<TestClipper>();
	return 0;
}