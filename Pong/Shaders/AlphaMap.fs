#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
    in vec2 TexCoords;

    uniform sampler2D alphaMap;

uniform float brightness;
uniform vec3 color;

void main()
{
    float alpha = texture(alphaMap,TexCoords).r;
    FragColor.a = alpha;
    FragColor.rgb = alpha*vec3(1,1,1);
    FragColor.rgb = mix(color, FragColor.rgb, alpha);
    FragColor.rgb *= brightness;
    
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
       if(brightness > 0.0)
           BrightColor = vec4(FragColor.rgb, 1.0)*alpha;
       else
          BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
