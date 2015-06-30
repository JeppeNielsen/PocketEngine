#include "DeferredLight.hpp"
#include "VisibleObject.hpp"
using namespace Pocket;
using namespace std;

void DeferredLight::Initialize() {

    vertexStride = sizeof(Vertex);
    
	string vertexShaderBatching =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
#endif
        "attribute vec4 Position;\n"
        "uniform mat4 ViewProjection;\n"
        "void main(void) {\n"
        "   gl_Position = Position * ViewProjection;\n"
        "}\n";
    
	string fragmentShader =
#ifdef OPENGL_DESKTOP
        "#version 120 \n"
        "uniform vec2 viewport;"
#else
        "precision highp float;"
        "uniform highp vec2 viewport;"
#endif
        "uniform mat4 InverseProjection;\n"
        "uniform mat4 View;\n"
		"uniform sampler2D colorBuffer;"
        "uniform sampler2D normalBuffer;"
        "uniform sampler2D depthBuffer;"
        "uniform vec3 lightColor;"
        "uniform vec3 lightPosition;"
        "uniform float lightSize;"
        "uniform float lightBrightness;"
        "void main(void) {"
        "   vec2 tex_coord = gl_FragCoord.xy/viewport;"
        "   vec3 color = texture2D(colorBuffer, tex_coord).rgb;    "
        "   vec3 normal = texture2D(normalBuffer, tex_coord).rgb;   "
        "   float depth = texture2D(depthBuffer, tex_coord).r;   "
        "   normal = normal * 2.0 - 1.0; "
        //"   float lightAmount = clamp(dot(normalize(vec3(1,-1,1)), normal),0.0,1.0); "
    
            "vec4 view_pos = vec4(tex_coord*2.0-1.0, depth*2.0 - 1.0,1.0);"
    "view_pos = view_pos * InverseProjection;"
    "view_pos /= view_pos.w;"
    "view_pos = view_pos * View; "
    "vec3 light_dir = lightPosition - view_pos.xyz;"
    "float dist = length(light_dir);"
    "float attenuation = 1.0 - pow( clamp(dist/lightSize, 0.0, 1.0), 2.0);"
    "light_dir = normalize(light_dir);"
    "float n_dot_l = clamp(dot(light_dir, normal), 0.0, 1.0);"
    "vec3 diffuse = lightColor * n_dot_l;"
    "vec3 final_lighting = attenuation * (diffuse) * lightBrightness;"
    "gl_FragColor = vec4(final_lighting * color,1.0);"
        //"	gl_FragColor = vec4(color.r,color.g,color.b,1) * lightAmount;"
        "}";
    
        //"	gl_FragColor = texture2D(Texture, DestinationTexCoords) * DestinationColor;"
    
    if (!shaderBatching.CreateShaders(vertexShaderBatching, fragmentShader)) return;
    
    positionSlot = shaderBatching.CreateAttribVariable("Position");
    
	viewProjectionSlot = shaderBatching.CreateUniformVariable("ViewProjection");
	colorBuffer = shaderBatching.CreateUniformVariable("colorBuffer");
    normalBuffer = shaderBatching.CreateUniformVariable("normalBuffer");
    depthBuffer = shaderBatching.CreateUniformVariable("depthBuffer");
    viewport = shaderBatching.CreateUniformVariable("viewport");
    lightColor = shaderBatching.CreateUniformVariable("lightColor");
    lightPosition = shaderBatching.CreateUniformVariable("lightPosition");
    lightSize = shaderBatching.CreateUniformVariable("lightSize");
    lightBrightness = shaderBatching.CreateUniformVariable("lightBrightness");
    inverseProjection = shaderBatching.CreateUniformVariable("InverseProjection");
    viewSlot = shaderBatching.CreateUniformVariable("View");
}

void DeferredLight::OnInitBatch(const VisibleObject *visibleObject) {
    
    ASSERT_GL( glUniformMatrix4fv(viewProjectionSlot, 1, GL_FALSE, viewProjection));
    ASSERT_GL(glUniform1i(colorBuffer, 0));
    ASSERT_GL(glUniform1i(normalBuffer, 1));
    ASSERT_GL(glUniform1i(depthBuffer, 2));
    
    ASSERT_GL(glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, vertexStride, (GLvoid*)0));
}

void DeferredLight::SetViewPort(Pocket::Vector2 viewportSize) {
    float viewportFloats[] = { viewportSize.x, viewportSize.y };
    ASSERT_GL(glUniform2fv(viewport, 1, viewportFloats));
}

void DeferredLight::SetLightProperties(Pocket::Vector3 position, Pocket::Colour color, float size, float brightness) {
    ASSERT_GL(glUniform3f(lightColor, color.R(), color.G(), color.B()));
    ASSERT_GL(glUniform3f(lightPosition, position.x, position.y, position.z));
    ASSERT_GL(glUniform1f(lightSize, size));
    ASSERT_GL(glUniform1f(lightBrightness, brightness));
}

void DeferredLight::SetInverseProjection(const float* invProjection, const float* view) {
    ASSERT_GL( glUniformMatrix4fv(inverseProjection, 1, GL_FALSE, invProjection));
    ASSERT_GL( glUniformMatrix4fv(viewSlot, 1, GL_FALSE, view));
}
