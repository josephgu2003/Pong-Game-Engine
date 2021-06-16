#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D texture0;
    uniform float duration;

    void main()
    {
        vec4 sampled = texture(texture0, TexCoords);
        vec3 colors = sampled.rgb;
       // sampled.w *= 5*log(duration);
        FragColor = vec4 (colors, sampled.a);
    }
