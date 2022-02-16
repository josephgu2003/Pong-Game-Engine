#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;
    in vec3 fragPos;

#include "Shaders/Include/Lighting.fs"

layout (std140) uniform Lights
{
    Light light;

    DirLight dirLight;
    
    vec3 viewPos;
};

uniform sampler2D noise;

//uniform samplerCube skyBox;

uniform sampler2D image;

uniform sampler2D normMap;

layout (std140) uniform StopWatch
{
    float time;
};

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);

    void main()

    {
        vec3 norm = cross(normalize(texture(normMap, vec2(TexCoords.x, TexCoords.y + time / 50.0)).rgb * 2.0 - 1.0), vec3(1,0,0));
        
        vec3 fragColor = vec3(0.0);
        
        vec3 viewDir = vec3(viewPos-fragPos);
        
        fragColor += CalcDirLight(dirLight, norm, viewDir);
        FragColor = vec4(fragColor, 1.0);

    }

vec3 CalcDirLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
    
    vec3 dirlightDir = normalize(-dirLight.dir);
    
    float diff = 0;
    float spec = 0;
    
    viewDir = normalize(viewDir);
        
    diff = max(dot(norm,dirlightDir),0.0);
        
    vec3 reflectDir = reflect(-dirlightDir, norm);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec3 reflectDir2 = reflect(-viewDir, norm);
    
    vec3 water = vec3(0.5,0.5,0.6);
    
    vec3 ambient = dirLight.ambient * water ;

    vec3 diffuse = dirLight.diffuse*diff * water;
    
    vec3 specular = (dirLight.specular) * spec;

    return (ambient + diffuse + specular);
    
}
