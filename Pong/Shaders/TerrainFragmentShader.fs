#version 410 core
layout (location = 0) out vec4 FragColor;
//layout (location = 1) out vec4 BrightColor;


#include "Shaders/Include/TangentSpace.fs"


    in vec2 TexCoords;
in vec2 TexCoordsLarger;
in vec3 Normals; 
    in vec3 fragPos;

in TangentSpaceInfo tanspaceinfo; 

layout (std140) uniform Lights
{
    Light light;

    DirLight dirLight;
    
    vec3 viewPos;
};

uniform sampler2D noise;

uniform sampler2D diffuse;

uniform sampler2D voronoi;

uniform sampler2D normMap;

layout (std140) uniform StopWatch
{
    float time;
};

layout (std140) uniform DistanceFog
{
    float fogDensity;
    float fogGradient;
    float frustrumNear;
    float frustrumFar;
    vec3 fogColor;
};


vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir);

void applyDistanceFog(inout vec3 color);

    void main()

    {
        vec3 norm = texture(normMap, 2.0*TexCoords).rgb;
          // transform normal vector to range [-1,1]
        norm = normalize(norm * 2.0 - 1.0);
        
        vec3 fragColor = vec3(0.0);  
        
        vec3 viewDir = vec3(tanspaceinfo.TangentViewPos-tanspaceinfo.TangentFragPos);
        vec3 dirLightDir = normalize(-tanspaceinfo.TangentLightDir);
        
        vec3 snowamb = vec3(1.0,1.0,1.0);
        vec3 snow = vec3(0.7,0.7,1.0);
        
        
        float sparkle = 1.0 - 0.5 * texture(voronoi, vec2(50.0, 50.0) * TexCoords).r - 0.5 * texture(voronoi, vec2(50.0, 50.0) * TexCoordsLarger).r;
        sparkle = smoothstep(0.35, 0.45, sparkle);
        
        vec3 snowspec = sparkle * 3.4 * vec3(1.0,1.0,1.0);
 
        fragColor += CalcDirLight(dirLight, dirLightDir, norm, viewDir, snowamb, snow, snowspec);
        fragColor += CalcPointLight(tanspaceinfo.tangentLight, tanspaceinfo.TangentFragPos, norm, viewDir, snowamb, snow, snowspec);
        
        fragColor.rgb += sparkle * 3.0 * dirLight.diffuse;
         
        applyDistanceFog(fragColor);
        
        FragColor = vec4(fragColor, 1);
       /**
        float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
           if(brightness > 0.8)
               BrightColor = 0.2*vec4(1.0,1.0,1.0,1.0);
           else
              BrightColor = vec4(0.0, 0.0, 0.0, 1.0);**/
    }

/**vec3 CalcDirLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
    
    vec3 dirlightDir = normalize(-tanspaceinfo.TangentLightDir);
    
    float diff = 0;
    float spec = 0;
        
    diff = max(dot(norm,dirlightDir),0.0);
        
    vec3 reflectDir = reflect(-dirlightDir, norm);
    viewDir = normalize(viewDir);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 30);
    
    vec3 snow = vec3(0.8,0.8,0.8);
    //snow = mix(snow, texture(diffuse, TexCoordsLarger).rgb, 0.5);
    vec3 snowamb = vec3(0.6,0.8,1.0);
    vec3 ambient = dirLight.ambient * snowamb ;

    vec3 diffuse = dirLight.diffuse * (snow) * diff;
    
    vec3 specular = dirLight.specular * spec*0.0;

    return (ambient+diffuse+specular);
}**/

 
void applyDistanceFog(inout vec3 color) {
    float z = 2.0*gl_FragCoord.z-1.0;
    z = (2.0 * frustrumNear * frustrumFar) / (frustrumFar + frustrumNear - z * (frustrumFar - frustrumNear));
    float fogFactor = exp(-pow(z*fogDensity,fogGradient));
    color = mix(fogColor , color,  fogFactor);
}
