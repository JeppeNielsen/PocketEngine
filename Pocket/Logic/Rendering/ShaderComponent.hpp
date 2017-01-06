//
//  ShaderComponent.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Shader.hpp"

namespace Pocket {
    class ShaderComponent {
    public:
        ShaderComponent() : shader(0) { }
        ~ShaderComponent() { delete shader; }

        void operator=(const ShaderComponent& other) {
            if (other.shader) {
                //shader = other.shader->Clone();
                delete shader;
                shader = 0;
            } else {
                delete shader;
                shader = 0;
            }
        }
        
        template<class Vertex>
        class Shader<Vertex>& GetShader() {
            if (!shader) {
                shader = new Shader<Vertex>();
            }
            Shader<Vertex>& s = *((Shader<Vertex>*)shader);
            return s;
        }

        template<class Vertex>
        const Shader<Vertex>& ConstShader() const {
            return (const Shader<Vertex>&)*((Shader<Vertex>*)shader);
        }
        
        IShader* GetBaseShader() {
            return shader;
        }
        
        void LoadAsset(const std::string& path);
        
    private:
        IShader* shader;
        bool TryParseShader(const std::string& shader, int startOffset, int& startIndex, int& endIndex);
    };
}
