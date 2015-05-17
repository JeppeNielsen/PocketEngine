#include "ShaderUnlitUncolored.hpp"
#include "VisibleObject.hpp"
using namespace Pocket;
using namespace std;

void ShaderUnlitUncolored::Initialize() {
    
	string vertexShader =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
#endif
		"attribute vec4 Position;\n"
		"attribute vec4 Color;\n"
		"uniform mat4 ViewProjection;\n"
		"uniform mat4 World;\n"
		"varying vec4 DestinationColor;\n"
		"void main(void) {\n"
		"	DestinationColor = Color;\n"
		"	gl_Position = Position * World * ViewProjection;\n"
		"}\n";
    
string vertexShader_batched =
#ifdef OPENGL_DESKTOP
    "#version 120 \n"
#endif
    "attribute vec4 Position;\n"
    "attribute vec4 Color;\n"
    "uniform mat4 ViewProjection;\n"
    "varying vec4 DestinationColor;\n"
    "void main(void) {\n"
    "	DestinationColor = Color;\n"
    "	gl_Position = Position * ViewProjection;\n"
    "}\n";
    
	string fragmentShader =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
        "varying vec4 DestinationColor;"
#else
        "varying lowp vec4 DestinationColor;"
#endif
		"void main(void) {"
        "	gl_FragColor = DestinationColor;"
		"}";
    
	if (!shader.CreateShaders(vertexShader, fragmentShader)) return;
    if (!shaderBatching.CreateShaders(vertexShader_batched, fragmentShader)) return;

	positionSlot = shader.CreateAttribVariable("Position");
	colorSlot = shader.CreateAttribVariable("Color");

	viewProjectionSlot = shader.CreateUniformVariable("ViewProjection");
	worldSlot = shader.CreateUniformVariable("World");
    
    positionSlot_batched = shaderBatching.CreateAttribVariable("Position");
	colorSlot_batched = shaderBatching.CreateAttribVariable("Color");
    
	viewProjectionSlot_batched = shaderBatching.CreateUniformVariable("ViewProjection");
	worldSlot_batched = shaderBatching.CreateUniformVariable("World");
    
    vertexStride = sizeof(Vertex);
}

void ShaderUnlitUncolored::OnStart(float* viewProjection) {
    glUniformMatrix4fv(viewProjectionSlot, 1, GL_FALSE, viewProjection);
}

void ShaderUnlitUncolored::Bind() {
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(colorSlot, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
}

void ShaderUnlitUncolored::Apply(VisibleObject* visibleObject) {
    glUniformMatrix4fv(worldSlot, 1, GL_FALSE, visibleObject->transform->World.GetValue()->GetGlMatrix());
}

void ShaderUnlitUncolored::OnInitBatch(const Pocket::VisibleObject *visibleObject) {
    
    glUniformMatrix4fv(viewProjectionSlot_batched, 1, GL_FALSE, viewProjection);
    
    glVertexAttribPointer(positionSlot_batched, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(colorSlot_batched, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
}







