#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

#include "Shaders/Include/TangentSpace.fs"

    in vec2 TexCoords;
in vec3 Normals;
in vec3 fragPos;

uniform sampler2D alphaMap;

uniform sampler2D gradient;
uniform vec3 color;
 
uniform float brightness;

layout (std140) uniform StopWatch
{
    float time;
};

layout (std140) uniform Lights
{
   Light light;

    DirLight dirLight;
    
    vec3 viewPos;
};


void main()
{
    FragColor.a = 1.0;
    vec3 viewDir = viewPos - fragPos;
    FragColor.rgb = brightness * color * clamp(texture(alphaMap, (TexCoords + vec2(0, -0.1* time)) * vec2(10.0, 1.0)).r - 0.5, 0, 1) * pow(1.0 - abs(dot(normalize(Normals), normalize(viewDir))), 2);
    //FragColor.rgb = vec3(0,0,1) * pow(1.0 - abs(dot(normalize(Normals), normalize(viewDir))), 3);
   // FragColor.rgb = brightness * color * clamp(texture(alphaMap, (TexCoords + vec2(0.01 * time, 0)) * vec2(20.0, 1.0)).r - 0.3, 0, 1);
    FragColor.rgb = FragColor.rgb * FragColor.rgb;

    float brightFactor = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    
       if(brightFactor > 0.0)
           BrightColor = vec4(FragColor.rgb, 1.0);
       else
          BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
