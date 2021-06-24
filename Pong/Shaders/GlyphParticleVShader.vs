#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords_;

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
        gl_Position =  projMat * viewMat * modelMat*vec4(size*aPos, 1.0);
        TexCoords = TexCoords_;
    }
