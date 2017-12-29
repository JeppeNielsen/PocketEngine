
{
    attribute vec4 Position;                   
    attribute vec4 Color;      
    attribute vec3 Normal;
    attribute vec2 TexCoords;                      
    uniform mat4 ViewProjection;               
    varying vec4 vColor;
    varying vec3 vNormal;
    varying vec2 vTexCoords;                      
    void main() {     
        vColor = Color;
        vNormal = Normal;
        vTexCoords = TexCoords;                                         
        gl_Position = Position * ViewProjection;
    }
}                                          

{
    varying vec4 vColor;   
    varying vec3 vNormal;
    varying vec2 vTexCoords;
    uniform sampler2D Texture;

    void main() {
        vec4 textureColor = texture2D(Texture, vTexCoords);
        float spec = 0.05 + clamp(dot(vNormal, vec3(0.0,1.0,0)),0.0,1.0) * 0.95;            
        gl_FragColor = vec4(textureColor.r * spec, textureColor.g * spec, textureColor.b * spec,textureColor.a);
    }
}


