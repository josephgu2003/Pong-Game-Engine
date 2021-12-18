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
        fragColor.rgb = vec3(0,0,0);
        vec3 pos = TexVec;
        float gradient = dot(normalize(pos),normalize(vec3(pos.x,0,pos.z)));
        gradient = acos(gradient) / 1.5;
        fragColor.rgb = mix(fogColor, fragColor.rgb, gradient);
        fragColor.a = 1.0;
        FragColor = fragColor;
    }
