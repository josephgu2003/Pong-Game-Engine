#version 410 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 Normals_;
    layout (location = 2) in vec2 TexCoords_;
    layout (location = 3) in vec3 Tangent;
    layout (location = 4) in vec3 Bitangent;
    
    out vec2 TexCoords;
    out vec2 TexCoordsLarger;
    out vec3 Normals;
    out vec3 fragPos;

layout(std140) uniform ViewProj
{
    mat4 viewProjMat;

};

out VS_OUT {
    vec3 fragPos;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;


struct Light {
vec3 pos;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};


struct DirLight {
vec3 dir;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

layout (std140) uniform Lights
{
   Light light;

    DirLight dirLight;
    
    vec3 viewPos;
};


    void main()
    {
        gl_Position =  viewProjMat * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        fragPos = aPos; 
        TexCoords = TexCoords_;
        TexCoordsLarger = vec2(mod(sin(5.0*fragPos.x),1.0),mod(sin(5.0*fragPos.z),1.0));
        Normals = Normals_;
        
        Functions::setupTanSpaceNormalsNoModelMat();
    }

// for terrain
