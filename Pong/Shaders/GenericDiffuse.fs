#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

    in vec2 TexCoords;

in float duration;

uniform sampler2D diffuse;

uniform float brightness;


void main()
{
    if (duration < 0.0) discard;
    vec4 color = texture(diffuse, TexCoords);
    color.rgb = brightness * color.rgb;
    FragColor = color;

    BrightColor = vec4(0.1*FragColor.rgb,1.0);
} 

 
