#version 410 core
    out vec4 FragColor;
    in vec3 TexVec;

    uniform samplerCube skyBox;
    uniform float brightness;

    void main()
    {
        FragColor = brightness * texture(skyBox, TexVec);
    }
