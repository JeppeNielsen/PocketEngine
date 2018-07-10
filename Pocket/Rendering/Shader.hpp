//
//  Shader.h
//  Shaders
//
//  Created by Jeppe Nielsen on 23/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "OpenGL.hpp"
#include <vector>
#include "Vertex.hpp"
#include "VertexRenderer.hpp"
#include "VertexMesh.hpp"
#include "GameObject.hpp"
#include "Matrix4x4.hpp"
#include <algorithm>

namespace Pocket {

#define SHADER_SOURCE(...) #__VA_ARGS__

static const std::string ViewProjectionUniformName = "ViewProjection";

class IShader {
public:
    virtual ~IShader() { }
    virtual bool Load(std::string vertexShader, std::string fragmentShader) = 0;
};

template<class V>
class Shader : public IShader {
public:
    Shader() : viewProjectionUniform(-1), shaderProgram(0), vertexShader(0), fragmentShader(0) { }
    ~Shader() { Clear(); }
    
    void Clear() {
        if (vertexShader) glDeleteShader(vertexShader);
        if (fragmentShader) glDeleteShader(fragmentShader);
        if (shaderProgram) glDeleteProgram(shaderProgram);
        uniforms.clear();
        textureUniforms.clear();
        attributes.clear();
    }

    bool Load(std::string vertexShader, std::string fragmentShader) override {
        Clear();
        
#ifdef IPHONE
        vertexShader =
        "precision highp float;"
        "precision highp int;"
        + vertexShader;
        
        fragmentShader =
        "precision highp float;"
        "precision highp int;"
        "precision lowp sampler2D;"
        + fragmentShader;
#elif EMSCRIPTEN
         vertexShader =
        "precision highp float;"
        "precision highp int;"
        + vertexShader;
        
        fragmentShader =
        "precision highp float;"
        "precision highp int;"
        "precision lowp sampler2D;"
        + fragmentShader;
#elif RASPBERRY_PI
         vertexShader =
        "precision highp float;"
        "precision highp int;"
        + vertexShader;
        
        fragmentShader =
        "precision highp float;"
        "precision highp int;"
        "precision lowp sampler2D;"
        + fragmentShader;
        
#elif WIN32
		vertexShader =
			"precision highp float;"
			"precision highp int;"
			+ vertexShader;

		fragmentShader =
			"precision highp float;"
			"precision highp int;"
			"precision lowp sampler2D;"
			+ fragmentShader;
#endif

        
        
        
        description = V::Description;
        
        if (!CreateShaders(vertexShader, fragmentShader)) {
            return false;
        }
        
        //std::cout << "Shader::Initialize()"<<std::endl;
        
        ASSERT_GL(glUseProgram(shaderProgram));
        
        if (!FindAttributes()) {
            return false;
        }
        
        if (!FindUniforms()) {
            return false;
        }
         
        return true;
    }
    
    std::string name;
    
    void Use() {
        ASSERT_GL(glUseProgram(shaderProgram));
        for (auto& a : attributes) {
            ASSERT_GL(glVertexAttribPointer(a.glAttribute, a.size, a.type, a.normalized, description.Stride(), (GLvoid*)(size_t)a.offset));
        }
        for (auto& a : attributes) {
            ASSERT_GL(glEnableVertexAttribArray(a.glAttribute));
        }
       // //std::cout<<"Using shader : " << name<<"  shader program " << shaderProgram<<" attributes:  " << attributes.size()<< std::endl;
       
        for (size_t i=0; i<textureUniforms.size(); ++i) {
            ASSERT_GL(glUniform1i(textureUniforms[i].location, i));
        }
    }
    
    template<class T>
    bool SetValue(const T& value) {
        GLenum uniformType = ShaderVariableType<T>::GetType();
        int counter = 0;
        
        for(auto& uniform : uniforms) {
            if (uniform.type == uniformType) {
                counter++;
            }
        }
        if (counter==0) {
            //std::cout << "No matching shader value found " << std::endl;
            return false;
        } else if (counter>1) {
            //std::cout << "Two or more matching shader values found: " << std::endl;
            for(auto uniform : uniforms) {
                if (uniform.type == uniformType) {
                    //std::cout << uniform.name << std::endl;
                }
            }
            //std::cout<<std::endl;
            return false;
        }
        for(auto& uniform : uniforms) {
            if (uniform.type == uniformType) {
                ASSERT_GL(glUseProgram(shaderProgram));
                ShaderVariableType<T>::SetValue(uniform.location, value);
                break;
            }
        }
        return true;
    }
    
    template<class T>
    bool SetValue(std::string name, const T& value) {
    
        auto uniform = std::find_if(uniforms.begin(), uniforms.end(), [&name] (const Uniform& u) {
            return u.name == name;
        });
    
        if (uniform == uniforms.end()) {
            //std::cout << "No shader variable with name: '"<<name<<"' found, valid names are:"<<std::endl;
            for(auto u : uniforms) {
                //std::cout<<u.name<<std::endl;
            }
            return false;
        }
        
        GLenum uniformType = ShaderVariableType<T>::GetType();
        if (uniformType!=uniform->type) {
            //std::cout << " Shader variable types mismatch "<< std::endl;
            return false;
        }
        ASSERT_GL(glUseProgram(shaderProgram));
        ShaderVariableType<T>::SetValue(uniform->location, value);
        return true;
    }

private:

    bool FindAttributes() {
        GLint numberOfAttributes;
        ASSERT_GL(glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &numberOfAttributes));
        
        attributes.clear();
        
        //std::cout << "Attributes: "<< std::endl;
        
        for (int i=0; i<numberOfAttributes; i++) {
            GLchar name[256];
            GLsizei length;
            GLint size;
            GLenum type;
            
            ASSERT_GL(glGetActiveAttrib(shaderProgram, (GLuint)i, sizeof(name), &length, &size, &type, &name[0]));
            
            std::string attributeName = std::string(name);
            
            auto it = description.Attributes().find(attributeName);
            if (it == description.Attributes().end()) {
            
                //std::cout<< "Vertex attribute not found: "<< attributeName << std::endl;
                //std::cout<< "Valid vertex attributes are: "<< std::endl;
                
                //for(auto a : description.Attributes()) {
                    //std::cout<<a.first<<std::endl;
                //}
                
                return false;
            }
            
            VertexAttribute attribute = it->second;
            attribute.glAttribute = glGetAttribLocation(shaderProgram, attributeName.c_str());
            attributes.push_back(attribute);
            //std::cout << attributeName << std::endl;
        }
        
        //std::cout << "End attributes: "<< std::endl;
        
        return true;
    }
    
    bool FindUniforms() {
        viewProjectionUniform = -1;
        GLint numberOfUniforms;
        //std::cout<<"Uniforms : " << std::endl;
        ASSERT_GL(glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &numberOfUniforms));
        for (int i=0; i<numberOfUniforms; i++) {
            GLchar name[256];
            GLsizei length;
            GLint size;
            GLenum type;
            ASSERT_GL(glGetActiveUniform(shaderProgram, (GLuint)i, sizeof(name), &length, &size, &type, &name[0]));
            std::string uniformName = std::string(name);
            Uniform uniform = { uniformName, glGetUniformLocation(shaderProgram, uniformName.c_str()), type };
            uniforms.push_back( uniform );
            if (type == GL_SAMPLER_2D) {
                textureUniforms.push_back( uniform );
                //std::cout<<uniformName << " type: " <<type << std::endl;
            }
        
            
            
            if (uniformName == ViewProjectionUniformName) {
                viewProjectionUniform = i;
            }
        }
        //std::cout<<"End uniforms: " << textureUniforms.size() << std::endl;
        return true;
    }
    

    bool CreateShaders(std::string vertexShaderSource, std::string fragmentShaderSource) {
        vertexShader = CreateShader(vertexShaderSource, false);
        if (!vertexShader) return false;
        fragmentShader = CreateShader(fragmentShaderSource, true);
        if (!fragmentShader) return false;
        
        shaderProgram = glCreateProgram();
        ASSERT_GL(glAttachShader(shaderProgram, vertexShader));
        ASSERT_GL(glAttachShader(shaderProgram, fragmentShader));
        ASSERT_GL(glLinkProgram(shaderProgram));
        
        GLint linkSuccess;
        ASSERT_GL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess));
        if (linkSuccess == GL_FALSE) {
            GLchar messages[256];
            ASSERT_GL(glGetProgramInfoLog(shaderProgram, sizeof(messages), 0, &messages[0]));
            std::cout<<messages<<std::endl;
            return false;
        }
        
        return true;
    }

    GLuint CreateShader(std::string shaderSource, bool isFragment) {
        if (shaderSource=="") return 0;
        
        GLuint shader = glCreateShader(isFragment ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER);
        ASSERT_GL();
        const char* shaderSourceGL = shaderSource.c_str();
        int length = (int)shaderSource.length();
        ASSERT_GL(glShaderSource(shader, 1, &shaderSourceGL, &length));
        ASSERT_GL(glCompileShader(shader));
        
        GLint compileSuccess;
        ASSERT_GL(glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess));
        if (compileSuccess == GL_FALSE) {
            GLchar messages[256];
            glGetShaderInfoLog(shader, sizeof(messages), 0, &messages[0]);
            std::cout<<messages<<std::endl;
            return 0;
        }
        
        return shader;
    }

    VertexDescription<V> description;
    
    typedef std::vector<VertexAttribute> Attributes;
    Attributes attributes;
    
    struct Uniform {
        std::string name;
        GLint location;
        GLenum type;
    };
    
    typedef std::vector<Uniform> Uniforms;
    Uniforms uniforms;
    Uniforms textureUniforms;
    
    GLuint shaderProgram;
    GLuint vertexShader;
    GLuint fragmentShader;
    int viewProjectionUniform;
    
public:

    virtual void SetViewProjection(const float* viewProjection) {
        if (viewProjectionUniform == -1) return;
        ASSERT_GL(glUniformMatrix4fv(uniforms[viewProjectionUniform].location, 1, GL_FALSE, viewProjection));
    }   

    virtual void RenderObject(Pocket::VertexRenderer<V>& renderer, const typename Pocket::VertexMesh<V>::Vertices& vertices, const Pocket::IVertexMesh::Triangles& triangles, const Pocket::Matrix4x4& world);
};
}
