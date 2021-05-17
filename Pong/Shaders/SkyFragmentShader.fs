#version 410 core
    out vec4 FragColor;
    in vec3 TexVec;

    uniform samplerCube skyBox;

    void main()
    {
        FragColor = texture(skyBox, TexVec);
    }
