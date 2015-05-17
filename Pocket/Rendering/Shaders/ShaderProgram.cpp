#include "ShaderProgram.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace Pocket;

ShaderProgram::ShaderProgram() {
    
}

ShaderProgram::~ShaderProgram() {
    
}

void ShaderProgram::Use() {
	glUseProgram(shaderProgram);
	for (ShaderVariables::iterator it=attribVariables.begin(); it!= attribVariables.end(); ++it) {
		glEnableVertexAttribArray(it->second);
	}
}

bool ShaderProgram::CreateShaders(std::string vertexShaderSource, std::string fragmentShaderSource) {
    
	vertexShader = CreateShader(vertexShaderSource, false);
	if (!vertexShader) return false;
	fragmentShader = CreateShader(fragmentShaderSource, true);
	if (!fragmentShader) return false;
    
    shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
    
	GLint linkSuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
	if (linkSuccess == GL_FALSE) {
		GLchar messages[256];
		glGetProgramInfoLog(shaderProgram, sizeof(messages), 0, &messages[0]);
		std::cout<<messages<<std::endl;
		return false;
	}
    
	return true;
}

GLuint ShaderProgram::CreateShader(std::string shaderSource, bool isFragment) {
	if (shaderSource=="") return 0;
    
	GLuint shader = glCreateShader(isFragment ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER);
    
	const char* shaderSourceGL = shaderSource.c_str();
	int length = (int)shaderSource.length();
	glShaderSource(shader, 1, &shaderSourceGL, &length);
	glCompileShader(shader);
    
	GLint compileSuccess;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
	if (compileSuccess == GL_FALSE) {
		GLchar messages[256];
		glGetShaderInfoLog(shader, sizeof(messages), 0, &messages[0]);
		std::cout<<messages<<std::endl;
		return 0;
	}
    
	return shader;
}

GLuint ShaderProgram::CreateAttribVariable(std::string id) {
	GLuint var = glGetAttribLocation(shaderProgram, id.c_str());
	attribVariables[id] = var;
	return var;
}

GLuint ShaderProgram::CreateUniformVariable(std::string id) {
	GLuint var = glGetUniformLocation(shaderProgram, id.c_str());
	uniformVariables[id] = var;
	return var;
}
