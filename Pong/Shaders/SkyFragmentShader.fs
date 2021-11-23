#version 410 core
    out vec4 FragColor;
    in vec3 TexVec;

    uniform samplerCube skyBox;
    uniform float brightness;

    void main()
    {
        vec4 fragColor = (brightness)*texture(skyBox, TexVec); //brightness
        vec3 direction = vec3(TexVec.x,0.0,TexVec.z);
        float gradient = dot(normalize(TexVec), normalize(direction));
        fragColor.a = 1.0;
        FragColor = fragColor;
    }
