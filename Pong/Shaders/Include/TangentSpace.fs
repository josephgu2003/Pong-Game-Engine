#include "Shaders/Include/Lighting.fs"

struct TangentSpaceInfo {
    vec3 fragPos;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    
    Light tangentLight;
};

void setupTanSpaceNormals(in mat4 modelMat_, in vec3 Tangent, in vec3 Bitangent, in vec3 Normals_, out TangentSpaceInfo tangentspaceinfo, in DirLight dirLight, Light light, in vec3 viewPos, in vec3 aPos) {
    vec3 T = normalize(vec3(modelMat_ * vec4(Tangent,   0.0)));
    vec3 B = normalize(vec3(modelMat_ * vec4(Bitangent, 0.0)));
    vec3 N = normalize(vec3(modelMat_ * vec4(Normals_,    0.0)));
    tangentspaceinfo.fragPos =  vec3(modelMat_*vec4(aPos,1.0));
    mat3 TBN = transpose(mat3(T, B, N));
    tangentspaceinfo.TangentLightDir = TBN * dirLight.dir;
    tangentspaceinfo.TangentViewPos = TBN * viewPos;
    tangentspaceinfo.TangentFragPos = TBN * tangentspaceinfo.fragPos;
    
    tangentspaceinfo.tangentLight = light;
    tangentspaceinfo.tangentLight.pos = TBN * light.pos;
}

void setupTanSpaceNormals(in vec3 Tangent, in vec3 Bitangent, in vec3 Normals_, out TangentSpaceInfo tangentspaceinfo, in DirLight dirLight, Light light, in vec3 viewPos, in vec3 aPos) {
    vec3 T = normalize(Tangent);
    vec3 B = normalize(Bitangent);
    vec3 N = normalize(Normals_);
    tangentspaceinfo.fragPos = aPos;
    mat3 TBN = transpose(mat3(T, B, N));
    tangentspaceinfo.TangentLightDir = TBN * dirLight.dir;
    tangentspaceinfo.TangentViewPos = TBN * viewPos;
    tangentspaceinfo.TangentFragPos = TBN * tangentspaceinfo.fragPos;
    
    tangentspaceinfo.tangentLight = light;
    tangentspaceinfo.tangentLight.pos = TBN * light.pos;
}
