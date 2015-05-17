#include "DeferredShader.hpp"
#include "VisibleObject.hpp"
using namespace Pocket;
using namespace std;

void DeferredShader::Initialize() {

    vertexStride = sizeof(Vertex);
    
	string vertexShaderBatching =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
#endif
        "attribute vec4 Position;\n"
        "attribute vec2 TexCoords;\n"
        "attribute vec4 Color;\n"
        "attribute vec3 Normal;\n"
        "uniform mat4 ViewProjection;\n"
        "uniform mat4 View;\n"
        "varying vec2 DestinationTexCoords;\n"
        "varying vec4 DestinationColor;\n"
        "varying vec3 DestinationNormal;\n"
        "void main(void) {\n"
        "	DestinationTexCoords = TexCoords;\n"
        "	DestinationColor = Color;\n"
        "   DestinationNormal = normalize(Normal);\n"
        "	gl_Position = Position * ViewProjection;\n"
        "}\n";
    
	string fragmentShader =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
        "varying vec2 DestinationTexCoords;"
        "varying vec4 DestinationColor;"
        "varying vec3 DestinationNormal;"
#else
        "varying lowp vec2 DestinationTexCoords;"
        "varying lowp vec4 DestinationColor;"
        "varying lowp vec3 DestinationNormal;"
#endif
		"uniform sampler2D Texture;"
		"void main(void) {"
        "	gl_FragData[0] = texture2D(Texture, DestinationTexCoords) * DestinationColor*2.0;"
        "	gl_FragData[1] = vec4(DestinationNormal.x * 0.5+0.5, DestinationNormal.y*0.5+0.5, DestinationNormal.z*0.5+0.5,0);"
        "}";
    
        //"	gl_FragColor = texture2D(Texture, DestinationTexCoords) * DestinationColor;"
    
    if (!shaderBatching.CreateShaders(vertexShaderBatching, fragmentShader)) return;
    
    positionSlot = shaderBatching.CreateAttribVariable("Position");
	textureCoordSlot = shaderBatching.CreateAttribVariable("TexCoords");
	colorSlot = shaderBatching.CreateAttribVariable("Color");
    normalSlot = shaderBatching.CreateAttribVariable("Normal");
    
	viewProjectionSlot = shaderBatching.CreateUniformVariable("ViewProjection");
	textureSlot = shaderBatching.CreateUniformVariable("Texture");
    viewSlot = shaderBatching.CreateUniformVariable("View");
}

void DeferredShader::OnInitBatch(const VisibleObject *visibleObject) {
    
    glUniformMatrix4fv(viewProjectionSlot, 1, GL_FALSE, viewProjection);
    ASSERT_GL(glUniformMatrix4fv(viewSlot, 1, GL_FALSE, viewMatrix));
    glUniform1i(textureSlot, 0);
    
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0);
	glVertexAttribPointer(textureCoordSlot, 2, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)12);
	glVertexAttribPointer(colorSlot, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (GLvoid*)20);
    glVertexAttribPointer(normalSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)24);
}

