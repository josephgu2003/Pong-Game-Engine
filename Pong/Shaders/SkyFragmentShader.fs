#version 410 core
    out vec4 FragColor;
    in vec3 TexVec;

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
        vec4 fragColor = vec4(0,0,0,0);
        fragColor.rgb = vec3(0,0,0);
        vec3 pos = TexVec;
        float gradient = dot(normalize(pos),normalize(vec3(pos.x,0,pos.z))); // 1 -> 0
        gradient = pow(gradient, 3);
        fragColor.rgb = mix(fragColor.rgb, fogColor, gradient * 0.7 + 0.3);
        fragColor.a = 1.0;
        FragColor = fragColor;
    }
