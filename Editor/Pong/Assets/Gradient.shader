
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
        float avg = 1.0;//(textureColor.r + textureColor.g + textureColor.b) / 3.0;
        float spec = 0.4 + clamp(dot(vNormal, vec3(0.0,1.0,0.0)),0.0,1.0) * 0.6;             
        gl_FragColor = vec4(avg * spec, avg * spec, avg * spec,textureColor.a);
    }
}


