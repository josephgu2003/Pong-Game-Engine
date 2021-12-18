#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normals_;
layout (location = 2) in vec2 TexCoords_;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 boneWeights;

out vec3 Normals;
 
out vec2 TexCoords;
 
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

uniform float size;
    
uniform mat4 modelMat;

uniform mat3 transposeInverseModelMat;

uniform mat4 boneOffsetMatrices[100];

uniform bool animated;

out VS_OUT {
    vec3 fragPos;
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
        mat4 modelMat_ = modelMat;
        mat4 transform = mat4(0.0);
        vec4 Pos = vec4(0.0,0.0,0.0,0.0);
        if (animated == true) {
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
            transform = mat4(1.0);
        }
         
        
        gl_Position =  viewProjMat * modelMat_ * vec4(size*Pos.xyz,1.0);
        
        modelMat_ = modelMat_ * transform;
        TexCoords = TexCoords_;
        Normals = Normals_;
        
        Functions::setupTanSpaceNormals();
    }


