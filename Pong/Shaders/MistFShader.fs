#version 410 core
layout (location = 0) out vec4 FragColor;

    in vec2 TexCoords;
in float duration;
    uniform sampler2D alphaMap;
uniform sampler2D gradient;

    void main()
    {
        if (duration < 0.0) discard;
        float alpha = texture(alphaMap, TexCoords).r;
        vec3 colors = vec3(0.2,0.2,0.2);
       // sampled.w *= 5*log(duration);
        float intensity = texture(gradient, TexCoords).r;

        FragColor = vec4(colors*alpha*intensity, alpha*intensity*intensity);
    }
