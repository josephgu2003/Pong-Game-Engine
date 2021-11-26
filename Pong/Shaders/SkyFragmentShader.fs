#version 410 core
    out vec4 FragColor;
    in vec3 TexVec;

    uniform samplerCube skyBox;
    uniform float brightness;

layout (std140) uniform DistanceFog
{
    float fogDensity;
    float fogGradient;
    float frustrumNear;
    float frustrumFar;
    vec3 fogColor;
};

    void main()
    {
        vec4 fragColor = texture(skyBox, TexVec); //brightness
        float gradient = TexVec.y;
        gradient = 1.0 - gradient;
        fragColor.rgb = mix(fragColor.rgb, fogColor, pow(gradient,2));
        fragColor.rgb *= 0.5; 
        fragColor.a = 1.0;
        FragColor = fragColor;
    }
