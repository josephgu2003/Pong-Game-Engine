#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords_;
layout (location = 2) in float arrayTexId_;
    
    out vec2 TexCoords;
out vec3 Normals;
    out vec3 fragPos;

    uniform mat4 modelMat;


layout(std140) uniform ViewProj
{
    mat4 viewProjMat;
    mat4 viewMath;
    mat4 projMat;
};


    void main()
    {
        gl_Position =  viewProjMat * modelMat * vec4(aPos, 1.0);
        fragPos = vec3(modelMat*vec4(aPos, 1.0));
        TexCoords = TexCoords_;
    }
