#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D texture0;
    uniform sampler2D texture1;

    void main()
    {
        FragColor = mix(texture(texture0, TexCoords), texture(texture1, TexCoords), 0.5);
    }
