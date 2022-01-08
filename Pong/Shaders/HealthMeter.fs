#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
in vec2 TexCoords;

    uniform sampler2D diffuse;
    uniform float ratio;

    void main()
    {
        vec4 sampled = texture(diffuse, TexCoords);
        float ratio2 = pow(ratio, 1.5);
        FragColor = sampled;
        FragColor.a *= 2.0*ratio2;
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
