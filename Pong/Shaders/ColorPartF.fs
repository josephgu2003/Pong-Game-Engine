#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

    in vec2 TexCoords;

in float duration;

uniform sampler2D gradient;
uniform vec4 color;

void main()
{
    vec4 fragColor = vec4(0,0,0,1);
    fragColor.a = pow(texture(gradient, gl_PointCoord).r, (1.0-duration)) * pow(duration, 0.25);
    fragColor.rgb += 2.0*pow(duration,1.8) * color.rgb * fragColor.a;
    FragColor = fragColor;
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.0)
        BrightColor = vec4(0.2*color.rgb*fragColor.a, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
} 

 
