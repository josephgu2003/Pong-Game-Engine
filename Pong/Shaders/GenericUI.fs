#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

    uniform sampler2D diffuse;

uniform float alpha;

    void main()
    {
        vec4 sampled = texture(diffuse, TexCoords);

        FragColor = sampled;
        FragColor.a = alpha * FragColor.a;
        BrightColor = vec4(0,0,0,alpha);
    }
