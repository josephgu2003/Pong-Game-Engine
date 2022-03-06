#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
    in vec2 TexCoords;

    uniform sampler2D alphaMap;

uniform sampler2D gradient;
uniform vec3 color;
 
uniform float brightness;

layout (std140) uniform StopWatch
{
    float time;
};

void main()
{
    FragColor.a = 1.0;
    //FragColor.rgb = color * (pow(texture(noise, TexCoords * vec2(0.001, 0.4)).r, 2) + 0.2) * texture(gradient, TexCoords).r * (1.3 + sin(1.0 * time + TexCoords.y * 10.0)) * 2.0 * TexCoords.x * brightness;
   // FragColor.rgb = color * (texture(noise, (TexCoords + vec2(0.2 * time + 0.1, 0.0)) * vec2(0.002, 0.05)).r * clamp(texture(noise, (vec2(0.2 * time, 0.002 * time) + TexCoords) * vec2(0.001, 0.24)).r - 0.3, 0.0, 1.0)) * (0.5 - abs(0.5 - TexCoords.x)) * 2.0 * brightness * 2;
    FragColor.rgb = color * clamp(texture(alphaMap, (vec2(0.2 * time, 0.0) + TexCoords) * vec2(0.01, 1.0)).r - 0.2, 0, 1) * TexCoords.x * brightness * texture(gradient, TexCoords).r * 1.5;
    FragColor.rgb = FragColor.rgb * FragColor.rgb;

    float brightFactor = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    
       if(brightFactor > 0.5)
           BrightColor = vec4(FragColor.rgb * 0.5, 1.0);
       else
          BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
