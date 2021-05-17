#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D mapTexture;

    void main()
    {
        FragColor = texture(mapTexture, TexCoords);
    }
