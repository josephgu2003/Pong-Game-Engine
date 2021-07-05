#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normals_;
layout (location = 2) in vec2 TexCoords_;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

layout(std140) uniform ViewProj
{
    mat4 viewMat;

    mat4 projMat;
};

struct Light {
vec3 pos;

vec3 ambient;
vec3 diffuse;
vec3 specular;
    
};


out vec3 Normals;
    
    uniform mat4 modelMat;

out VS_OUT {
    vec3 fragPos;
    vec2 TexCoords;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

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
        gl_Position =  projMat * viewMat * modelMat * vec4(0.005*aPos.x , 0.005*aPos.y , 0.005*aPos.z, 1.0);
        Normals = mat3(transpose(inverse(modelMat))) * Normals_;
        
        vec3 T = normalize(vec3(modelMat * vec4(Tangent,   0.0)));
           vec3 B = normalize(vec3(modelMat * vec4(Bitangent, 0.0)));
           vec3 N = normalize(vec3(modelMat * vec4(Normals_,    0.0)));

        vs_out.fragPos =  vec3(modelMat*vec4(0.005*aPos, 1.0));
        vs_out.TexCoords = TexCoords_;
        
        mat3 TBN = transpose(mat3(T, B, N));
        
        vs_out.TangentLightDir = TBN * dirLight.dir;
        vs_out.TangentViewPos = TBN * viewPos;
        vs_out.TangentFragPos = TBN * vs_out.fragPos;
    }
