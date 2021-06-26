#version 410 core
    layout (location = 0) in vec3 aPos;
    
    out vec3 TexVec;

layout(std140) uniform ViewProj
{
    mat4 viewMat;

    mat4 projMat;
};

uniform mat4 viewMat2;

    void main()
    {
        TexVec = aPos;
        vec4 pos = projMat * viewMat2 * vec4(aPos.x , aPos.y, aPos.z, 1.0);
        gl_Position = pos.xyww;
    }
