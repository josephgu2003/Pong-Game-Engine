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

uniform vec3 skyColor;

    void main()
    {
        vec4 fragColor = vec4(0,0,0,0);
        fragColor.rgb = skyColor;
        vec3 pos = normalize(TexVec);
        float gradient = max(2.0 * atan(pos.y, length(vec2(pos.x, pos.z))) / 3.1415, 0.0); // 0 -> 1
       // gradient = 1.0 / (1.0 + exp(-70.0 * gradient + 68.0));
        gradient = smoothstep(0.02, 0.18, gradient);
        fragColor.rgb = mix(fogColor, fragColor.rgb, gradient);
        fragColor.a = 1.0;
        FragColor = fragColor;
    }
