#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D fbotexture;

    void main()
    {
            FragColor = texture(fbotexture, TexCoords);
    }
