#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords_;
layout (location = 2) in vec3 Displacement;
layout (location = 3) in float duration_;

//out VS_OUT {
   // vec2 TextureCoords;
//} vs_out;

out vec2 TexCoords;
out float duration;

    uniform float size;

layout(std140) uniform ViewProj
{
    mat4 viewProjMat;
};
    
    void main()
    {
        gl_Position =  viewProjMat * vec4(Displacement.xyz, 1.0);
      //  vs_out.TextureCoords = TexCoords_;
        gl_PointSize = 1600.0*size / gl_Position.z; // perspective? w coord?
        TexCoords = TexCoords_;
        duration = duration_;
    }
