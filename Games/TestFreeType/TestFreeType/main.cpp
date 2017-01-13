


/* example1.c                                                      */
/*                                                                 */
/* This small program shows how to print a rotated string with the */
/* FreeType 2 library.                                             */


#include <stdio.h>
#include <string.h>
#include <math.h>

#include <ft2build.h>
#include FT_FREETYPE_H


#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "FirstPersonMoverSystem.hpp"

using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld world;
    GameObject* camera;
    GameObject* cube;
    GameObject* root;
    
    struct Rotator { Vector3 speed; };
    struct RotatorSystem : public GameSystem<Transform, Rotator> {
        void Update(float dt) {
            for(auto o : Objects()) { o->GetComponent<Transform>()->Rotation += o->GetComponent<Rotator>()->speed * dt; }
        }
    };
    
    FT_Library    library;
    FT_Face       face;

    FT_GlyphSlot  slot;
    FT_Matrix     matrix;                 /* transformation matrix */
    FT_Vector     pen;                    /* untransformed origin  */
    FT_Error      error;

    char*         filename;
    char*         text;

    double        angle;
    int           target_height;
    int           n, num_chars;
    
    GameObject* mesh;
    
    void Initialize() {
        
        root = world.CreateRoot();
        
        root->CreateSystem<RenderSystem>();
        root->CreateSystem<FirstPersonMoverSystem>();
        
        camera = root->CreateObject();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        camera->AddComponent<FirstPersonMover>();
        
        mesh = root->CreateObject();
        mesh->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 0.5f);
        auto& verts = mesh->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }

        
        
        //filename      = "/Projects/PocketEngine/Games/TestFreeType/SourceSansPro-Regular.ttf";                           /* first argument     */
        //filename = "/Users/Jeppe/Library/Fonts/D3Mouldism.ttf";
        filename = "/Users/Jeppe/Library/Fonts/NuevaStd-Bold_ttf.ttf";
        
        text          = "Pocket Engine";                           /* second argument    */
        num_chars     = strlen( text );
        angle         = 0;//( 25.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */

        error = FT_Init_FreeType( &library );              /* initialize library */
        /* error handling omitted */

        error = FT_New_Face( library, filename, 0, &face );/* create face object */
        /* error handling omitted */

        /* use 50pt at 100dpi */
        error = FT_Set_Char_Size( face, 40 * 64, 0,
                            50, 0 );                /* set character size */
        /* error handling omitted */

        slot = face->glyph;

        /* set up matrix */
        matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
        matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
        matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
        matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

        /* the pen position in 26.6 cartesian space coordinates; */
        /* start at (300,200) relative to the upper left corner  */
        pen.x = 0;
        pen.y = 0;

        for ( n = 0; n < num_chars; n++ )
        {
            /* set transformation */
            FT_Set_Transform( face, &matrix, &pen );

            /* load glyph image into the slot (erase previous one) */
            error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
            if ( error )
                continue;                 /* ignore errors */

            /* now, draw to our target surface (convert position) */
            drawCubes(&slot->bitmap, {(float)(slot->bitmap_left), (float)(slot->bitmap_top),0});

            /* increment pen position */
            pen.x += slot->advance.x;
            pen.y += slot->advance.y;
        }
    }
    
    void CreateCube(Vector3 position) {
        GameObject* go = root->CreateObject();
        go->AddComponent<Transform>()->Position = position;
        go->AddComponent<Mesh>(mesh);
        go->AddComponent<Renderable>();
    }
    
    void CreateColorCube(Vector3 position, Colour color) {
        GameObject* go = root->CreateObject();
        go->AddComponent<Transform>()->Position = position;
        go->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 0.5f, color);
        go->AddComponent<Renderable>();
    }
    
    void drawCubes( FT_Bitmap*  bitmap, Vector3 position)
    {
        for (int x = 0; x<bitmap->width; ++x) {
            for (int y=0; y<bitmap->rows; ++y) {
                 unsigned char pixel = bitmap->buffer[y * bitmap->width + x];
                
                if (pixel>0) {
                    float alpha = pixel / 255.0f;
                    Colour color = Colour::HslToRgb(x*10, 1, alpha, 1.0f);
                    CreateColorCube(position+Vector3((float)x,(float)-y,0), color);
                }
            }
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

int main_truetypetest() {
    Engine e;
    e.Start<Game>();
	return 0;
}
