#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;
in float duration;
    uniform sampler2D diffuse;
uniform sampler2D gradient;

    void main()
    {
        vec4 sampled = texture(diffuse, gl_PointCoord);
        vec3 colors = sampled.rgb;
       // sampled.w *= 5*log(duration);
        float intensity = texture(gradient, gl_PointCoord).b;
        sampled.a *= 0.04*intensity*intensity*intensity;
        sampled.a *= min(duration,1);
        FragColor = vec4(colors, sampled.a);
    }
