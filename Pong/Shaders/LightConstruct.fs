#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

in float duration;

uniform vec3 color;

uniform float brightness;

uniform sampler2D alphaMap;


uniform int diffuseFrameCounter;

void main()
{
    if (duration < 0.0) discard;
    int counter = diffuseFrameCounter;
    float alpha = texture(alphaMap, TexCoords).r;
   
    
    FragColor = vec4(brightness*color, alpha);
    BrightColor = vec4(0.05*FragColor.rgb,1.0);
} 

 
