#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords_;
layout (location = 2) in float duration_;
layout (location = 3) in mat4 modelMat;


    out vec2 TexCoords;
out float duration;

uniform float size;

layout(std140) uniform ViewProj
{
    mat4 viewMat;

    mat4 projMat;
};
    
    void main()
    {
        gl_Position =  projMat * viewMat * modelMat*vec4(size*aPos, 1.0);
        TexCoords = TexCoords_;
        duration = duration_;
    }
