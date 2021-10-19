#version 410 core
#extension GL_EXT_texture_array : enable

    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2DArray diffuse;
    uniform sampler2D gradient;

in float duration;
 
    void main()
    {

        float duration1 = duration;
        duration1 *= float(36.0);
        int layer = 109;
        layer -= int(floor(duration1));
        vec4 sampled = vec4(0.1,0.1,0.1,1.0-texture(diffuse, vec3(gl_PointCoord, layer)).r);
        
        float multiplier = duration-1.5;
        float intensity = texture(gradient, gl_PointCoord).b;
        sampled.w *= intensity*intensity;
        sampled.w *= min((5.0/multiplier/multiplier/multiplier/multiplier),1);
        FragColor = sampled;
    }
