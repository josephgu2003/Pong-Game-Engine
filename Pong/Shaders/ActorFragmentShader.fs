#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D texture0;
    uniform sampler2D texture1;
    uniform float brightness;
    uniform float light;

    void main()
    {
        vec4 lightColor = light*vec4(1,1,1,0);
        vec4 brightnessColor = brightness*vec4(1,1,1,0);
        FragColor = (lightColor+brightnessColor) * (mix(texture(texture0, TexCoords), texture(texture1, TexCoords), 0.5));
    }
