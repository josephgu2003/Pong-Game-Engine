#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normals_;
layout (location = 2) in vec2 TexCoords_;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 boneWeights;

layout(std140) uniform ViewProj
{
    mat4 viewProjMat;
};

struct Light {
vec3 pos;

vec3 ambient;
vec3 diffuse;
vec3 specular;
    
};

const int MAX_BONES = 100;
const int MAX_BONE_WEIGHTS = 4;

out vec3 Normals; 

uniform float size;
    
    uniform mat4 modelMat;

uniform mat3 transposeInverseModelMat;

uniform mat4 boneOffsetMatrices[100];

uniform bool animated;

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

        vec4 Pos = vec4(0.0,0.0,0.0,0.0);
        if (animated == true) {
            mat4 transform = mat4(0.0);
            int z = 0;
            for (int i = 0; i < MAX_BONE_WEIGHTS; i++) {
                if (boneIDs[i] == -1) {
                    continue;
                }
                if (boneIDs[i] >= MAX_BONES) {

                    transform = mat4(1.0);
                    break;
                }
                int x = int(boneIDs[i]);
                mat4 offset = boneOffsetMatrices[x];
                float weight = boneWeights[i];
           
                transform += offset*weight;

                z =1;
            }
            if (z ==0 ) {

                transform = mat4(1.0);
            }
            Pos = transform * vec4(aPos.xyz,1.0);
        } else {
            Pos = vec4(aPos.xyz, 1.0);
        }
         
        gl_Position =  viewProjMat * modelMat * vec4(size*Pos.xyz,1.0);
        
        Normals = Normals_.xyz;
        vec3 T = normalize(vec3(modelMat * vec4(Tangent,   0.0)));
           vec3 B = normalize(vec3(modelMat * vec4(Bitangent, 0.0)));
           vec3 N = normalize(vec3(modelMat * vec4(Normals_,    0.0)));

        vs_out.fragPos =  vec3(modelMat*vec4(aPos,1.0));
        vs_out.TexCoords = TexCoords_;
        
        mat3 TBN = transpose(mat3(T, B, N));
        
        vs_out.TangentLightDir = TBN * dirLight.dir;
        vs_out.TangentViewPos = TBN * viewPos;
        vs_out.TangentFragPos = TBN * vs_out.fragPos;
    }
