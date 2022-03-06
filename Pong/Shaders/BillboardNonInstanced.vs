#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords_;
layout (location = 2) in float index;

uniform mat4 modelMat;

//out VS_OUT {
   // vec2 TextureCoords;
//} vs_out;

out vec2 TexCoords;

layout(std140) uniform ViewProj
{
    mat4 viewProjMat;
    mat4 viewMat;
    mat4 projMat;
};
    
    void main()  
    {
        vec4 newPos = viewMat * modelMat * vec4(0,0,0,1.0);
        newPos.xy = newPos.xy + aPos.xy;
        newPos = projMat * newPos;
        gl_Position = newPos;
      //  vs_out.TextureCoords = TexCoords_;
        TexCoords = TexCoords_;
    }
