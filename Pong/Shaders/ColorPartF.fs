#version 410 core

    out vec4 FragColor;

    in vec2 TexCoords;

uniform sampler2D texture0;
    uniform float duration;
uniform vec4 color;

    void main()
    {
        vec4 fragColor = color;
       fragColor.a = sqrt(texture(texture0, gl_PointCoord).r) * sqrt(sqrt(duration / 1.0));
       fragColor.rgb = mix(fragColor.rgb, vec3(1.0,1.0,1.0), fragColor.a);
        FragColor = fragColor;
    }
