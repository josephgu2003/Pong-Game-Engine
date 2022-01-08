#version 410 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D brightColors;
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
uniform float weights[12] = float[](0.079788,0.078208,0.073654,0.066644,0.057938,0.048394,0.038837,0.029945,0.022184,0.015790,0.010798,0.007094);

    void main() 
    {
        vec2 tex_offset = 1.0 / textureSize(brightColors, 0);

        vec3 sampled =  texture(brightColors, TexCoords).rgb * weight[0];
        if (horizontal) {
            for (int i = 1; i < 5; ++i) {
                sampled += texture(brightColors, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
                   sampled += texture(brightColors, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
        }
        else {
            for (int i = 1; i < 5; ++i) {
                sampled += texture(brightColors, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
                   sampled += texture(brightColors, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            }
        }
        FragColor = vec4(sampled,1);

    }
