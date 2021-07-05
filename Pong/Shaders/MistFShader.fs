#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D texture0;
uniform sampler2D texture1;
    uniform float duration;

    void main()
    {
        vec4 sampled = texture(texture0, TexCoords);
        vec3 colors = sampled.rgb;
       // sampled.w *= 5*log(duration);
        float intensity = texture(texture1, vec2((TexCoords.x-0.5)+0.5,(TexCoords.y-0.5)+0.5)).b;
        sampled.a *= 0.08*intensity*intensity;
        sampled.a *= min(duration,1);
        FragColor = vec4(colors, sampled.a);
    }
