#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
    in vec2 TexCoords;

    uniform sampler2D noise;
uniform sampler2D gradient;

uniform float brightness;
uniform float duration;
uniform vec3 color;

layout (std140) uniform StopWatch
{
    float time;
};

void main()
{
   // float alpha = clamp(TexCoords.y - duration, 0.0, 1.0) * texture(alphaMap, vec2(0.001,0.001) * TexCoords + vec2(0, 0.1 * time)).r * texture(gradient, TexCoords).r * duration;
  
    FragColor.a = 1.0;
    FragColor.rgb = 5.0  *brightness * color * clamp(texture(noise, vec2(0.01, 0.005) * (TexCoords + vec2(0.0, 2.0 * time))).r - 0.5, 0.0, 1.0) * texture(gradient, TexCoords).r * clamp(1.0 - 2.0 * abs(0.5 - duration), 0.0, 1.0);
    FragColor.rgb = FragColor.rgb * FragColor.rgb;
    
 //   FragColor.rgb =  color * clamp(texture(alphaMap, vec2(0.04, 0.01) * TexCoords + vec2(0.0, -0.01 * duration)).r - 0.4, 0.0, 1.0);
    
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
       if(brightness > 10.0)
           BrightColor = vec4(FragColor.rgb, 1.0);
       else
          BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
