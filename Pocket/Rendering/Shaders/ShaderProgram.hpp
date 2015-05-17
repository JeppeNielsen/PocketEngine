#pragma once
#include "OpenGL.hpp"
#include <string>
#include <map>


namespace Pocket {
	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();
        
        void Use();
        
        bool CreateShaders(std::string vertexShaderSource, std::string fragmentShaderSource);
        
        GLuint CreateAttribVariable(std::string id);
		GLuint CreateUniformVariable(std::string id);
        
	private:
        
		GLuint shaderProgram;
		GLuint vertexShader;
		GLuint fragmentShader;
        
		typedef std::map<std::string, GLuint> ShaderVariables;
        
		ShaderVariables attribVariables;
		ShaderVariables uniformVariables;
        
		
    private:
		GLuint CreateShader(std::string shaderSource, bool isFragment);
        GLuint CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader);
	};
    
}