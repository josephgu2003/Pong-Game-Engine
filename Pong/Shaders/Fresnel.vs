#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normals_;
layout (location = 2) in vec2 TexCoords_;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

//#include "Shaders/Include/TangentSpace.fs"

    out vec2 TexCoords;
    out vec3 Normals;
    out vec3 fragPos;
    
    uniform mat4 modelMat;
uniform mat3 transposeInverseModelMat;

layout(std140) uniform ViewProj
{
    mat4 viewProjMat;
    mat4 viewMath;
    mat4 projMat;
};


layout (std140) uniform Lights
{
    Light light;

    DirLight dirLight;
    
    vec3 viewPos;
};

out TangentSpaceInfo tanspaceinfo; 

    void main() 
    {
        gl_Position =  viewProjMat * modelMat * vec4(aPos.x, aPos.y,  aPos.z, 1.0);
        TexCoords = TexCoords_;
        fragPos = vec3(modelMat * vec4(aPos.x, aPos.y,   aPos.z, 1.0));
        Normals = transposeInverseModelMat * Normals_;
        
       // setupTanSpaceNormals(Tangent, Bitangent, Normals_, tanspaceinfo, dirLight, light, viewPos, aPos);
    } 



 
