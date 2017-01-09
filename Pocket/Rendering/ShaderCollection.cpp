//
//  ShaderCollection.cpp
//  Shaders
//
//  Created by Jeppe Nielsen on 10/07/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "ShaderCollection.hpp"

using namespace Pocket;

void ShaderCollection::Initialize() {

    {
        Colored.Load(
			SHADER_SOURCE(
				attribute vec4 Position;                   
				attribute vec4 Color;                      
				uniform mat4 ViewProjection;               
				varying vec4 vColor;                       
				void main() {                              
				   vColor = Color;                         
					gl_Position = Position * ViewProjection;
				}                                          
			)
            ,
			SHADER_SOURCE(
				varying vec4 vColor;                       
				void main() {                              
					gl_FragColor = vColor;
				}
			)
        );
    }

    {
        Textured.Load(
			SHADER_SOURCE(
				attribute vec4 Position;                   
				attribute vec2 TexCoords;                  
				attribute vec4 Color;                      
				uniform mat4 ViewProjection;               
				varying vec2 vTexCoords;                   
				varying vec4 vColor;                       
				void main() {                              
					vTexCoords = TexCoords;                 
				   vColor = Color;                         
					gl_Position = Position * ViewProjection;
				}                                          
			)
            ,
			SHADER_SOURCE(
				varying vec2 vTexCoords;                   
				varying vec4 vColor;                       
				uniform sampler2D Texture;                 
				void main() {                              
					gl_FragColor = texture2D(Texture, vTexCoords) * vColor;
				}
			)
        );
    }
    
    {
        LitColored.Load(
			SHADER_SOURCE(
				attribute vec4 Position;                   
				attribute vec4 Color;                      
				attribute vec3 Normal;                     
				uniform mat4 ViewProjection;               
				varying vec4 vColor;                       
				varying vec3 vNormal;                      
				void main() {                              
				   vColor = Color;                         
				   vNormal = Normal;                       
					gl_Position = Position * ViewProjection;
				}                                          
			)
            ,
			SHADER_SOURCE(
				varying vec4 vColor;                       
				varying vec3 vNormal;                      
				uniform vec3 LightDirection;               
				uniform vec4 AmbientLight;                 
				void main() {                              
				   float n = clamp(dot(LightDirection, vNormal),0.0,1.0);              
				   vec4 color = (AmbientLight + vColor * n);
					gl_FragColor = vec4(color.r, color.g, color.b, vColor.a);
				}
			)
        );
        LitColored.SetValue("LightDirection", Vector3(1,1,1).Normalized());
        LitColored.SetValue("AmbientLight", Colour(0.2f));
    }

    {
        LitTextured.Load(
			SHADER_SOURCE(
				attribute vec4 Position;                   
				attribute vec2 TexCoords;                  
				attribute vec4 Color;                      
				attribute vec3 Normal;                     
				uniform mat4 ViewProjection;               
				varying vec2 vTexCoords;                   
				varying vec4 vColor;                       
				varying vec3 vNormal;                      
				void main() {                              
					vTexCoords = TexCoords;                 
				   vColor = Color;                         
				   vNormal = Normal;                       
					gl_Position = Position * ViewProjection;
				}                                          
			)
            ,
			SHADER_SOURCE(
				varying vec2 vTexCoords;                   
				varying vec4 vColor;                       
				varying vec3 vNormal;                      
				uniform vec3 LightDirection;               
				uniform vec4 AmbientLight;                 
				uniform sampler2D Texture;                 
				void main() {                              
				   float n = clamp(dot(LightDirection, vNormal),0.0,1.0);   
				   vec4 textureColor = texture2D(Texture, vTexCoords);      
				   vec4 color =  textureColor * (AmbientLight + vColor * n);             
					gl_FragColor = vec4(color.r, color.g, color.b, textureColor.a * vColor.a);
				}
			)
        );
        LitTextured.SetValue("LightDirection", Vector3(1,1,1).Normalized());
        LitTextured.SetValue("AmbientLight", Colour(0.2f));
    }



}



