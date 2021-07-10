#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords_;

//out VS_OUT {
   // vec2 TextureCoords;
//} vs_out;

out vec2 TexCoords;
    uniform float size;
    
    uniform mat4 modelMat;

layout(std140) uniform ViewProj
{
    mat4 viewMat;

    mat4 projMat;
};
    
    void main()
    {
        gl_Position =  projMat * viewMat * modelMat*vec4(aPos, 1.0);
      //  vs_out.TextureCoords = TexCoords_;
        gl_PointSize = 1600.0*size / gl_Position.z;
        TexCoords = TexCoords_;
    }
