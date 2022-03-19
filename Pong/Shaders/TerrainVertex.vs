#version 410 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 Normals_;
    layout (location = 2) in vec2 TexCoords_;
    layout (location = 3) in vec3 Tangent;
    layout (location = 4) in vec3 Bitangent;
    
#include "Shaders/Include/TangentSpace.fs"

    out vec2 TexCoords;
    out vec2 TexCoordsLarger;
    out vec3 Normals;
    out vec3 fragPos;

layout(std140) uniform ViewProj
{
    mat4 viewProjMat;
    mat4 viewMat;
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
        gl_Position =  viewProjMat * vec4(aPos.xyz, 1.0);
        fragPos = aPos; 
        TexCoords = TexCoords_;
        TexCoordsLarger = vec2(mod(sin(2.0*fragPos.x),1.0),mod(sin(2.0*fragPos.z),1.0));
        Normals = Normals_;
        
        setupTanSpaceNormals(Tangent, Bitangent, Normals_, tanspaceinfo, dirLight, light, viewPos, aPos);
    }

// for terrain
