#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords_;
layout (location = 2) in float index;
layout (location = 3) in vec4 Displacement0;
layout (location = 4) in vec4 Displacement1;
layout (location = 5) in vec4 Displacement2;
layout (location = 6) in vec4 Displacement3;
layout (location = 7) in float duration_;

//out VS_OUT {
   // vec2 TextureCoords;
//} vs_out;

out vec2 TexCoords;
out float duration;

layout(std140) uniform ViewProj
{
    mat4 viewProjMat;
    mat4 viewMat;
    mat4 projMat;
};

    
    void main() 
    {
        mat4 Displacement = mat4(Displacement0, Displacement1, Displacement2, Displacement3);
        gl_Position =  viewProjMat *Displacement * vec4(aPos.x, aPos.y, aPos.z, 1.0);
      //  vs_out.TextureCoords = TexCoords_;
        TexCoords = TexCoords_;
        duration = duration_;
    }
