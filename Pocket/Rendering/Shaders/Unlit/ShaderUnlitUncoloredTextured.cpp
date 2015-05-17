#include "ShaderUnlitUncoloredTextured.hpp"
#include "VisibleObject.hpp"
using namespace Pocket;
using namespace std;

void ShaderUnlitUncoloredTextured::Initialize() {

    vertexStride = sizeof(Vertex);
    lastTexture = 0;
    
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
        "uniform mat4 ViewProjection;\n"
        "varying vec2 DestinationTexCoords;\n"
        "varying vec4 DestinationColor;\n"
        "void main(void) {\n"
        "	DestinationTexCoords = TexCoords;\n"
        "	DestinationColor = Color;\n"
        "	gl_Position = Position * ViewProjection;\n"
        "}\n";
    
	string fragmentShader =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
        "varying vec2 DestinationTexCoords;"
        "varying vec4 DestinationColor;"
#else
        "varying lowp vec2 DestinationTexCoords;"
        "varying lowp vec4 DestinationColor;"
#endif
		"uniform sampler2D Texture;"
		"void main(void) {"
        "	gl_FragColor = texture2D(Texture, DestinationTexCoords) * DestinationColor;"
		"}";
    
	if (!shader.CreateShaders(vertexShader, fragmentShader)) return;
    if (!shaderBatching.CreateShaders(vertexShaderBatching, fragmentShader)) return;

	positionSlot = shader.CreateAttribVariable("Position");
	textureCoordSlot = shader.CreateAttribVariable("TexCoords");
	colorSlot = shader.CreateAttribVariable("Color");

	viewProjectionSlot = shader.CreateUniformVariable("ViewProjection");
	worldSlot = shader.CreateUniformVariable("World");
	textureSlot = shader.CreateUniformVariable("Texture");
    
    
    positionSlot_batched = shaderBatching.CreateAttribVariable("Position");
	textureCoordSlot_batched = shaderBatching.CreateAttribVariable("TexCoords");
	colorSlot_batched = shaderBatching.CreateAttribVariable("Color");
    
	viewProjectionSlot_batched = shaderBatching.CreateUniformVariable("ViewProjection");
	textureSlot_batched = shaderBatching.CreateUniformVariable("Texture");
    
}

void ShaderUnlitUncoloredTextured::OnStart(float* viewProjection) {
    glUniformMatrix4fv(viewProjectionSlot, 1, GL_FALSE, viewProjection);
    glUniform1i(textureSlot, 0);
    lastTexture = 0;
}

void ShaderUnlitUncoloredTextured::Bind() {
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(textureCoordSlot, 2, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)12);
	glVertexAttribPointer(colorSlot, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
}

void ShaderUnlitUncoloredTextured::Apply(VisibleObject* visibleObject) {
    
    glUniformMatrix4fv(worldSlot, 1, GL_FALSE, visibleObject->transform->World.GetValue()->GetGlMatrix());
    
    if (visibleObject->texture!=lastTexture) {
        lastTexture = visibleObject->texture;
        glBindTexture(GL_TEXTURE_2D, lastTexture->Texture().GetHandle());
    }
}

void ShaderUnlitUncoloredTextured::OnInitBatch(const VisibleObject *visibleObject) {
    
    glUniformMatrix4fv(viewProjectionSlot_batched, 1, GL_FALSE, viewProjection);
    glUniform1i(textureSlot_batched, 0);
    
    glVertexAttribPointer(positionSlot_batched, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(textureCoordSlot_batched, 2, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)12);
	glVertexAttribPointer(colorSlot_batched, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
    
}






