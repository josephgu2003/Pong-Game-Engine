#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

    in vec2 TexCoords;

in float duration;

uniform sampler2D gradient;
uniform vec4 color;

void main()
{
    if (duration < 0.0) discard;
    
    vec4 fragColor = vec4(0,0,0,1);
    fragColor.a = pow(texture(gradient, gl_PointCoord).r, (1.0-duration)) * pow(duration, 0.25);
    fragColor.rgb += pow(duration,1.8)*vec3(8.0,8.0,8.0) * fragColor.a;
    FragColor = fragColor;
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.0)
        BrightColor = 0.2*vec4(color.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
} 

 
