#include "ShaderTexturedLighting.hpp"
using namespace Pocket;
using namespace std;

void ShaderTexturedLighting::Initialize() {
    
    LightDirection = Vector3(0,-1,-1).Normalized();
	LightColor = Colour::White();
	AmbientColor = Colour(0.2f,0.2f,0.2f,1.0f);

	string vertexShader =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
#endif
		"attribute vec4 Position;\n"
		"attribute vec2 TexCoords;\n"
		"attribute vec4 Color;\n"
		"attribute vec3 Normal;\n"
		"uniform mat4 ViewProjection;\n"
		"uniform mat4 World;\n"
		"varying vec2 DestinationTexCoords;\n"
		"varying vec4 DestinationColor;\n"
		"varying vec3 DestinationNormal;\n"
		"void main(void) {\n"
		"	DestinationTexCoords = TexCoords;\n"
		"	DestinationColor = Color;\n"
		"	DestinationNormal = normalize(Normal * mat3(World));\n"
		"	gl_Position = Position * World * ViewProjection;\n"
		"}\n";
	
	string fragmentShader =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
        "varying vec2 DestinationTexCoords;"
        "varying vec4 DestinationColor;"
        "varying vec3 DestinationNormal;"
#endif
#ifdef OPENGL_EMBEDDED
		"precision mediump float;"
		"varying lowp vec2 DestinationTexCoords;"
		"varying lowp vec4 DestinationColor;"
		"varying lowp vec3 DestinationNormal;"
#endif
		"uniform vec3 LightDirection;"
		"uniform vec4 LightColor;"
		"uniform vec4 AmbientColor;"
		"uniform sampler2D Texture;"
		"void main(void) {"
		"   float lightIntensity = max(dot(LightDirection, DestinationNormal), 0.0); "
        "	gl_FragColor = texture2D(Texture, DestinationTexCoords) * DestinationColor * (AmbientColor + LightColor * lightIntensity);"
		"}";

	if (!shader.CreateShaders(vertexShader, fragmentShader)) return;

	positionSlot = shader.CreateAttribVariable("Position");
	textureCoordSlot = shader.CreateAttribVariable("TexCoords");
	colorSlot = shader.CreateAttribVariable("Color");
	normalSlot = shader.CreateAttribVariable("Normal");

	viewProjectionSlot = shader.CreateUniformVariable("ViewProjection");
	worldSlot = shader.CreateUniformVariable("World");
	textureSlot = shader.CreateUniformVariable("Texture");
	lightDirectionSlot = shader.CreateUniformVariable("LightDirection");
	lightColorSlot = shader.CreateUniformVariable("LightColor");
	ambientColorSlot = shader.CreateUniformVariable("AmbientColor");
    
    vertexStride = sizeof(Vertex);
    
    lastTexture = 0;
}

void ShaderTexturedLighting::OnStart(float* viewProjection) {
    glUniformMatrix4fv(viewProjectionSlot, 1, GL_FALSE, viewProjection);
    
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(textureCoordSlot, 2, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)12);
	glVertexAttribPointer(colorSlot, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
	glVertexAttribPointer(normalSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)24);
    
    glUniform1i(textureSlot, 0);
    
    glUniform3f(lightDirectionSlot, -LightDirection.x, -LightDirection.y, -LightDirection.z);
	glUniform4f(lightColorSlot, LightColor.R(), LightColor.G(), LightColor.B(), LightColor.A());
	glUniform4f(ambientColorSlot, AmbientColor.R(), AmbientColor.G(), AmbientColor.B(), AmbientColor.A());
    
    lastTexture = 0;
}

void ShaderTexturedLighting::Apply(VisibleObject *visibleObject) {
    glUniformMatrix4fv(worldSlot, 1, GL_FALSE, visibleObject->transform->World.GetValue()->GetGlMatrix());
    
    if (visibleObject->texture!=lastTexture) {
        lastTexture = visibleObject->texture;
        glBindTexture(GL_TEXTURE_2D, lastTexture->Texture().GetHandle());
    }
}

/*
void ShaderTexturedLighting::ApplyWorld(float* world) {
	glUniformMatrix4fv(worldSlot, 1, GL_FALSE, world);
}

void ShaderTexturedLighting::ApplyTexture(Pocket::Texture *texture) {
    glBindTexture(GL_TEXTURE_2D, texture ? texture->GetHandle() : 0);
	glUniform1i(textureSlot, 0);
}

void ShaderTexturedLighting::OnBegin() {
	
}
*/