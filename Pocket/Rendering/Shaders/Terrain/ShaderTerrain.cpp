#include "ShaderTerrain.hpp"
#include "VisibleObject.hpp"
#include "Terrain.hpp"
#include "RenderSystem.hpp"
using namespace Pocket;
using namespace std;

void ShaderTerrain::Initialize() {

    vertexStride = sizeof(Vertex);
    
	string vertexShader =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
#endif
		"attribute vec4 Position;\n"
		"attribute vec2 TexCoords;\n"
		"attribute vec4 Color;\n"
		"uniform mat4 ViewProjection;\n"
		"uniform mat4 World;\n"
		"varying vec2 DestinationTexCoords;\n"
		"varying vec4 DestinationColor;\n"
		"void main(void) {\n"
		"	DestinationTexCoords = TexCoords;\n"
		"	DestinationColor = Color;\n"
		"	gl_Position = Position * World * ViewProjection;\n"
		"}\n";
    
	string vertexShaderBatching =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
#endif
        "attribute vec4 Position;\n"
        "attribute vec2 TexCoords;\n"
        "attribute vec4 Color;\n"
        "attribute vec3 Normal;\n"
        "uniform mat4 ViewProjection;\n"
        "varying vec2 DestinationTexCoords;\n"
        "varying vec4 DestinationColor;\n"
        "varying vec3 DestinationNormal;\n"
        "void main(void) {\n"
        "	DestinationTexCoords = TexCoords;\n"
        "	DestinationColor = Color;\n"
        "	DestinationNormal = normalize(Normal);\n"
        "	gl_Position = Position * ViewProjection;\n"
        "}\n";
        
	string fragmentShader =
    
#ifdef OPENGL_EMBEDDED
    "precision mediump float;"
    "varying lowp vec2 DestinationTexCoords;"
    "varying lowp vec4 DestinationColor;"
    "varying lowp vec3 DestinationNormal;"
#endif
#ifdef OPENGL_DESKTOP
    "#version 120 \n"
    "varying vec2 DestinationTexCoords;"
    "varying vec4 DestinationColor;"
    "varying vec3 DestinationNormal;"
#endif
    "uniform vec3 LightDirection;"
    "uniform vec4 LightColor;"
    "uniform vec4 AmbientColor;"
    "uniform sampler2D Texture0;"
    "uniform sampler2D Texture1;"
    "void main(void) {"
    "   vec4 color = texture2D(Texture0, DestinationTexCoords); "
    "   color = mix(color, texture2D(Texture1, DestinationTexCoords), DestinationColor.r); "
    "   float lightIntensity = max(dot(LightDirection, DestinationNormal), 0.0); "
    "	gl_FragColor = color * (AmbientColor + LightColor * lightIntensity);"
    "}";
    
	//if (!shader.CreateShaders(vertexShader, fragmentShader)) return;
    if (!shaderBatching.CreateShaders(vertexShaderBatching, fragmentShader)) {
        return;
    }

	positionSlot = shaderBatching.CreateAttribVariable("Position");
    textureCoordSlot = shaderBatching.CreateAttribVariable("TexCoords");
	colorSlot = shaderBatching.CreateAttribVariable("Color");
    normalSlot = shaderBatching.CreateAttribVariable("Normal");
    
    viewProjectionSlot = shaderBatching.CreateUniformVariable("ViewProjection");
    
    lightDirectionSlot = shaderBatching.CreateUniformVariable("LightDirection");
	lightColorSlot = shaderBatching.CreateUniformVariable("LightColor");
	ambientColorSlot = shaderBatching.CreateUniformVariable("AmbientColor");
	textureSlot[0] = shaderBatching.CreateUniformVariable("Texture0");
    textureSlot[1] = shaderBatching.CreateUniformVariable("Texture1");
    
    LightDirection = Vector3(1,1,1).Normalized();
    LightColor = Colour::White();
    LightIntensity = 1.0f;
    AmbientColor = Colour(0.1f,0.1f,0.1f,1.0f);
    AmbientIntensity = 0.0f;
}

void ShaderTerrain::OnStart(float* viewProjection) {
    //glUniformMatrix4fv(viewProjectionSlot, 1, GL_FALSE, viewProjection);
    //glUniform1i(textureSlot, 0);
    //lastTexture = 0;
}

void ShaderTerrain::Bind() {
    //glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	//glVertexAttribPointer(textureCoordSlot, 2, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)12);
	//glVertexAttribPointer(colorSlot, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
}


void ShaderTerrain::Apply(VisibleObject* visibleObject) {
    
    //glUniformMatrix4fv(worldSlot, 1, GL_FALSE, visibleObject->transform->World.GetValue()->GetGlMatrix());
    
    //if (visibleObject->texture!=lastTexture) {
    //    lastTexture = visibleObject->texture;
    //    glBindTexture(GL_TEXTURE_2D, lastTexture->Texture().GetHandle());
    //}
}

void ShaderTerrain::OnInitBatch(const VisibleObject *visibleObject) {
    
    Terrain& terrain = *visibleObject->object->GetComponent<Terrain>();
    
    glUniformMatrix4fv(viewProjectionSlot, 1, GL_FALSE, viewProjection);
    glUniform1i(textureSlot[0], 0);
    glUniform1i(textureSlot[1], 1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, visibleObject->texture->Texture().GetHandle());
    
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, terrain.Textures[0]->Texture().GetHandle());
    
    
    glUniform3f(lightDirectionSlot, LightDirection.x, LightDirection.y, LightDirection.z);
	glUniform4f(lightColorSlot, LightColor.R() * LightIntensity, LightColor.G()* LightIntensity, LightColor.B()* LightIntensity, LightColor.A());
	glUniform4f(ambientColorSlot, AmbientColor.R() * AmbientIntensity, AmbientColor.G() * AmbientIntensity, AmbientColor.B() * AmbientIntensity, AmbientColor.A());
    
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(textureCoordSlot, 2, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)12);
	glVertexAttribPointer(colorSlot, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
	glVertexAttribPointer(normalSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)24);
}

bool ShaderTerrain::RenderVertices(const VisibleObject* visibleObject, const Mesh* mesh, VertexRenderer& renderer) {

    world = visibleObject->transform->World.GetValue();
    Terrain& terrain = *visibleObject->object->GetComponent<Terrain>();
    Transform* transform = cameraObject->GetComponent<Transform>();
    Vector3 camPosition = transform->Position;
    chunks = 0;
    Draw(terrain, *frustum, camPosition, 0, 0, 1, 1, 1, terrain.maxLevels, visibleObject, mesh, renderer);
    //cout<<"Chunks:"<<chunks<<endl;
    return false;
}

bool ShaderTerrain::EvaluateCriterion(const Terrain& terrain, float halfU, float halfV, float u1, float u2, int level, const Vector3& camPosition) {
    
    float extentOfTriangle = (u1 - u2) * terrain.size.x / (terrain.meshPatchSize - 1.0f);
    float extentOfTriangle2 = extentOfTriangle * extentOfTriangle;
    
    Vector3 chunkPosition = Vector3(
                                    halfU * terrain.size.x,
                                    camPosition.y,
                                    halfV * terrain.size.y);
    
    Vector3 cameraDistance = chunkPosition - camPosition;
    
    float length2 = cameraDistance.LengthSqaured();
    float criterio2 = length2 / extentOfTriangle2;
    
    return (criterio2 > terrain.lod * terrain.lod || level < 1);
}



void ShaderTerrain::Draw(const Terrain& terrain, const BoundingFrustum& frustum, const Vector3& camPosition, float u1, float v1, float u2, float v2, float scale, int level,const VisibleObject* visibleObject, const Mesh* mesh, VertexRenderer& renderer) {

    //BoundingBox boundingBox = new BoundingBox(new Vector3(u1 * size.X, minHeight, v1 * size.Y),
    //                                          new Vector3(u2 * size.X, size.Y, v2 * size.Y));
    
    Vector3 minBox(u1 * terrain.size.x, terrain.minHeight, v1 * terrain.size.y);
    Vector3 maxBox(u2 * terrain.size.x, terrain.maxHeight, v2 * terrain.size.y);
    
    BoundingBox boundingBox((minBox + maxBox) * 0.5f, maxBox - minBox);
    
    if (frustum.Intersect(boundingBox) == BoundingFrustum::OUTSIDE) {
        return;
    }
    
    float halfU = (u1 + u2) * 0.5f;
    float halfV = (v1 + v2) * 0.5f;
    
    if (EvaluateCriterion(terrain, halfU, halfV, u1, u2, level, camPosition)) {
       // glUniform3f(TerrainPatchValues, u1, v1, scale);
       // DrawElements();
        
        
        
        float skirtHeight = 0.2f;
        
        /*
        
        "   vec4 pos = Position; \n"
        "   vec2 uv; \n"
        "   uv.x = pos.x * TerrainPatchValues.z + TerrainPatchValues.x;\n"
        "   uv.y = pos.z * TerrainPatchValues.z + TerrainPatchValues.y;\n"
        "   pos.x = uv.x * TerrainSize.x;\n"
        "   pos.z = uv.y * TerrainSize.y;\n"
        "   float height =  TerrainMinHeight + (TerrainMaxHeight - TerrainMinHeight) * texture2D(HeightTexture, uv).x ;\n"
        "   if (pos.y<-0.5) { pos.y = height - TerrainSkirtHeight; } else { pos.y = height; }\n"
        //"   if (TerrainMinHeight>0) { if (pos.y<TerrainMinHeight) { pos.y = TerrainMinHeight; } }\n"
        //"   if (TerrainMaxHeight>0) { if (pos.y>TerrainMaxHeight) { pos.y = TerrainMaxHeight; } } \n"
        "	DestinationTexCoords = uv * TerrainUVScale;\n"
        
        */
        
        size_t verticesSize = mesh->VerticesSize();
        size_t trianglesSize = mesh->TrianglesSize();
        
        renderer.Begin(verticesSize, trianglesSize);
        
        size_t index = renderer.vertexIndex;
        
        for (size_t v=0; v<verticesSize; ++v) {
            const Vertex& source = mesh->vertices[v];
            Vertex& dest = renderer.vertices[renderer.vertexIndex++];
            dest.Position = source.Position;
            
            Vector2 uv;
            uv.x = dest.Position.x * scale + u1;
            uv.y = dest.Position.z * scale + v1;
            
            dest.Position.x = uv.x * terrain.size.x;
            dest.Position.z = uv.y * terrain.size.y;
            
            float height = terrain.GetHeight(uv.x, uv.y, dest.Normal, dest.Color);//sinf(uv.x * 100.0f) * 0.3f;// terrain.GetHeight(uv.x, uv.y);
            
            if (source.Position.y<-0.5f) { dest.Position.y = height - skirtHeight; } else { dest.Position.y = height; }
            //dest.Position.y = height;
            /*
            float rightHeight = terrain.GetHeight(uv.x + 0.01f / terrain.size.x, uv.y);
            float forwardHeight = terrain.GetHeight(uv.x, uv.y + 0.01f / terrain.size.y);
            
            Vector3 right =   Vector3(0.01f, rightHeight - height, 0);
            Vector3 forward = Vector3(0, forwardHeight - height,0.01f);
            
            dest.Normal = forward.Cross(right);
            */
            //world->TransformPositionAffine(dest.Position, dest.Position);
            //world->TransformVectorAffine(source.Normal, dest.Normal);
            //dest.Normal = source.Normal;
            //dest.Color = source.Color;
            dest.TextureCoords.x = uv.x * terrain.uvScale;
            dest.TextureCoords.y = uv.y * terrain.uvScale;
            
        }
        
        for (size_t v = 0; v<trianglesSize; v++) {
            renderer.triangles[renderer.triangleIndex++] = (index + mesh->triangles[v]);
        }
        
        chunks++;
        
    }
    else {
        scale *= 0.5f;
        level--;
        Draw(terrain, frustum, camPosition, u1, v1, halfU, halfV, scale, level, visibleObject, mesh, renderer);
        Draw(terrain, frustum, camPosition, halfU, v1, u2, halfV, scale, level, visibleObject, mesh, renderer);
        Draw(terrain, frustum, camPosition, u1, halfV, halfU, v2, scale, level, visibleObject, mesh, renderer);
        Draw(terrain, frustum, camPosition, halfU, halfV, u2, v2, scale, level, visibleObject, mesh, renderer);
    }
}

/*

private void RenderBlock(GraphicsDevice device, float u, float v, float scale, int level, ICamera camera) {
    currentRenderer.SetParameter("TerrainPatchValues", new Vector3(u, v, scale));
    currentRenderer.Effect.CommitChanges();
    base.Render(device, camera);
}


*/






