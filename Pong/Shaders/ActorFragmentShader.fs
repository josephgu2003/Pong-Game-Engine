#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

#include "Shaders/Include/TangentSpace.fs"

in vec3 Normals;

in vec2 TexCoords;

layout (std140) uniform Lights
{
    Light light;

    DirLight dirLight;
    
    vec3 viewPos;
};

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normMap;
uniform float brightness;


layout (std140) uniform DistanceFog
{
    float fogDensity;
    float fogGradient;
    float frustrumNear;
    float frustrumFar;
    vec3 fogColor;
};

in TangentSpaceInfo tanspaceinfo;

void applyDistanceFog(inout vec3 color) {
   float z = 2.0*gl_FragCoord.z-1.0;
   z = (2.0 * frustrumNear * frustrumFar) / (frustrumFar + frustrumNear - z * (frustrumFar - frustrumNear));
   float fogFactor = exp(-pow(z*fogDensity,fogGradient));
   color = mix(fogColor , color,  fogFactor);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec4 fragDiffuse = texture(diffuse, TexCoords).rgba;
    
    if (fragDiffuse.a <= 0.0) {
        discard;
    }
    
    vec3 viewDir = vec3(tanspaceinfo.TangentViewPos-tanspaceinfo.TangentFragPos);
    float fogFactor = exp(-pow(length(viewDir)*0.03,2));
    vec3 norm = normalize(texture(normMap, TexCoords).rgb * 2.0 - 1.0);
     
    vec3 fragColor = vec3(0.0);
    vec3 diffuseColor = fragDiffuse.rgb; 
    vec3 specColor = vec3(0,0,0);
    fragColor += CalcDirLight(dirLight, norm, viewDir);
    fragColor += CalcPointLight(tanspaceinfo.tangentLight, tanspaceinfo.TangentFragPos, norm, viewDir, diffuseColor, diffuseColor, specColor);
    
    applyDistanceFog(fragColor);

    FragColor = vec4(fragColor, texture(diffuse, TexCoords).a);
    
    float brightnessLvl = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
       if(brightnessLvl > 1.0)
           BrightColor = vec4(0.3*FragColor.rgb, 1.0);
       else
          BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

vec3 CalcDirLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
    
    vec3 dirlightDir = normalize(-tanspaceinfo.TangentLightDir);
     
    float diff = 0;
    float spec = 0;
         
    diff = max(dot(norm,dirlightDir),0.0);
        
    vec3 reflectDir = reflect(-dirlightDir, norm);
    viewDir = normalize(viewDir);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec3 ambient = (vec3(brightness, brightness, brightness)+dirLight.ambient)*vec3(texture(diffuse, TexCoords));

    vec3 diffuse = dirLight.diffuse*diff*vec3(texture(diffuse, TexCoords));
    vec3 specular = dirLight.specular * spec * vec3(texture(specular, TexCoords));

    return (ambient + diffuse);

}
